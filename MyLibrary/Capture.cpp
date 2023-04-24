#include "Capture.h"

MelLib::ScreenCapture* MelLib::ScreenCapture::GetInstance()
{
	static ScreenCapture s;
	return &s;
}

void MelLib::ScreenCapture::GetCaptureData(Texture& refTexture, HWND hwnd)
{
	// Windows GDI�œǂݍ���



}

void MelLib::ScreenCapture::GetCaptureData(Texture& refTexture, const std::wstring& windowName)
{
	// �w�肵���E�B���h�E�̃n���h�����擾
	HWND hwnd = FindWindow(NULL, windowName.c_str());

	HDC hdcWindow = GetDC(hwnd);

	BITMAP bmp;
	GetObject(hdcWindow, sizeof(BITMAP), &bmp);

	// �����炭�摜�����̃T�C�Y���w�肷��΂����񂾂낤���ǁA�`���[�g���A���ł킩��Ȃ������������Ă悭�킩���
	// ����������ƃo�C�g����Ȃ��ăr�b�g�Ŏw�肷��Ƃ�?
	size_t bmpDataSize = (bmp.bmWidth * bmp.bmHeight) * 4;

	// GHND�͉������������蓖�āA�X��0�ŏ�����
	// �摜�f�[�^�̓T�C�Y�ɂ���ĕK�v���������Ⴄ����GHND�ɂ��Ă�
	//HANDLE hDIB = GlobalAlloc(GHND,);

	// GetDIBits�ł����炭�摜���擾�ł���

	BITMAPINFOHEADER header;
	header.biSize = sizeof(BITMAPINFOHEADER);
	header.biWidth = bmp.bmWidth;
	header.biHeight = bmp.bmHeight;
}

void MelLib::VideoCapture::StartCapture(HWND hwnd)
{
}

void MelLib::VideoCapture::EndCapture()
{
}

void MelLib::VideoCapture::GetCurrentFrameCaptureTexture(Texture& refTexture)const 
{
}

void MelLib::VideoCapture::Update()
{
}

MelLib::VideoCaptureManager* MelLib::VideoCaptureManager::GetInstance()
{
	static VideoCaptureManager v;
	return &v;
}

void MelLib::VideoCaptureManager::Update()
{
}
