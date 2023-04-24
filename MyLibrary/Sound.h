#pragma once
#include<xaudio2.h>
#pragma comment(lib,"xaudio2.lib")
#include<wrl.h>
#include<inttypes.h>
#include<string>
#include<unordered_map>
#include"SoundData.h"
using namespace Microsoft::WRL;


namespace MelLib
{
	//�T�E���h�̃p�����[�^
	struct PlaySoundData
	{
		//�{�����[��(�p�[�Z���g)
		float volume = 50.0f;

	};

	//�T�u�~�b�N�X�{�C�X�Ń\�[�X�{�C�X���܂Ƃ߂ĊǗ��ł���炵��
	//�u���@�F�\�[�X�{�C�X�R�[���o�b�N���g�p����v�ɃR�[���o�b�N���Ă΂ꂽ�Ƃ��Ƀ\�[�X�{�C�X��������肷�鏈�������Ă���?
	//https://docs.microsoft.com/en-us/windows/win32/xaudio2/how-to--use-source-voice-callbacks

	//�قڂ��ׂĂ�SE�𒲐��������Ƃ��Ƃ��͂ǂ����邩�B
	//PlayLoadSound�łǂ̃T�u�~�b�N�X�{�C�X�ɒǉ����邩��I�ׂ�悤�ɂ���?
	//�T�u�~�b�N�X�{�C�X������悤�ɂ���?�T�u�~�b�N�X�{�C�X���ƕ�����ɂ�����������Ȃ�����T�E���h�O���[�v�Ƃ��ɂ���?

	//�T�E���h��ModelObject�Ƃ�Sprite2D�݂����Ɏ����ŊǗ��ł���悤�ɂ���?

	//�\�[�X�{�C�X�����Ȃ��Ă����߂���Đ�����������ۂ�

	//SoundData�̉������Đ���������H����N���X
	class Sound
	{
	private:
		//�R�[���o�b�N
		class XAudio2VoiceCallback :public IXAudio2VoiceCallback
		{
		public:
			HANDLE streamEndEvent;
			XAudio2VoiceCallback();
			~XAudio2VoiceCallback();

			STDMETHOD_(void, OnVoiceProcessingPassStart)(THIS_ UINT32 BytesRequired) {};
			STDMETHOD_(void, OnVoiceProcessingPassEnd)(THIS) {};

			//�w�肵�����[�v�����Đ�������Ă΂��
			STDMETHOD_(void, OnStreamEnd)(THIS);
			STDMETHOD_(void, OnBufferStart)(THIS_ void* pBufferContext) {};
			STDMETHOD_(void, OnBufferEnd)(THIS_ void* pBufferContext) {};
			STDMETHOD_(void, OnLoopEnd)(THIS_ void* pBufferContrxt) {};
			STDMETHOD_(void, OnVoiceError)(THIS_ void* pBufferContrxt, HRESULT errow) {};

		};
		XAudio2VoiceCallback voiceCallback;

		static std::unordered_map<std::string, std::unique_ptr<Sound>> pSounds;
		static std::vector<std::unique_ptr<Sound>>pNoneNameSounds;

		static IXAudio2* iXAudio2;
		IXAudio2SourceVoice* pSourceVoice;

		SoundData* pSoundData;
		PlaySoundData playSoundData;

		//���[�v��
		UINT32 loopNum = 0;
		//���[�v��
		UINT loopCount = 0;

		//�Đ����I�������u�Ԃ��ǂ���
		bool playEndMoment = false;
		//�Đ����I���������ǂ���
		bool playEnd = false;


		bool Play(SoundData* soundData, const UINT32 loopNum, const PlaySoundData& playSoundData);
		void CheckPlayEnd();
	public:
		virtual ~Sound();

		static const UINT32 LOOP_INFINITY = XAUDIO2_LOOP_INFINITE;
		static const UINT32 LOOP_MAX = XAUDIO2_MAX_LOOP_COUNT;

		static void Initialize(IXAudio2* pIXAudio2);
		static void Update();
		static void Finalize();


		/// <summary>
		/// SoundData���Đ����܂��B����name�ɖ��O��n����Get�֐��ŎQ�Ƃł���悤�ɂȂ�A�����̃f�[�^���ォ��ݒ肵����擾�����肷�邱�Ƃ��ł��܂��B
		/// name�Ɉ�����n�����ꍇ�͍Đ��I����Ƀ��������������Ȃ����߁AStopSound�֐����Ăяo���A�蓮�ŉ�����Ă��������B
		/// </summary>
		/// <param name="soundData">�����f�[�^</param>
		/// <param name="loopNum">���[�v��(0����254(LOOP_MAX)�܂���LOOP_INFINITY)</param>
		/// <param name="playSoundData">�T�E���h�̐ݒ���܂Ƃ߂�����</param>
		/// <param name="name">�o�^��</param>
		/// <returns></returns>
		static bool PlayLoadSound(SoundData* soundData, const UINT32 loopNum, const PlaySoundData& playSoundData, const std::string& name = "");

		static Sound* Get(const std::string& name) { return pSounds[name].get(); }

		/// <summary>
		/// �Đ����I�����܂��BGet�֐��ł̎Q�Ƃ��s�\�ɂȂ�̂Œ��ӂ��Ă��������B
		/// </summary>
		/// <param name="name"></param>
		static void StopSound(const std::string& name);

		/// <summary>
		/// �ŏ�����Đ����Ȃ����܂��B
		/// </summary>
		/// <param name="name"></param>
		void ResetSound();



#pragma region �Z�b�g

		/// <summary>
		/// playSoundData�̃f�[�^��S�ăZ�b�g���܂��B
		/// </summary>
		/// <param name="playSoundData"></param>
		void SetPlaySoundData(const PlaySoundData& playSoundData);

		/// <summary>
		/// ���ʂ�ݒ肵�܂��B
		/// </summary>
		/// <param name="volume">����(0����100�̃p�[�Z���g)</param>
		void SetSoundVolume(const float volume);

		/// <summary>
		/// �ꎞ��~�t���O���Z�b�g���܂��B
		/// </summary>
		/// <param name="flag"></param>
		void SetSoundPauseFlag(const bool flag);


#pragma endregion

#pragma region �Q�b�g
		/// <summary>
		/// �Đ����I�������u�Ԃ�true��Ԃ��܂��B
		/// </summary>
		/// <returns></returns>
		bool GetPlayEndMomentFlag() { return playEndMoment; }

		/// <summary>
		/// �Đ����I�����Ă�����true��Ԃ��܂��B
		/// </summary>
		/// <returns></returns>
		bool GetPlayEnd() { return playEnd; }

		UINT GetLoopNumber() { return loopNum; }

		UINT GetLoopCount() { return loopCount; }
#pragma endregion

	};

}