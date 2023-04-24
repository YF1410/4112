//#pragma once
//#include<Windows.h>
//#include<Xinput.h>
//#pragma comment(lib,"Xinput9_1_0.lib")
//
//
//enum class XInputButton
//{
//	UP_BUTTON = 0x0001,//�\���L�[��
//	DOWN_BUTTON = 0x0002,//�\���L�[��
//	LEFT_BUTTON = 0x0004,//�\���L�[��
//	RIGHT_BUTTON = 0x0008,//�\���L�[�E
//	START_BUTTON = 0x00010,
//	BACK_BUTTON = 0x00020,
//	A_BUTTON = 0x1000,//PS�́~ Switch��B
//	B_BUTTON = 0x2000,//PS�́� Switch��A
//	X_BUTTON = 0x4000,//PS�́� Switch��Y
//	Y_BUTTON = 0x8000,//PS�́� Switch��X
//	R_STICK_BUTTON = 0x0040,//PS��R3
//	L_STICK_BUTTON = 0x0080,//PS��L3
//	RB_BUTTON = 0x0200,//PS��R1 Switch��R
//	LB_BUTTON = 0x0100,//PS��L1 Switch��L
//};
//
//class XInputManager
//{
//private:
//	XInputManager(){}
//	~XInputManager(){}
//
//	static XINPUT_STATE padState[4];
//	static XINPUT_STATE padPrevious[4];
//
//	static bool padConnected[4];
//
//	/// <summary>
//	/// �p�b�h�ԍ���3�𒴂��Ă��Ȃ����ƁA�ڑ�����Ă��邩���m�F���܂��B
//	/// </summary>
//	/// <param name="padNum"></param>
//	/// <returns></returns>
//	static bool PadCheck(const UCHAR& padNum);
//
//	//���̍ō��l
//    static const short MAX_AXIS_VALUE = 32767;
//	//�U���̍ō��l
//	static const WORD MAX_VIBRATION_VALUE = 65535;
//
//public:
//	static void Update();
//
//	static bool GetPadConnectedFlag(const UCHAR& padNum);
//
//#pragma region �{�^��
//	static bool ButtonState(const XInputButton& button,const UCHAR& padNum);
//	static bool ButtonTrigger(const XInputButton& button,const UCHAR& padNum);
//	static bool ButtonRelease(const XInputButton& button,const UCHAR& padNum);
//
//	/// <summary>
//	/// �\���{�^���������p�x���擾���܂��B
//	/// </summary>
//	/// <returns></returns>
//	static float DirectionalButtonAngle(const UCHAR& padNum);
//#pragma endregion
//
//#pragma region �A�i���O�X�e�B�b�N
//	static bool LeftStickLeft(const float& lXPar , const UCHAR& padNum);
//	static bool LeftStickRight(const float&  lXPar, const UCHAR& padNum);
//	static bool LeftStickUp(const float&  lYPar, const UCHAR& padNum);
//	static bool LeftStickDown(const float&  lYPar, const UCHAR& padNum);
//	static bool RightStickLeft(const float&  lXPar, const UCHAR& padNum);
//	static bool RightStickRight(const float&  lXPar, const UCHAR& padNum);
//	static bool RightStickUp(const float&  lYPar, const UCHAR& padNum);
//	static bool RightStickDown(const float&  lYPar, const UCHAR& padNum);
//
//	/// <summary>
//	/// ���X�e�B�b�N�����x�����p�x�ŕԂ��܂��B�X�e�B�b�N���X���Ă��Ȃ��ꍇ�A-1��Ԃ��܂��B
//	/// </summary>
//	/// <returns></returns>
//	static float LeftStickAngle(const UCHAR& padNum);
//
//	/// <summary>
//	/// �E�X�e�B�b�N�����x�����p�x�ŕԂ��܂��B�X�e�B�b�N���X���Ă��Ȃ��ꍇ�A-1��Ԃ��܂��B
//	/// </summary>
//	/// <returns></returns>
//	static float RightStickAngle(const UCHAR& padNum);
//#pragma endregion
//
//#pragma region �U��
//	/// <summary>
//	/// �Q�[���p�b�h��U�������܂�
//	/// </summary>
//	/// <param name="leftPar">�����̐U����(0%�`100%)</param>
//	/// <param name="rightPar">�E���̐U����(0%�`100%)</param>
//	/// <param name="padNum">�p�b�h�ԍ�(1P��2P�̂���)</param>
//	static void PadVibration(const float& leftPar,const float& rightPar,const UCHAR& padNum);
//
//#pragma endregion
//
//};
//
