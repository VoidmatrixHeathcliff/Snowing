#pragma once
#include "Blob.h"
#include <cstdint>
#include <cassert>

namespace Snowing::Audio
{
	template <typename TImpl>
	class[[nodiscard]] SoundPlayerInterface final
	{
	private:
		TImpl impl_;

	public:
		SoundPlayerInterface()
		{}

		//����Ҫ���ŵ�Blob�Ϳ�ʼ���ŵ�λ��
		void Play(const Blob *blob,uint32_t begin = 0)
		{
			impl_.Play(blob,begin);
		}

		[[nodiscard]]
		uint32_t GetPosition() const
		{
			return impl_.GetPosition();
		}

		//����ֹͣ�����ص�ʱ���ڲ��ŵ�λ��
		uint32_t Stop()
		{
			const auto ret = impl_.GetPosition();
			impl_.Stop();
			return ret;
		}

		//����ʱ���ر����ŵ�Blob��ֹͣʱ����nullptr
		[[nodiscard]]
		const Blob * GetPlaying() const
		{
			return impl_.GetPlaying();
		}

		//��Χ��0~1
		void SetVolume(float v)
		{
			assert(v >= 0 && v <= 1);
			impl_.SetVolume(v);
		}

		//��Χ��-1~1��-1Ϊ����1Ϊ���ң�
		void SetPan(float pan)
		{
			assert(pan >= -1 && pan <= 1);
			impl_.SetPan(pan);
		}

		//��Χ��0~������
		void SetSpeed(float spd)
		{
			assert(spd > 0);
			impl_.SetSpeed(spd);
		}

		void ResetStates()
		{
			SetSpeed(1);
			SetPan(0);
			SetVolume(1);
		}
	};
}