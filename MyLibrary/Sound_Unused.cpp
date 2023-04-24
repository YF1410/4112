//#include "Sound.h"
//#include<fstream>
//#include<assert.h>
//
//
//class XAudio2VoiceCallback :public IXAudio2VoiceCallback
//{
//public:
//	STDMETHOD_(void, OnVoiceProcessingPassStart)(THIS_ UINT32 BytesRequired) {};
//	STDMETHOD_(void, OnVoiceProcessingPassEnd)(THIS) {};
//	STDMETHOD_(void, OnStreamEnd)(THIS) {};
//	STDMETHOD_(void, OnBufferStart)(THIS_ void* pBufferContext) {};
//	STDMETHOD_(void, OnBufferEnd)(THIS_ void* pBufferContext)
//	{
//		delete[] pBufferContext;
//	};
//	STDMETHOD_(void, OnLoopEnd)(THIS_ void* pBufferContrxt) {};
//	STDMETHOD_(void, OnVoiceError)(THIS_ void* pBufferContrxt, HRESULT) {};
//
//};
//class XAudio2VoiceCallback2 :public IXAudio2VoiceCallback
//{
//public:
//	STDMETHOD_(void, OnVoiceProcessingPassStart)(THIS_ UINT32 BytesRequired) {};
//	STDMETHOD_(void, OnVoiceProcessingPassEnd)(THIS) {};
//	STDMETHOD_(void, OnStreamEnd)(THIS) {};
//	STDMETHOD_(void, OnBufferStart)(THIS_ void* pBufferContext) {};
//	STDMETHOD_(void, OnBufferEnd)(THIS_ void* pBufferContext)
//	{
//		delete[] pBufferContext;
//	};
//	STDMETHOD_(void, OnLoopEnd)(THIS_ void* pBufferContrxt) {};
//	STDMETHOD_(void, OnVoiceError)(THIS_ void* pBufferContrxt, HRESULT) {};
//
//};
//
//XAudio2VoiceCallback voiceCallback;
//XAudio2VoiceCallback2 voiceCallback2;
//
//ComPtr<IXAudio2> Sound::xAudio2;
//IXAudio2MasteringVoice* Sound::masterVoice;
//
//Sound::Sound()
//{
//	
//
//}
//
//
//Sound::~Sound()
//{
//}
//
//void Sound::Initialize()
//{
//	XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
//	xAudio2->CreateMasteringVoice(&masterVoice);
//}
//
//bool Sound::Load(const std::string& path, const bool loopFlag, const std::string& name)
//{
//	pSound.emplace(name, std::make_unique<Sound>());
//	return pSound[name]->LoadSound(path.c_str(), name.c_str(), loopFlag);
//}
//
//
//void Sound::PlayWave(const char* path)
//{
//	HRESULT result;
//
//#pragma region �t�@�C���I�[�v��
//	std::ifstream file;
//	file.open(path, std::ios_base::binary);
//
//	if (file.fail())assert(0);
//#pragma endregion
//
//#pragma region �ǂݍ���
//	WAVRiffHeader riff;
//	file.read((char*)&riff, sizeof(riff));
//	if (strncmp(riff.chunk.id, "RIFF", 4) != 0)assert(0);
//
//	WAVFormatChunk format;
//	file.read((char*)&format, sizeof(format));
//
//	WAVChunk chunk;
//	file.read((char*)&chunk, sizeof(chunk));
//
//	char* pBuffer = new char[chunk.size];
//	file.read(pBuffer, chunk.size);
//
//	file.close();
//#pragma endregion
//
//#pragma region �Đ�
//	WAVEFORMATEX wfex{};
//
//	//�g�`�t�H�[�}�b�g�̐ݒ�
//	memcpy(&wfex, &format.fmt, sizeof(format.fmt));
//	wfex.wBitsPerSample = format.fmt.nBlockAlign * 8 / format.fmt.nChannels;
//
//	IXAudio2SourceVoice* pSourceVoice = nullptr;
//	//result = xAudio2->CreateSourceVoice(&pSourceVoice, &wfex);
//	result = xAudio2->CreateSourceVoice(&pSourceVoice, &wfex, 0, 2.0f, &voiceCallback);
//	if FAILED(result)
//	{
//		delete[] pBuffer;
//		return;
//	}
//
//	XAUDIO2_BUFFER buf{};
//	buf.pAudioData = (BYTE*)pBuffer;
//	buf.pContext = pBuffer;
//	buf.Flags = XAUDIO2_END_OF_STREAM;
//	buf.AudioBytes = chunk.size;
//
//
//	result = pSourceVoice->SubmitSourceBuffer(&buf);
//	result = pSourceVoice->Start();
//
//#pragma endregion
//
//}
//
//bool Sound::LoadSound(const char* path, std::string name, bool loop)
//{
//	HRESULT result;
//
//#pragma region �t�@�C���I�[�v��
//	std::ifstream file;
//	file.open(path, std::ios_base::binary);
//
//	if (file.fail())
//	{
//
//#ifdef _DEBUG
//		OutputDebugStringA(path);
//		OutputDebugStringW(L"�̓ǂݍ��݂Ɏ��s���܂����Bpath�̓��̓~�X�̉\��������܂��B\n");
//		return false;
//#endif // _DEBUG
//	}
//#pragma endregion
//
//#pragma region �ǂݍ���
//	WAVRiffHeader riff;
//	file.read((char*)&riff, sizeof(riff));
//	if (strncmp(riff.chunk.id, "RIFF", 4) != 0)assert(0);
//
//	WAVFormatChunk format;
//	file.read((char*)&format, sizeof(format));
//
//	WAVChunk chunk;
//	file.read((char*)&chunk, sizeof(chunk));
//
//	char* pBuffer = new char[chunk.size];
//	file.read(pBuffer, chunk.size);
//
//	file.close();
//#pragma endregion
//
//#pragma region �Đ�
//	WAVEFORMATEX wfex{};
//
//	�g�`�t�H�[�}�b�g�̐ݒ�
//	memcpy(&wfex, &format.fmt, sizeof(format.fmt));
//	wfex.wBitsPerSample = format.fmt.nBlockAlign * 8 / format.fmt.nChannels;
//
//	IXAudio2SourceVoice* pSourceVoice = nullptr;
//	result = xAudio2->CreateSourceVoice(&pSourceVoice, &wfex);
//	result = xAudio2->CreateSourceVoice(&pSourceVoice, &wfex, 0, 2.0f, &voiceCallback2);
//	if FAILED(result)
//	{
//		delete[] pBuffer;
//
//#ifdef _DEBUG
//		OutputDebugStringA(path);
//		OutputDebugStringW(L"�̓ǂݍ��݂Ɏ��s���܂����B�Ή����Ă��Ȃ��\��������܂��B\n");
//#endif // _DEBUG
//
//
//		return false;
//	}
//
//	XAUDIO2_BUFFER buf{};
//	buf.pAudioData = (BYTE*)pBuffer;
//	buf.pContext = pBuffer;
//	buf.Flags = XAUDIO2_END_OF_STREAM;
//	buf.AudioBytes = chunk.size;
//	if(loop)
//	buf.LoopCount = XAUDIO2_LOOP_INFINITE;
//	else
//		buf.LoopCount = 0;
//	
//
//	result = pSourceVoice->SubmitSourceBuffer(&buf);
//	pLoadSourceVoices.emplace(name, pSourceVoice);
//
//#pragma endregion
//
//
//	return true;
//}
//
//void Sound::PlayLoadSound(std::string name)
//{
//	pLoadSourceVoices[name]->Start();
//}
//
//void Sound::StopLoadSound(std::string name, bool resetFlag)
//{
//	if(!resetFlag)
//	pLoadSourceVoices[name]->Stop();
//	else 
//	{
//		pLoadSourceVoices[name]->Stop(XAUDIO2_PLAY_TAILS);
//		pLoadSourceVoices[name]->Stop(0);
//	}
//}
