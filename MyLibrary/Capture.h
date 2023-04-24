#pragma once
#include<vector>
#include<unordered_map>
#include<string>
#include<Windows.h>

#include"Library.h"
#include"Texture.h"

namespace MelLib 
{
	// �����_�[�^�[�Q�b�g���w��ł���悤�ɂ����Ⴈ��

	// ��ʂ��L���v�`������N���X
	class ScreenCapture final
	{
	private:
		ScreenCapture(){}
		~ScreenCapture(){}
	public:
		ScreenCapture(ScreenCapture& s) = delete;
		ScreenCapture& operator=(ScreenCapture& s) = delete;
		static ScreenCapture* GetInstance();

		/// <summary>
		/// �L���v�`���������ʂ�Texture�^�Ŏ󂯎��܂��B
		/// </summary>
		/// <param name="texture"></param>
		/// <param name="hwnd">�L���v�`���ΏہB�w�肵�Ȃ��ꍇ�A���C�u�����Ő��������E�B���h�E���w�肳���</param>
		void GetCaptureData(Texture& refTexture,HWND hwnd = Library::GetHWND());

		/// <summary>
		/// �L���v�`���������ʂ�Texture�^�Ŏ󂯎��܂��B
		/// </summary>
		/// <param name="refTexture"></param>
		/// <param name="windowName">�L���v�`���������E�B���h�E�̖��O</param>
		void GetCaptureData(Texture& refTexture, const std::wstring& windowName);
	};


	// �����L���v�`���ł���悤�ɂ��邽�߂ɃV���O���g������Ȃ�����
	// �V�[�����܂����Ř^�悷��\�������邽�߁A�^��̊J�n�I�����̂̓V���O���g���ōs�����ق����ǂ�

	// �E�B���h�E��^�悷��N���X
	class VideoCapture
	{
	private:
		// �^�悵���f�[�^
		std::vector<std::vector<std::vector<Texture>>>captureData;

		HWND hwnd = nullptr;
	public:

		/// <summary>
		/// �L���v�`�����J�n���܂�
		/// </summary>
		/// <param name="refCaptureAddVector"></param>
		/// <param name="hwnd"></param>
		void StartCapture(HWND hwnd = nullptr);

		void EndCapture();

		/// <summary>
		/// ���݂̉�ʂ̃t���[���̃L���v�`�������e�N�X�`��
		/// </summary>
		/// <param name="refTexture"></param>
		void GetCurrentFrameCaptureTexture(Texture& refTexture)const;


		/// <summary>
		/// �����ŌĂ΂�邽�߁A�Ăяo���K�v�Ȃ�
		/// </summary>
		void Update();
	};

	// �^����Ǘ�����N���X
	class VideoCaptureManager final 
	{
	private:

		

	private:
		// �o�^��,�L���v�`��
		std::unordered_map<std::string,VideoCapture*>pVideoCaptures;

	private:
		VideoCaptureManager() {}
		~VideoCaptureManager() {}
	public:
		VideoCaptureManager(VideoCaptureManager& v) = delete;
		VideoCaptureManager& operator=(VideoCaptureManager& v) = delete;
		static VideoCaptureManager* GetInstance();

		void StartCapture(const std::string& name);
		void EndCapture(const std::string& name);
		const VideoCapture& GetCapture(const std::string& name) { return *pVideoCaptures.at(name); }

		void Update();
	};
}

