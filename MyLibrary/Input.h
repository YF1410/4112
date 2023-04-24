#pragma once

#include<Windows.h>
#include<Xinput.h>
#pragma comment(lib,"Xinput9_1_0.lib")


#define DIRECTINPUT_VERSION 0x0800
#include<dinput.h>
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

#include<DirectXMath.h>
#include<unordered_map>

#include"Vector.h"
#include"Camera.h"

namespace MelLib
{
	// �Q�[���p�b�h�̃{�^���̗�
	enum class PadButton
	{
		UP = 0x0001,//�\���L�[��
		DOWN = 0x0002,//�\���L�[��
		LEFT = 0x0004,//�\���L�[��
		RIGHT = 0x0008,//�\���L�[�E
		START = 0x00010,//�X�^�[�g�{�^�� PS4�ȍ~��OPTION
		BACK = 0x00020,//�o�b�N�{�^�� PS(1�`3)�ASwitch��Start PS4�ȍ~��SHARE
		A = 0x1000,//PS�́~ Switch��B
		B = 0x2000,//PS�́� Switch��A
		X = 0x4000,//PS�́� Switch��Y
		Y = 0x8000,//PS�́� Switch��X
		L_STICK = 0x0040,//PS��L3
		R_STICK = 0x0080,//PS��R3
		RB = 0x0200,//PS��R1 Switch��R
		LB = 0x0100,//PS��L1 Switch��L
	};

	enum class MouseButton
	{
		LEFT,
		RIGHT,
		CENTER,//�z�C�[����������
	};

	// Input ���t�@�����X
	// https://sites.google.com/view/melgames/%E8%87%AA%E4%BD%9C%E3%83%A9%E3%82%A4%E3%83%96%E3%83%A9%E3%83%AA/%E3%83%AA%E3%83%95%E3%82%A1%E3%83%AC%E3%83%B3%E3%82%B9/%E4%BD%BF%E7%94%A8%E4%BB%BB%E6%84%8F%E3%82%AF%E3%83%A9%E3%82%B9/%E5%85%A5%E5%8A%9B

	// �����G���A
	// GetPressKeyChars�g�킸�ɁAWinAPI�̃G�f�B�b�g�{�b�N�X�ŕ����擾����悤�ɂ���?
	// �G�f�B�b�g�{�b�N�X�𓧖��ɂł��邩����
	// ���������q�E�B���h�E����������Q�[����ʂɕ\���ł��Ȃ�?�ł���?
	// �ł��Ȃ�����
	// ���������ĉB��ĂĂ����͂͂ł���?�E�B���h�E�^�C�v�Őݒ肷��΂�����

	// �f�o�C�X���ƂɃN���X�������Ă����������B
	// Input�͊e�f�o�C�X�̏����̌Ăяo��������N���X�ɂ���

	// ���̓N���X
	class Input
	{

	private:
		Input() {}
		~Input() {}

#pragma region DirectInput


		static HINSTANCE hInstance;
		static HRESULT dIResult;//���ʎ擾
		static LPDIRECTINPUT8 dInput8;//dxInput������ϐ�
		static LPDIRECTINPUTDEVICE8 keyDev;//�f�o�C�X������ϐ�

		static IDirectInput8* dInputDevice;
		static IDirectInputDevice8* devKeyBoard;
		static IDirectInputDevice8* devMouse;

		//���݂̃L�[���
		static BYTE keysState[256];

		//1�t���[���O�̃L�[���
		static BYTE preKeysState[256];

		//���݉�����Ă���L�[
		static std::vector<BYTE> currentPressKeys;

		//GetPressKeyChars()���當������擾�������ǂ���
		static bool callGetPressKeyChars;
		//�O�t���[����GetPressKeyChars()���當������擾�������ǂ���
		static bool callPreGetPressKeyChars;


		static std::unordered_map<std::string, HWND>hwnds;
		//�L�[�����������ɓ��͕�����Ԃ����߂̔z��
		static std::vector<std::unordered_map<UCHAR, char>>returnChars;

		//�p�b�h�ł̓��͂Ɏg���邩���Ǝv���A�p�ӂ�������
		static const std::vector<wchar_t>hiragana;
		static const std::vector<wchar_t>katakana;


		//�}�E�X
		static DIMOUSESTATE mouseState;
		static DIMOUSESTATE mousePrevious;

		//�擾�����E�B���h�E�n���h��
		static HWND mHwnd;

		static LRESULT ParamChildWindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
#pragma endregion

#pragma region XInput


		static XINPUT_STATE padState[4];
		static XINPUT_STATE padPrevious[4];


		static bool padConnected[4];

		//���̍ō��l
		static const short MAX_AXIS_VALUE = 32767;
		//�U���̍ō��l
		static const WORD MAX_VIBRATION_VALUE = 65535;
#pragma endregion

		//�s��
		static DirectX::XMMATRIX viewMat;
		static DirectX::XMMATRIX projectionMat;

		//�J����
		static float nearNum;
		static float farNum;


		//�E�B���h�E�T�C�Y
		static int winWidth;
		static int winHeight;

		/// <summary>
		/// �p�b�h�ԍ���3�𒴂��Ă��Ȃ����ƁA�ڑ�����Ă��邩���m�F���܂��B
		/// </summary>
		/// <param name="padNum"></param>
		/// <returns></returns>
		static bool PadCheck(UCHAR padNum);


	public:
		static void Initialize(HWND hwnd, int windowWidth, int windowHeight);
		static void Update();
		static void Finalize();

		/// <summary>
		/// �w�肵���p�b�h���ڑ�����Ă��邩���m�F���܂��B
		/// </summary>
		/// <param name="padNum"></param>
		/// <returns></returns>
		static bool GetPadConnectedFlag(UCHAR padNum);


#pragma region �L�[�{�[�h

		/// <summary>
		/// �L�[��������Ă�����true��Ԃ��܂��B
		/// </summary>
		/// <param name="keyDef"></param>
		/// <returns></returns>
		static bool KeyState(BYTE keyDef);

		/// <summary>
		/// �L�[�������ꂽ�u��(�����ꂽ�t���[��)��������true��Ԃ��܂��B
		/// </summary>
		/// <param name="keyDef"></param>
		/// <returns></returns>
		static bool KeyTrigger(BYTE keyDef);

		/// <summary>
		/// �L�[�������̂���߂��u��(��߂��t���[��)��������true��Ԃ��܂��B
		/// </summary>
		/// <param name="keyDef"></param>
		/// <returns></returns>
		static bool KeyRelease(BYTE keyDef);


#pragma region �����擾
		//GetPressKeyChars�Ƃ��͋C�y�ɕ����擾�ł��邵�A
		//�^�C�s���O�Q�[���݂����ɘg�p�ӂ���K�v�Ȃ��Ƃ���A�K�v�Ȃ��Ƃ��Ɏg���邩��c���Ƃ��Ă���

		/// <summary>
		/// ���݉�����Ă���L�[�̕�����Ԃ��܂��B
		/// </summary>
		/// <returns></returns>
		static std::string GetPressKeyChars();

		/// <summary>
		/// �����ꂽ�u�ԂɃL�[�̕�����Ԃ��܂��B
		/// </summary>
		/// <returns></returns>
		static std::string GetTriggerKeyChars();


		static void CreateStringInputWindow
		(
			const std::wstring& initStr,
			const Vector2& position,
			const Vector2& size,
			HWND parentHWND,
			const std::string& name
		);

		static void DeleteStringInputWindow(const std::string& name);

		static std::wstring GetInputString(const std::string& name);

#pragma endregion
		/// <summary>
		/// ���݉�����Ă���L�[���擾���܂��B
		/// </summary>
		/// <returns></returns>
		static const std::vector<BYTE>& GetStateKeys() { return currentPressKeys; }

		/// <summary>
		/// �A���[�L�[�����x�������Ă��邩���p�x�ŕԂ��܂��B	
		/// </summary>
		/// <returns></returns>
		static float ArrowKeyAngle();
#pragma endregion

#pragma region �}�E�X


#pragma region �J�[�\��


		static void SetViewMatrix(const DirectX::XMMATRIX& viewMatrix) { viewMat = viewMatrix; }
		static void SetProjectionMatrix(const DirectX::XMMATRIX& projectionMatrix) { projectionMat = projectionMatrix; }
		static void SetNear(float nearNumber) { nearNum = nearNumber; }
		static void SetFar(float farNumber) { farNum = farNumber; }

		/// <summary>
		/// �N���C�A���g���W�ɕϊ����A�ŏ��ɐݒ肵���E�B���h�E�T�C�Y�ɉ����ĕ␳�����}�E�X�̍��W���擾���܂�
		/// </summary>
		/// <returns></returns>
		static Vector2 GetMousePosition();

		/// <summary>
		/// �E�B���h�E�̒��S����Ƃ����J�[�\���̃x�N�g�����擾���܂��B
		/// </summary>
		/// <returns>�x�N�g��</returns>
		static Vector2 GetCenterToMouseVector();

		/// <summary>
		/// �E�B���h�E�̒��S����Ƃ����J�[�\���̊p�x���擾���܂��B
		/// </summary>
		/// <returns>�p�x</returns>
		static float GetMouseAngle();


		/// <summary>
		/// �}�E�X�J�[�\������o�Ă�������̍ŋߓ_�ƍŉ��_���擾���܂�
		/// </summary>
		/// <param name="nearPoint"></param>
		/// <param name="farPoint"></param>
		static void GetMouse3DLine(Vector3& nearPoint, Vector3& farPoint);

		/// <summary>
		/// �J�����̏������ɁA�}�E�X�J�[�\�������W�ϊ������Ƃ��̍ŋߓ_�ƍŉ��_�����߂܂��B
		/// </summary>
		/// <param name="pCamera"></param>
		/// <param name="pNear"></param>
		/// <param name="pFar"></param>
		static void GetMouse3DLine2
		(
			Camera* pCamera,
			Vector3* pNear,
			Vector3* pFar
		);
#pragma endregion

#pragma region �{�^��


		/// <summary>
		/// �}�E�X�̃{�^�����擾���܂��B
		/// </summary>
		/// <param name="mouseButton"></param>
		/// <returns></returns>
		static bool MouseButtonState(MouseButton mouseButton);

		/// <summary>
		/// �}�E�X�̃{�^���������ꂽ�u��(�����ꂽ�t���[��)���ǂ������擾���܂��B
		/// </summary>
		/// <param name="mouseButton"></param>
		/// <returns></returns>
		static bool MouseButtonTrigger(MouseButton mouseButton);

		/// <summary>
		/// �}�E�X�̃{�^���������ꂽ�u��(�����̂��~�߂��t���[��)���ǂ������擾���܂��B
		/// </summary>
		/// <param name="mouseButton"></param>
		/// <returns></returns>
		static bool MouseButtonRelease(MouseButton mouseButton);


#pragma endregion

#pragma region �z�C�[��
		static LONG GetMouseWheelValue() { return mouseState.lZ; }

#pragma endregion


#pragma endregion

#pragma region �Q�[���p�b�h


#pragma region �{�^��
		static bool PadButtonState(PadButton button, UCHAR padNum = 1);
		static bool PadButtonTrigger(PadButton button, UCHAR padNum = 1);
		static bool PadButtonRelease(PadButton button, UCHAR padNum = 1);

		/// <summary>
		/// �\���{�^���������p�x���擾���܂��B
		/// </summary>
		/// <returns></returns>
		static float DirectionalButtonAngle(UCHAR padNum = 1);
#pragma endregion

#pragma region �A�i���O�X�e�B�b�N

#pragma region ��

#pragma region ���

		static bool LeftStickLeft(float lXPar, UCHAR padNum = 1);
		static bool LeftStickRight(float lXPar, UCHAR padNum = 1);
		static bool LeftStickUp(float lYPar, UCHAR padNum = 1);
		static bool LeftStickDown(float lYPar, UCHAR padNum = 1);

#pragma endregion

#pragma region �g���K�[

		/// <summary>
		/// �O�t���[���̃X�e�B�b�N�̌X���̊�����lXPar�ȉ��A���݂̃t���[���̌X����lXPar�ȏゾ������true��Ԃ��܂��B
		/// </summary>
		/// <param name="lXPar">true��Ԃ��</param>
		/// <param name="padNum">�p�b�h�ԍ�</param>
		/// <returns></returns>
		static bool LeftStickLeftTrigger(float lXPar, UCHAR padNum = 1);

		static bool LeftStickRightTrigger(float lXPar, UCHAR padNum = 1);

		static bool LeftStickUpTrigger(float lXPar, UCHAR padNum = 1);

		static bool LeftStickDownTrigger(float lXPar, UCHAR padNum = 1);


#pragma endregion


		/// <summary>
		/// ���X�e�B�b�N�̊p�x��Ԃ��܂��B�X�e�B�b�N���X���Ă��Ȃ��ꍇ�A-1��Ԃ��܂��B
		/// </summary>
		/// <param name="par">���p�[�Z���g�ȏ�X��������͂��ꂽ���Ƃɂ��邩</param>
		/// <param name="padNum">�R���g���[���[�ԍ�(1P��2P�Ȃǂ̔ԍ��̂���)</param>
		/// <returns></returns>
		static float LeftStickAngle(float par, UCHAR padNum = 1);

		/// <summary>
		/// �X�e�B�b�N���X���Ă��������Vector2�ŕԂ��܂��B
		/// </summary>
		/// <param name="par">���p�[�Z���g�ȏ�X��������͂��ꂽ���Ƃɂ��邩</param>
		/// <param name="dimention3D">Y����3�����Ɠ���(���+)�ɂ��邩�ǂ���</param>
		/// <param name="padNum">�R���g���[���[�ԍ�(1P��2P�Ȃǂ̔ԍ��̂���)</param>
		/// <returns></returns>
		static Vector2 LeftStickVector2(float par, bool dimention3D, UCHAR padNum = 1);

		/// <summary>
		/// �X�e�B�b�N���X���Ă��������Vector3�ŕԂ��܂��B���������͂����ꍇ�A�J�����̌����Ă��������Ԃ��܂��B
		/// </summary>
		/// <param name="par">���p�[�Z���g�ȏ�X��������͂��ꂽ���Ƃɂ��邩</param>
		/// <param name="pCamera"></param>
		/// <param name="rotX"></param>
		/// <param name="rotY"></param>
		/// <param name="padNum"></param>
		/// <returns></returns>
		static Vector3 LeftStickVector3(float par, Camera* pCamera = nullptr, bool rotX = false, bool rotY = false, UCHAR padNum = 1);
#pragma endregion

#pragma region �E

#pragma region ���

		static bool RightStickLeft(float lXPar, UCHAR padNum = 1);
		static bool RightStickRight(float lXPar, UCHAR padNum = 1);
		static bool RightStickUp(float lYPar, UCHAR padNum = 1);
		static bool RightStickDown(float lYPar, UCHAR padNum = 1);
#pragma endregion

#pragma region �g���K�[
		static bool RightStickLeftTrigger(float lXPar, UCHAR padNum = 1);

		static bool RightStickRightTrigger(float lXPar, UCHAR padNum = 1);

		static bool RightStickUpTrigger(float lYPar, UCHAR padNum = 1);

		static bool RightStickDownTrigger(float lYPar, UCHAR padNum = 1);
#pragma endregion



		/// <summary>
		/// �E�X�e�B�b�N�����x�����p�x�ŕԂ��܂��B�X�e�B�b�N���X���Ă��Ȃ��ꍇ�A-1��Ԃ��܂��B
		/// </summary>
		/// <returns></returns>
		static float RightStickAngle(float par, UCHAR padNum = 1);


		static Vector2 RightStickVector2(float par, bool dimention3D, UCHAR padNum = 1);
		static Vector3 RightStickVector3(float par, Camera* pCamera = nullptr, bool rotX = false, bool rotY = false, UCHAR padNum = 1);
#pragma endregion


#pragma endregion

#pragma region �U��
		/// <summary>
		/// �Q�[���p�b�h��U�������܂�
		/// </summary>
		/// <param name="leftPar">�����̐U����(0%�`100%)</param>
		/// <param name="rightPar">�E���̐U����(0%�`100%)</param>
		/// <param name="padNum">�p�b�h�ԍ�(1P��2P�̂���)</param>
		static void PadVibration(float leftPar, float rightPar, UCHAR padNum = 1);

#pragma endregion
#pragma endregion

	};
}