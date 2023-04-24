#pragma once
#include<xaudio2.h>
#pragma comment(lib,"xaudio2.lib")
#include<x3daudio.h>

#include<wrl.h>
#include<inttypes.h>
using namespace Microsoft::WRL;

namespace MelLib 
{
	//�I�[�f�B�I�N���X(XAudio2��X3DAudio�Ǘ��N���X)
	class Audio
	{
	private:
		Audio() {}
		~Audio() {}

		//XAudio2
		ComPtr<IXAudio2>xAudio2;
		//�}�X�^�[�{�C�X(IXAudio2�Ɠ����Ɏ����I�ɉ�������)
		IXAudio2MasteringVoice* masterVoice = nullptr;

		

	public:
		Audio(const Audio& a) = delete;
		Audio& operator=(const Audio& a) = delete;
		static Audio* GetInstance();

		void Initialize();

	};

}