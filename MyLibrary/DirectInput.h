//#pragma once
//
//#define DIRECTINPUT_VERSION 0x0800
//#include<dinput.h>
//#pragma comment(lib,"dinput8.lib")
//#pragma comment(lib,"dxguid.lib")
//#include<Windows.h>
//#include<vector>
//#include"Vector.h"
//
//#include<DirectXMath.h>
//
//
//#pragma region �p�b�h�̃{�^��
////����O���[�o���萔�ɂ���?
//const int XButton = 0;
//const int YButton = 1;
//const int AButton = 2;
//const int BButton = 3;
//const int LBButton = 4;
//const int RBButton = 5;
//const int LTButton = 6;
//const int RTButton = 7;
//const int StartButton = 11;
//#pragma endregion
//#pragma region �\���L�[
////enum�ɕς���
//const int UpButton = 0;
//const int RightButton = 1;
//const int DownButton = 2;
//const int LeftButton = 3;
//#pragma endregion
//
//#pragma region �}�E�X�{�^��
//
//enum MouseButton
//{
//	MOUSE_BUTTON_LEFT,
//	MOUSE_BUTTON_RIGHT,
//	MOUSE_BUTTON_CENTER,
//	//MOUSE_BUTTON_SIDE_DOWN,
//
//};
//#pragma endregion
//
//
//class DirectInput
//{
//private:
//	DirectInput(){}
//	~DirectInput(){}
//
//	//�񋓗p�R�[���o�b�N
//	static BOOL CALLBACK diEnumDevicesCallBack(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef);
//	static BOOL CALLBACK diEnumDevicesObjectCallBack(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef);
//
//	static HINSTANCE hInstance;
//	static HRESULT dIResult;//���ʎ擾
//	static LPDIRECTINPUT8 dInput8;//dxInput������ϐ�
//	static LPDIRECTINPUTDEVICE8 keyDev;//�f�o�C�X������ϐ�
//
//	static IDirectInput8* dInputDevice;
//	static IDirectInputDevice8* devKeyBoard;
//	static IDirectInputDevice8* devMouse;
//	static IDirectInputDevice8* devPad;
//
//	//static IDirectInputDevice8* padSetUp;
//	//static DIDEVICEOBJECTDATA* lpdidod;
//	static std::vector<DIDEVICEINSTANCE>getEnum;
//	static DIDEVCAPS padCaps;
//
//
//	//�p�b�h
//	static DIJOYSTATE padState;//�p�b�h���
//	static DIJOYSTATE padPrevious;//�p�b�h���(1���[�v�O)
//
//
//	//���݂̃L�[���
//	static BYTE key[256];
//	//1�t���[���O�̃L�[���
//	static BYTE preKey[256];
//
//	//�}�E�X
//	static DIMOUSESTATE mouseState;
//	static DIMOUSESTATE mousePrevious;
//
//
//	//�擾�����E�B���h�E�n���h��
//	static HWND mHwnd;
//
//	//�p�b�h��F���������ǂ���
//	static bool setPad;
//
//	//�E�B���h�E�T�C�Y
//	static int winWidth;
//	static int winHeight;
//
//	//�s��
//	static DirectX::XMMATRIX viewMat;
//	static DirectX::XMMATRIX projectionMat;
//
//	//�J����
//	static float nearNum;
//	static float farNum;
//
//public:
//
//
//	//����
//	static void Initialize(HWND hwnd,int windowWidth,int windowHeight);
//	//���擾
//	static void Update();
//
//
//	//DInput�̊J��
//	static void Release();
//
//#pragma region �L�[�{�[�h
//
//	//�����Ă��邩
//	static bool KeyState(const BYTE& keyDef);
//	//�������u�Ԃ�
//	static bool KeyTrigger(const BYTE& keyDef);
//	//�������u�Ԃ�
//	static bool KeyRelease(const BYTE& keyDef);
//
//	/// <summary>
//	/// �L�[�̕�����Ԃ��܂�
//	/// </summary>
//	/// <returns></returns>
//	static char KeyChar();
//
//	/// <summary>
//	/// �A���[�L�[�����x�������Ă��邩���p�x�ŕԂ��܂��B	
//	/// </summary>
//	/// <returns></returns>
//	static float ArrowKeyAngle();
//#pragma endregion
//
//#pragma region �p�b�h
//#pragma region �ʏ�{�^��
//
//	//�����Ă��邩
//	static bool ButtonState(int button);
//	//�������u�Ԃ�
//	static bool ButtonTrigger(int button);
//	//�������u�Ԃ�
//	static bool ButtonRelease(int button);
//#pragma endregion
//
//#pragma region �\���L�[
//
//	static bool DirectionalButtonState(int button);
//	static bool DirectionalButtonTrigger(int button);
//#pragma endregion
//
//#pragma endregion
//
//#pragma region �X�e�B�b�N
//	/// <summary>
//	/// 
//	/// </summary>
//	/// <param name="lNum">��(32767���ō��l)</param>
//	/// <returns></returns>
//	static bool LeftStickLeft(int lXNum);
//	static bool LeftStickRight(int lXNum);
//	static bool LeftStickUp(int lYNum);
//	static bool LeftStickDown(int lYNum);
//	static bool RightStickLeft(int lXNum);
//	static bool RightStickRight(int lXNum);
//	static bool RightStickUp(int lYNum);
//	static bool RightStickDown(int lYNum);
//
//	
//#pragma endregion
//
//#pragma region �}�E�X
//
//#pragma region �J�[�\���֌W
//
//	static void SetMatrixAndNearFar(DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix,float nearNumber ,float farNumber);
//
//	static void SetViewMatrix(const DirectX::XMMATRIX& viewMatrix) { viewMat = viewMatrix; }
//	static void SetProjectionMatrix(const DirectX::XMMATRIX& projectionMatrix) { projectionMat = projectionMatrix; }
//	static void SetNear(const float nearNumber) { nearNum = nearNumber; }
//	static void SetFar(const float farNumber) { farNum = farNumber; }
//
//	/// <summary>
//	/// �N���C�A���g���W�ɕϊ����A�ŏ��ɐݒ肵���E�B���h�E�T�C�Y�ɉ����ĕ␳�����}�E�X�̍��W���擾���܂�
//	/// </summary>
//	/// <returns></returns>
//	static Vector2 GetMousePosition();
//
//	/// <summary>
//	/// �E�B���h�E�̒��S����Ƃ����J�[�\���̃x�N�g�����擾���܂��B
//	/// </summary>
//	/// <returns>�x�N�g��</returns>
//	static Vector2 GetCenterToMouseVector();
//
//	/// <summary>
//	/// �E�B���h�E�̒��S����Ƃ����J�[�\���̊p�x���擾���܂��B
//	/// </summary>
//	/// <returns>�p�x</returns>
//	static float GetMouseAngle();
//#pragma endregion
//
//
//	/// <summary>
//	/// �}�E�X�J�[�\������o�Ă�������̍ŋߓ_�ƍŉ��_���擾���܂�
//	/// </summary>
//	/// <param name="nearPoint"></param>
//	/// <param name="farPoint"></param>
//	static void GetMouse3DLine(Vector3& nearPoint,Vector3& farPoint);
//
//	static bool MouseButtonState(MouseButton mouseButton);
//
//	static bool MouseButtonTrigger(MouseButton mouseButton);
//	
//	static bool MouseButtonRelease(MouseButton mouseButton);
//
//#pragma endregion
//
//
//
//};
//
//
//
