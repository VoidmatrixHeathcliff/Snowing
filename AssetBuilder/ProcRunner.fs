﻿module ProcRunner
open Job
open System.IO
open System

let private FilesLastWriteTime (files : string list) = 
    files
    |> List.filter (String.IsNullOrWhiteSpace >> not)
    |> List.map FileInfo
    |> List.map (fun x -> x.LastWriteTimeUtc)
    |> List.max

let private ShoudRebuild (job : Job) : bool =
    try
        let outputTime =
            let outputFile =
                job.OutputPath
                |> FileInfo
            outputFile.LastWriteTimeUtc
        
        match job.Processor.InputType with
        | None -> true
        | File -> 
            let inputTime = 
                let inputFile =
                    job.Input.Head |> FileInfo
                inputFile.LastWriteTimeUtc
            inputTime >= outputTime
        | Files -> 
            let inputTime = FilesLastWriteTime job.Input
            inputTime >= outputTime
        | Directory ->
            let dir = 
                job.Input
                |> List.head
                |> DirectoryInfo
            let inputTime =
                dir.EnumerateFiles ("*",SearchOption.AllDirectories)
                |> Seq.toList
                |> List.map (fun x -> x.FullName)
                |> FilesLastWriteTime
            inputTime >= outputTime
    with _ -> true

let RunProcs (procs : Job[]) outputPath =  
    let failed = ref 0
    let logLock = obj()
    procs
    |> Array.filter ShoudRebuild
    |> Array.Parallel.iter (fun job ->
        if File.Exists job.OutputPath then
            File.Delete job.OutputPath

        try
            job.Processor.Proc job
            if job.Processor.FinishLogEnabled then
                lock logLock (fun () -> 
                    printfn "%s: %A -> %s"
                        job.Processor.Command
                        job.Input
                        job.OutputPath.[String.length outputPath + 1..])

        with jobException ->
            System.Threading.Interlocked.Increment(failed) |> ignore
            lock logLock (fun () ->
                try
                    File.Delete job.OutputPath
                with _ -> ()
                Console.ForegroundColor <- ConsoleColor.Red
                printfn "Exception in %s" job.Processor.Command
                printfn "JobInfo:"
                printfn "%A" job
                printfn "ExceptionMessage:%s" jobException.Message
                printfn "ExceptionInfo:" 
                printfn "%A" jobException
                Console.ResetColor ()
                Console.Beep ()))

    if !failed > 0 then
        Directory.Delete (outputPath,true)