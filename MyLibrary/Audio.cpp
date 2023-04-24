#include "Audio.h"
#include"Sound.h"


using namespace MelLib;

Audio* Audio::GetInstance()
{
	static Audio a;
	return &a;
}

void Audio::Initialize()
{
	XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	xAudio2->CreateMasteringVoice(&masterVoice);

	Sound::Initialize(xAudio2.Get());


	//// X3DAudio����������
	//DWORD channelMask;
	//masterVoice->GetChannelMask(&channelMask);

	//X3DAUDIO_HANDLE handle;
	//X3DAudioInitialize(channelMask, X3DAUDIO_SPEED_OF_SOUND, handle);

	//// �������牺�̓\�[�X�{�C�X���Ƃɍs������?
	//X3DAUDIO_LISTENER listener = {};
	//X3DAUDIO_EMITTER emitter = {};
	//emitter.ChannelCount = 1;
	//emitter.CurveDistanceScaler = FLT_MIN;

	//// X3DAudio�̌v�Z���ʂ���������\����
	//X3DAUDIO_DSP_SETTINGS dspSettings = {};

	//dspSettings.SrcChannelCount = 1;

	//XAUDIO2_VOICE_DETAILS ditails;
	//masterVoice->GetVoiceDetails(&ditails);
	//dspSettings.DstChannelCount = ditails.InputChannels;

	//FLOAT32* matrix = new FLOAT32[dspSettings.DstChannelCount];
	//dspSettings.pMatrixCoefficients = matrix;




}
