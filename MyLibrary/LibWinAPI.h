#pragma once
#include<Windows.h>
#include<string>
#include"Vector.h"

//L"EDIT"�Ƃ�����Ȃ��q�E�B���h�E�́A�e�̖��O�n���Ȃ��Ƃ����Ȃ�?

namespace MelLib
{
	//WinAPI���܂Ƃ߂�����
	class LibWinAPI
	{
	private:
		//static WNDCLASSEX w;


	public:

		/// <summary>
		/// �E�B���h�E���쐬���܂��B
		/// </summary>
		/// <param name="className">�E�B���h�E�N���X��</param>
		/// <param name="windowName">�E�B���h�E��(�{�^���Ȃǂ��쐬����ꍇ�A������Ă��閼�O)</param>
		/// <param name="windowStyle">�E�B���h�E�̃X�^�C��</param>
		/// <param name="posX">�c��</param>
		/// <param name="posY">�c��</param>
		/// <param name="windowWidth">�c��</param>
		/// <param name="windowHeighr">����</param>
		/// <param name="parentHWND">�e�E�B���h�E��HWND�B�e�E�B���h�E�𐶐�����ꍇ�́Anullptr��n���B</param>
		/// <param name="winProc">�E�B���h�E�v���V�[�W��</param>
		/// <returns></returns>
		static HWND CreateNormalWindow
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
		);

		static HWND CreateExpansionWindow
		(
			const std::wstring& className,
			const std::wstring& winName,
			const DWORD& expansionWindowStyle,
			const DWORD& windowStyle,
			const DWORD& posX,
			const DWORD& posY,
			const int& width,
			const int& height,
			const HWND parentHWND,
			const WNDPROC& winProc
		);

		//static HWND createExpansionWindow
		//(
		//	const std::wstring& windowName,
		//	const int& width,
		//	const int& heighr,
		//	const int& windowStyle,
		//	const HWND parentHWND,
		//	const WNDPROC& winProc
		//);

		/// <summary>
		/// �ۑ����邽�߂̃E�B���h�E�𐶐����A���͂����t�@�C�������擾���܂�
		/// </summary>
		/// <param name="hwnd">�E�B���h�E�n���h��</param>
		/// <param name="fileName">�t�@�C��������͂��镔���̏��������L�q����Btrue���Ԃ��ꂽ�ꍇ�A���͂����t�@�C�������i�[�����B</param>
		/// <param name="defaultFormat">�t�H�[�}�b�g���w�肵�Ȃ������ꍇ�̃t�H�[�}�b�g��</param>
		/// <returns></returns>
		static bool CreateSaveWindow
		(
			const HWND& hwnd,
			std::wstring& fileName,
			const std::wstring& defaultFormat
		);

		/// <summary>
		///	�J�����߂̃E�B���h�E�𐶐����A���͂����t�@�C�������擾���܂�
		/// </summary>
		/// <param name="hwnd">�E�B���h�E�n���h��</param>
		/// <param name="fileName">�t�@�C��������͂��镔���̏��������L�q����Btrue���Ԃ��ꂽ�ꍇ�A���͂����t�@�C�������i�[�����B</param>
		/// <param name="defaultFormat">�t�H�[�}�b�g���w�肵�Ȃ������ꍇ�̃t�H�[�}�b�g��</param>
		/// <returns></returns>
		static bool CreateLoadWindow
		(
			const HWND& hwnd,
			std::wstring& fileName,
			const std::wstring& defaultFormat
		);

	};
}
