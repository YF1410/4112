//#pragma once
//#include<xaudio2.h>
//#pragma comment(lib,"xaudio2.lib")
//#include<wrl.h>
//#include<inttypes.h>
//#include<unordered_map>
//using namespace Microsoft::WRL;
//
//struct WAVChunk
//{
//	char id[4];
//	int32_t size;
//};
//
//struct WAVRiffHeader
//{
//	WAVChunk chunk;
//	char type[4];
//};
//
//struct WAVFormatChunk
//{
//	WAVChunk chunk;
//	WAVEFORMAT fmt;
//};
//
////�\�[�X�{�C�X�͒萔�o�b�t�@�AXAUDIO2_BUFFER�͒��_�o�b�t�@�I�ȍl���ł���?
////XAUDIO2_BUFFER�g���܂킵�ă\�[�X�{�C�X�����p�ӂ���Ε����Đ��ł���?����?
////�Đ��ʒu���Z�b�g�Ȃ����ǁA���������ă\�[�X�{�C�X�����O��?Stop�֐��̓o�b�t�@�폜���邽�߂̂���?�ł�Stop�Ƀ��Z�b�g���ۂ������̂������悤��
//
//
////�T�E���h�N���X
//class Sound
//{
//private:
//
//	static ComPtr<IXAudio2>xAudio2;
//	//�}�X�^�[�{�C�X(IXAudio2�Ɠ����Ɏ����I�ɉ�������)
//	static IXAudio2MasteringVoice* masterVoice;
//
//	IXAudio2SourceVoice* spurceVoice;
//
//	//�T�E���h��u_map
//	std::unordered_map<std::string, IXAudio2SourceVoice*> pLoadSourceVoices;
//
//
//	static std::unordered_map<std::string, std::unique_ptr<Sound>> pSound;
//	char* pBuffer = nullptr;
//
//
//	static std::unordered_map<std::string, ComPtr<IXAudio2SourceVoice>> pSourceVoice;
//public:
//	Sound();
//	~Sound();
//
//	static void Initialize();
//	static bool Load(const std::string& path,const bool loopFlag,const std::string& name);
//	static Sound* Get(const std::string& name) { return pSound[name].get(); }
//	static void Delete(const std::string& name);
//
//	//void PlayWave(const char* path);
//
//
//	bool LoadSound(const char* path, std::string name, bool loop);
//	void PlayLoadSound(std::string name);
//	void StopLoadSound(std::string name,bool resetFlag);
//	void EraseLoadSound(const std::string& name);
//};
//
