#pragma once
#include<Windows.h>
#include<ctime>
#include<random>
#include"Vector.h"
#include"Color.h"
#include"DirectX12.h"
#include"DirectXStruct.h"
#include"CreatePolygon.h"


namespace MelLib
{

	class Library
	{
	private:
		Library() {}
		~Library() {}

		static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

		static DirectX12* dx12;
		//static CreatePolygon* createPolygon;

		static UINT count;
		static int createPointCount;


		static WNDCLASSEX w;
		static MSG msg;
		static HWND hwnd;
		static bool isEnd;


		static int loadFontTextureCounter;
		static int loadTextureCounter;
		static int createSpriteConter;

		static int createPipelineCounter;//���݂̃p�C�v���C��������
		static int createPostEffectPuiperineCounter;//

		static HDC hdc;//dc�̃n���h��

		static bool isSetFPS60;
		static UINT fps;//���ݐݒ肳��Ă���fps
		static int refReat;//���t���b�V�����[�g
		static DWORD startProsessTime;//�v���J�n���̎���
		static DWORD nowTime;//���݂̎���

		//�`��p(����Ƀ��f���f�[�^����āA�J�������牓�����ɕ��ёւ��āA������g��for�ŉ񂵂ĕ`��R�}���h���Z�b�g���邱�ƂŁA
		//�֐��̌Ăяo�����ɂ�����炸�A���u�����h���K�؂ɍs�����Ƃ��ł���)
		static std::vector<std::tuple<ModelDataAndKey, int>>modelDatas;

#pragma region �������l
		static int winWidth;
		static int winHeight;
		static Color clearColor;
#pragma endregion
		static std::wstring winName;
	public:


#pragma region �������Ȃǂ̏���

		/// <summary>
		/// ���C�u�����̏��������s���܂��B�ǂ̃��C�u�����̊֐�������ɌĂяo���Ă�������
		/// </summary>
		/// <param name="windowWidth">�E�B���h�E����</param>
		/// <param name="windowHeight">�E�B���h�E�c��</param>
		static void Initialize(int windowWidth, int windowHeight, const Color& screenColor, const wchar_t* winName);

		/// <summary>
		/// �`�揀�������܂��B�X�V���������̈�ԏ�ŌĂяo���Ă�������
		/// </summary>
		static void LoopStartProcess();

		/// <summary>
		/// �`�悵�܂��B�`�揈���̈�ԍŌ�̌Ăяo���Ă�������
		/// </summary>
		static void LoopEndProcess();

		/// <summary>
		/// ���C�u�����̏I���t���O��true�ɂ��܂�
		/// </summary>
		static void EndFlagTrue();

		/// <summary>
		/// �I���t���O���擾���܂��B�I���t���O�́A�E�B���h�E���j�������(�~���������)���AEndFlagTrue��Ăяo����true�ɂȂ�܂��B
		/// </summary>
		/// <returns>�I���t���O</returns>
		static bool GetIsEnd();

		/// <summary>
		/// �I���������s���܂�
		/// </summary>
		static void Finalize();
#pragma endregion

#pragma region �������l�擾
		static int GetWindowWidth() { return winWidth; }
		static int GetWindowHeight() { return winHeight; }
		static Color GetScreenColor() { return clearColor; }
#pragma endregion

#pragma region �E�B���h�E�֌W
		static HWND GetHWND(){ return hwnd; }
		static std::wstring GetWindowName() { return winName; }
#pragma endregion




#pragma region �ݒ�

		/// <summary>
		/// FPS��60�ɌŒ肵�܂�
		/// </summary>
		/// <param name="framesPerSecond">�t���O</param>
		static void SetFramesPerSecond60(bool flag);



#pragma endregion


	};

}
