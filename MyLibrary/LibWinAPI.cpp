#include "LibWinAPI.h"

//�q�E�B���h�E�́AWNDCLASSEX��o�^�����Ⴂ���Ȃ��B�o�^����ƃ{�^���Ƃ����Ȃ�

//�q�E�B���h�E���Ƃ���proc�n���Ȃ��悤�ɂ���

//���ꃁ���o�ϐ��ɂ��Ȃ���HWND��NULL�ɂȂ�
//WNDCLASSEX LibWinAPI::w;
using namespace MelLib;

HWND LibWinAPI::CreateNormalWindow
(
	const std::wstring& className,
	const std::wstring& winName,
	const DWORD& windowStyle,
	const DWORD& posX,
	const DWORD& posY,
	const int& width,
	const int& height,
	const HWND parentHWND,
	const WNDPROC& winProc
)
{
	WNDCLASSEX w;
	if (!parentHWND)
	{
		w = {};
		w.cbSize = sizeof(WNDCLASSEX);
		w.lpfnWndProc = (WNDPROC)winProc;
		w.lpszClassName = className.c_str();
		w.hInstance = GetModuleHandle(nullptr);
		w.hCursor = LoadCursor(NULL, IDC_ARROW);

		RegisterClassEx(&w);
	}
	RECT wrc = { 0,0,width,height };
	AdjustWindowRect(&wrc, windowStyle, false);


	HWND hwnd;
	hwnd = CreateWindow
	(
		className.c_str(),
		winName.c_str(),
		windowStyle,
		posX,
		posY,
		wrc.right - wrc.left,
		wrc.bottom - wrc.top,
		parentHWND,
		nullptr,
		GetModuleHandle(nullptr),
		nullptr);

	ShowWindow(hwnd, SW_SHOW);

	return hwnd;
}

HWND LibWinAPI::CreateExpansionWindow(const std::wstring& className, const std::wstring& winName, const DWORD& expansionWindowStyle, const DWORD& windowStyle, const DWORD& posX, const DWORD& posY, const int& width, const int& height, const HWND parentHWND, const WNDPROC& winProc)
{
	WNDCLASSEX w;
	if (!parentHWND)
	{
		w = {};
		w.cbSize = sizeof(WNDCLASSEX);
		w.lpfnWndProc = (WNDPROC)winProc;
		w.lpszClassName = className.c_str();
		w.hInstance = GetModuleHandle(nullptr);
		w.hCursor = LoadCursor(NULL, IDC_ARROW);

		RegisterClassEx(&w);
	}
	RECT wrc = { 0,0,width,height };
	AdjustWindowRectEx(&wrc, windowStyle, false, expansionWindowStyle);


	HWND hwnd;
	hwnd = CreateWindowEx
	(
		expansionWindowStyle,
		className.c_str(),
		winName.c_str(),
		windowStyle,
		posX,
		posY,
		wrc.right - wrc.left,
		wrc.bottom - wrc.top,
		parentHWND,
		nullptr,
		GetModuleHandle(nullptr),
		nullptr);

	ShowWindow(hwnd, SW_SHOW);

	return hwnd;
}



//
//HWND LibWinAPI::createExpansionWindow
//(
//	const std::wstring& windowName,
//	const int& width,
//	const int& heighr,
//	const int& windowStyle,
//	const HWND parentHWND,
//	const WNDPROC& winProc
//)
//{
//	w.cbSize = sizeof(WNDCLASSEX);
//	w.lpfnWndProc = (WNDPROC)winProc;
//	w.lpszClassName = windowName.c_str();
//	w.hInstance = GetModuleHandle(nullptr);
//	w.hCursor = LoadCursor(NULL, IDC_ARROW);
//
//	RegisterClassEx(&w);
//	RECT wrc = { 0,0,width,height };
//	AdjustWindowRect(&wrc, windowStyle, false);
//
//	HWND hwnd;
//	hwnd = CreateDirectoryEx
//	(
//		w.lpszClassName,
//		windowName.c_str(),
//		windowStyle,
//		CW_USEDEFAULT,
//		CW_USEDEFAULT,
//		wrc.right - wrc.left,
//		wrc.bottom - wrc.top,
//		parentHWND,
//		nullptr,
//		w.hInstance,
//		nullptr);
//
//	ShowWindow(hwnd, SW_SHOW);
//
//	return hwnd;
//}


bool LibWinAPI::CreateSaveWindow
(
	const HWND& hwnd, 
	std::wstring& fileName,
	const std::wstring & defaultFormat
)
{
	OPENFILENAMEW save;
	//������Ԃ̂Ƃ��̕\��
	fileName.resize(400);

	save.lStructSize = sizeof(OPENFILENAME);
	save.hwndOwner = hwnd;//������HWND���Z�b�g����ƃ_�C�A���O�̃n���h�����i�[�����?
	save.hInstance = GetModuleHandle(nullptr);
	save.lpstrFilter = nullptr;
	save.lpstrCustomFilter = nullptr;
	save.nMaxCustFilter = 0;
	save.nFilterIndex = 1;
	save.lpstrFile = (LPTSTR)fileName.data();//�����ɓ��͂������̂��ۑ������
	save.nMaxFile = fileName.size();
	save.lpstrFileTitle = NULL;//NULL�ɂ���ƁA�p�X��C:����n�܂�
	save.nMaxFileTitle = 0;
	save.lpstrInitialDir = (LPCWSTR)L"C:";//�E�B���h�E�J���čŏ��ɕ\������ꏊ?�Ⴄ����
	save.lpstrTitle = NULL;
	save.Flags = OFN_HIDEREADONLY;
	save.nFileOffset = 0;
	save.nFileExtension = 8;
	save.lpstrDefExt = (LPWSTR)defaultFormat.c_str();//�`������͂��Ȃ������ꍇ�A���̌`���ɂ��邩
	//saveName.lCustData = ram;

	bool result = GetSaveFileName(&save);//�E�B���h�E�n���h���擾���ăv���V�[�W���Ń��b�Z�[�W�擾���āA�j��������GamePlay��isSave��false�ɕς���悤�ɂ���

	if (!result)return result;
	int strCount = 0;
	for(auto& wStr : fileName)
	{
		if(wStr == '\0')
		{
			fileName.erase(fileName.begin() + strCount, fileName.end());
			break;
		}
		strCount++;
	}
	return result;

}

bool LibWinAPI::CreateLoadWindow
(
	const HWND& hwnd,
	std::wstring& fileName,
	const std::wstring& defaultFormat
)
{
	OPENFILENAMEW load;
	//������Ԃ̂Ƃ��̕\��
	fileName.resize(400);

	load.lStructSize = sizeof(OPENFILENAME);
	load.hwndOwner = hwnd;//������HWND���Z�b�g����ƃ_�C�A���O�̃n���h�����i�[�����?
	load.hInstance = GetModuleHandle(nullptr);
	load.lpstrFilter = nullptr;
	load.lpstrCustomFilter = nullptr;
	load.nMaxCustFilter = 0;
	load.nFilterIndex = 1;
	load.lpstrFile = (LPTSTR)fileName.data();//�����ɓ��͂������̂��ۑ������
	load.nMaxFile = fileName.size();
	load.lpstrFileTitle = NULL;//NULL�ɂ���ƁA�p�X��C:����n�܂�
	load.nMaxFileTitle = 0;
	load.lpstrInitialDir = (LPCWSTR)L"C:";//�E�B���h�E�J���čŏ��ɕ\������ꏊ?�Ⴄ����
	load.lpstrTitle = NULL;
	load.Flags = OFN_HIDEREADONLY;
	load.nFileOffset = 0;
	load.nFileExtension = 8;
	load.lpstrDefExt = (LPWSTR)defaultFormat.c_str();//�`������͂��Ȃ������ꍇ�A���̌`���ɂ��邩
	//saveName.lCustData = ram;

	bool result = GetOpenFileName(&load);//�E�B���h�E�n���h���擾���ăv���V�[�W���Ń��b�Z�[�W�擾���āA�j��������GamePlay��isSave��false�ɕς���悤�ɂ���

	if (!result)return result;
	int strCount = 0;
	for (auto& wStr : fileName)
	{
		if (wStr == '\0')
		{
			fileName.erase(fileName.begin() + strCount, fileName.end());
			break;
		}
		strCount++;
	}

	return result;
}
