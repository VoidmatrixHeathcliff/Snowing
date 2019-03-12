#pragma once
#include <future>
#include <Snowing.h>

namespace Yukimi
{
	class[[nodiscard]] AudioChannel final :
	        public Snowing::MemPool<AudioChannel>,
		public Snowing::Scene::Object
	{
	private:
		std::future<Snowing::Blob> soundLoading_;
		Snowing::Blob sound_;
		Snowing::Audio::SoundPlayer player_;
		Snowing::Scene::Tween<float> fadeVolume_, mainVolume_ = 1;
		Snowing::Scene::VirtualConditionTask onSoundLoaded_;

		std::optional<uint32_t> pausePoint_;

	public:

		using AudioLoader = Snowing::Blob(*)(Snowing::AssetName);

		// �첽���ز�����һ������
		AudioChannel(AudioLoader loader,Snowing::AssetName,float fadeIn = 0,uint32_t begin = 0,float pan = 0);

		// ֹͣ��������
		void Stop(float fadeOut = 0);

		// ��ȡ����λ��
		uint32_t GetPosition() const;

		// ��ͣ
		void Pause(float fadeOut = 0);

		// ����ͣ�лָ�
		void Resume(float fadeIn = 0);

		// ���뵭����������
		void FadeVolume(float volume,float fade = 0);

		// ���������Ž�������ȫֹͣ�󣬶�������
		bool Update() override;
	};
}