//#include "DirectInput.h"
//#include"LibMath.h"
//
//HINSTANCE DirectInput::hInstance;//�n���h��
//HRESULT DirectInput::dIResult;//���ʎ擾
//LPDIRECTINPUT8 DirectInput::dInput8;//dxInput������ϐ�
//LPDIRECTINPUTDEVICE8 DirectInput::keyDev;//�f�o�C�X������ϐ�
//
//
//IDirectInput8* DirectInput::dInputDevice;
//IDirectInputDevice8* DirectInput::devKeyBoard;
//IDirectInputDevice8* DirectInput::devMouse;
//IDirectInputDevice8* DirectInput::devPad;
//
////IDirectInputDevice8* DirectInput::padSetUp;
////DIDEVICEOBJECTDATA* DirectInput::lpdidod;
//
//DIDEVCAPS DirectInput::padCaps;
//
//std::vector<DIDEVICEINSTANCE> DirectInput::getEnum;
//
//BYTE DirectInput::key[256];
//BYTE DirectInput::preKey[256];
//DIMOUSESTATE DirectInput::mouseState;
//DIMOUSESTATE DirectInput::mousePrevious;
//DIJOYSTATE DirectInput::padState;
//DIJOYSTATE DirectInput::padPrevious;
//
//
//HWND DirectInput::mHwnd;
//bool DirectInput::setPad;
//
//int DirectInput::winHeight;
//int DirectInput::winWidth;
//
//DirectX::XMMATRIX DirectInput::viewMat;
//DirectX::XMMATRIX DirectInput::projectionMat;
//float DirectInput::nearNum;
//float DirectInput::farNum;
//
//void DirectInput::Initialize(HWND hwnd, int windowWidth, int windowHeight)
//{
//	mHwnd = hwnd;
//	winWidth = windowWidth;
//	winHeight = windowHeight;
//
//	hInstance = GetModuleHandle(nullptr);//�n���h��
//	//dInput8 = nullptr;//dxInput������ϐ�
//
//	////DrectInput�I�u�W�F�N�g�̐���
//	//dIResult = DirectInput8Create(
//	//	hInstance,
//	//	DIRECTINPUT_VERSION,
//	//	IID_IDirectInput8A,
//	//	(LPVOID*)&dInput8,
//	//	nullptr
//	//);
//
//	////�f�o�C�X�I�u�W�F�N�g�̐���
//
//	////�L�[�{�[�h�f�o�C�X�̍쐬
//	//dInput8->CreateDevice(
//	//	GUID_SysKeyboard, //�L�[�{�[�h�̓��͂��󂯎��Ƃ����ݒ�?
//	//	&keyDev, //���͏����󂯎�邽�߂̂���?
//	//	nullptr);
//
//	////�f�o�C�X�̃Z�b�g�A�b�v
//	//keyDev->SetCooperativeLevel(hwnd, DISCL_BACKGROUND);
//	//keyDev->SetDataFormat(&c_dfDIKeyboard);
//
//
//	////�f�o�C�X�̎擾
//	//dIResult = keyDev->Acquire();
//
//	//�f�o�C�X�쐬
//	dInputDevice = nullptr;
//	DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dInputDevice, nullptr);
//
//#pragma region �L�[�{�[�h
//
//	devKeyBoard = nullptr;
//	dInputDevice->CreateDevice(GUID_SysKeyboard, &devKeyBoard, NULL);
//	devKeyBoard->SetDataFormat(&c_dfDIKeyboard);
//	devKeyBoard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
//
//#pragma endregion
//
//#pragma region �}�E�X
//	devMouse = nullptr;
//	dInputDevice->CreateDevice(GUID_SysMouse, &devMouse, NULL);
//	devMouse->SetDataFormat(&c_dfDIMouse);
//	devMouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
//
//#pragma endregion
//
//	/*if (getEnum.size() == 0)
//	{
//		dInputDevice->EnumDevices(DI8DEVCLASS_GAMECTRL, &diEnumDevicesCallBack, NULL, DIEDFL_ATTACHEDONLY);
//
//		if (getEnum.size() != 0)
//		{
//
//			WCHAR* w = getEnum[0].tszInstanceName;
//			WCHAR wArr[260];
//			for (int i = 0; i < 260; i++)
//				wArr[i] = w[i];
//			char c[260];
//
//			size_t size;
//			wcstombs_s(&size, c, wArr, sizeof(c));
//			std::string enumName;
//
//			for (int i = 0; i < 40; i++)
//			{
//				enumName += c[i];
//			}
//			if (enumName.find("DirectInput") == std::string::npos)
//			{
//				getEnum.clear();
//				getEnum.shrink_to_fit();
//			}
//		}
//	}*/
//}
//
////�����EnumDevice�ɓn���Ƃ���ɐڑ��������̂�n���Ă����
////CALLBACK�����ĂȂ�����G���[�o�ēn���Ȃ�����
////CALLBACK���ď����Ȃ���������CAKKBACK�֐�����EnumDevices�֐����F�����Ȃ�����?
//
//BOOL CALLBACK DirectInput::diEnumDevicesCallBack(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef)
//{
//	if (lpddi != nullptr)
//	{
//		//vector���|�C���^�ɂ��ē�����Create�ł��Ȃ�?
//		getEnum.push_back(*lpddi);
//		return DIENUM_CONTINUE;
//	}
//
//	return DIENUM_STOP;
//}
//
//BOOL CALLBACK DirectInput::diEnumDevicesObjectCallBack(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef)
//{
//	return DIENUM_STOP;
//}
//
//void DirectInput::Update()
//{
//
//#pragma region �p�b�h
//	//BOOL(*calc)(DIDEVICEINSTANCE*,LPVOID);
//	//LPDIENUMDEVICESCALLBACK(enumInst,);//2�ڂɂ�EnumDevice��pvRef��n��
//	//LPVOID callV = nullptr;
//
//	//�Q�[���p�b�h�̐ݒ�ɂ����̂Ƀy���^�u�F������
//	//padPrevious = padState;
//
//	//
//	//
//	//
//	//if (getEnum.size() != 0 && !setPad)
//	//{
//	//	//�p�b�h�ڑ������GUID_Joystick��S_OK�߂��Ă���(����Ă������͂킩���)
//	//	//enum�̂�n���ꍇ�A�C���X�^���X�̂��n���΂������ۂ�?
//	//	auto ifresult = dInputDevice->CreateDevice(getEnum[0].guidInstance, &devPad, NULL);
//	//	padCaps.dwSize = sizeof(DIDEVCAPS);
//	//	ifresult = devPad->GetCapabilities(&padCaps);
//	//	ifresult = devPad->SetDataFormat(&c_dfDIJoystick);
//	//	ifresult = devPad->SetCooperativeLevel(mHwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
//	//	//result = devPad->EnumObjects();
//	//	//joyPadState = new DIJOYSTATE();
//	//
//	//	setPad = true;
//
//	//	
//	//}
//	//
//
//	//if (setPad) 
//	//{
//	//	//�A�N�Z�X�G���[
//	//	 auto ifresult = devPad->Acquire();
//
//	//	//�����G���[
//	//	ifresult = devPad->Poll();
//	//	ifresult = devPad->GetDeviceState(sizeof(padState), &padState);
//	//}
//	//
//#pragma endregion
//
//
//#pragma region �L�[�{�[�h
//
//	auto result = devKeyBoard->Acquire();
//	for (int i = 0; i < 256; i++)
//	{
//		preKey[i] = key[i];
//	}
//
//	//�L�[���擾
//	//dIResult = keyDev->GetDeviceState(256, key);
//	result = devKeyBoard->GetDeviceState(sizeof(key), key);
//
//#pragma endregion
//
//#pragma region �}�E�X
//	devMouse->Acquire();
//	mousePrevious = mouseState;
//	result = devMouse->GetDeviceState(sizeof(mouseState), &mouseState);
//#pragma endregion
//
//
//#ifdef _DEBUG
//	if (DirectInput::KeyTrigger(DIK_SPACE))
//	{
//		int a = 0;
//	}
//
//#endif // _DEBUG
//
//}
//
//#pragma region �L�[�{�[�h
//
//
//bool DirectInput::KeyState(const BYTE& keyDef)
//{
//	if (key[keyDef])
//	{
//		//�����Ă�Ƃ�
//		return true;
//	}
//	else
//	{
//		//�����ĂȂ��Ƃ�
//		return false;
//	}
//}
//
//bool DirectInput::KeyTrigger(const BYTE& keyDef)
//{
//	//1�t���[���O��true��false��
//	bool check;
//	if (preKey[keyDef])
//	{
//		check = true;
//	}
//	else
//	{
//		check = false;
//	}
//
//	if (KeyState(keyDef) && !check)
//	{
//		//�����ꂽ�u��(�O�t���[��������ĂȂ�&���t���[�������ꂽ)��
//		return true;
//	}
//
//	return false;
//}
//
//bool DirectInput::KeyRelease(const BYTE& keyDef)
//{
//	bool check;
//	if (preKey[keyDef])
//	{
//		check = true;
//	}
//	else
//	{
//		check = false;
//	}
//
//	if (!KeyState(keyDef) && check)
//	{
//		return true;
//	}
//
//	return false;
//}
//
//char DirectInput::KeyChar()
//{
//	char returnChar = 0;
//
//
//	return returnChar;
//}
//
//float DirectInput::ArrowKeyAngle()
//{
//	bool right = DirectInput::KeyState(DIK_RIGHT);
//	bool left = DirectInput::KeyState(DIK_LEFT);
//	bool up = DirectInput::KeyState(DIK_UP);
//	bool down = DirectInput::KeyState(DIK_DOWN);
//
//	if (right && up)return 45.0f;
//	if (up && left)return 135.0f;
//	if (left && down)return 225.0f;
//	if (down && right)return 315.0f;
//
//	if (right)return 0.0f;
//	if (up)return 90.0f;
//	if (left)return 180.0f;
//	if (down)return 270.0f;
//
//	return -1.0f;
//}
//#pragma endregion
//
//#pragma region �p�b�h
//
//bool DirectInput::ButtonState(int button)
//{
//	if (!devPad)return false;
//	if (padState.rgbButtons[button])
//	{
//		//�����Ă�Ƃ�
//		return true;
//	}
//	else
//	{
//		//�����ĂȂ��Ƃ�
//		return false;
//	}
//}
//
//bool DirectInput::ButtonTrigger(int button)
//{
//	if (!devPad)return false;
//	//1�t���[���O��true��false��
//	bool check;
//	if (padPrevious.rgbButtons[button])
//	{
//		check = true;
//	}
//	else
//	{
//		check = false;
//	}
//
//	if (ButtonState(button) && !check)
//	{
//		//�����ꂽ�u��(�O�t���[��������ĂȂ�&���t���[�������ꂽ)��
//		return true;
//	}
//
//	return false;
//}
//
//bool DirectInput::ButtonRelease(int button)
//{
//	if (!devPad)return false;
//	bool check;
//	if (padPrevious.rgbButtons[button])
//	{
//		check = true;
//	}
//	else
//	{
//		check = false;
//	}
//
//	if (!ButtonState(button) && check)
//	{
//		return true;
//	}
//
//	return false;
//}
//
//
//bool DirectInput::DirectionalButtonState(int button)
//{
//	//�����Ă�Ƃ�
//
//	if (!devPad)return false;
//
//	switch (button)
//	{
//	case UpButton:
//		if (padState.rgdwPOV[0] <= 4500 ||
//			padState.rgdwPOV[0] >= 31500 &&
//			padState.rgdwPOV[0] <= 36000)
//		{
//			return true;
//		}
//		break;
//
//	case RightButton:
//		if (padState.rgdwPOV[0] <= 13500 &&
//			padState.rgdwPOV[0] >= 4500) return true;
//		break;
//
//	case DownButton:
//		if (padState.rgdwPOV[0] <= 22500 &&
//			padState.rgdwPOV[0] >= 13500) return true;
//		break;
//
//	case LeftButton:
//		if (padState.rgdwPOV[0] <= 31500 &&
//			padState.rgdwPOV[0] >= 22500)
//		{
//			return true;
//		}
//		break;
//	}
//	return false;
//
//
//}
//
//bool DirectInput::DirectionalButtonTrigger(int button)
//{
//
//	//�����Ă�Ƃ�
//
//	if (!devPad)return false;
//
//	switch (button)
//	{
//	case UpButton:
//		if (padState.rgdwPOV[0] <= 4500 ||
//			padState.rgdwPOV[0] >= 31500 &&
//			padState.rgdwPOV[0] <= 36000)
//		{
//			if (padPrevious.rgdwPOV[0] <= 4500 ||
//				padPrevious.rgdwPOV[0] >= 31500 &&
//				padPrevious.rgdwPOV[0] <= 36000) {
//			}
//			else
//				return true;
//		}
//		break;
//
//	case RightButton:
//		if (padState.rgdwPOV[0] <= 13500 &&
//			padState.rgdwPOV[0] >= 4500)
//		{
//			if (padPrevious.rgdwPOV[0] <= 13500 &&
//				padPrevious.rgdwPOV[0] >= 4500) {
//			}
//			else
//				return true;
//		}
//		break;
//
//	case DownButton:
//		if (padState.rgdwPOV[0] <= 22500 &&
//			padState.rgdwPOV[0] >= 13500)
//		{
//			if (padPrevious.rgdwPOV[0] <= 22500 &&
//				padPrevious.rgdwPOV[0] >= 13500) {
//			}
//			else
//				return true;
//		}
//		break;
//
//	case LeftButton:
//		if (padState.rgdwPOV[0] <= 31500 &&
//			padState.rgdwPOV[0] >= 22500)
//		{
//			if (padPrevious.rgdwPOV[0] <= 31500 &&
//				padPrevious.rgdwPOV[0] >= 22500) {
//			}
//			else
//				return true;
//		}
//		break;
//	}
//	return false;
//}
//
//bool DirectInput::LeftStickLeft(int lXNum)
//{
//	if (!devPad)return false;
//
//	if (32767 - lXNum >= padState.lX)
//	{
//		return true;
//	}
//	return false;
//
//}
//bool DirectInput::LeftStickRight(int lXNum)
//{
//	if (!devPad)return false;
//	if (32767 + lXNum <= padState.lX)
//	{
//		return true;
//	}
//	return false;
//}
//bool DirectInput::LeftStickUp(int lYNum)
//{
//	if (!devPad)return false;
//	if (32767 - lYNum >= padState.lY)
//	{
//		return true;
//	}
//	return false;
//}
//bool DirectInput::LeftStickDown(int lYNum)
//{
//	if (!devPad)return false;
//	if (32767 + lYNum <= padState.lY)
//	{
//		return true;
//	}
//	return false;
//}
//bool DirectInput::RightStickLeft(int lXNum)
//{
//	if (!devPad)return false;
//	if (32767 - lXNum >= padState.lZ)
//	{
//		return true;
//	}
//	return false;
//
//}
//bool DirectInput::RightStickRight(int lXNum)
//{
//	if (!devPad)return false;
//	if (32767 + lXNum <= padState.lZ)
//	{
//		return true;
//	}
//	return false;
//}
//bool DirectInput::RightStickUp(int lYNum)
//{
//	if (!devPad)return false;
//	if (32767 - lYNum >= padState.lRz)
//	{
//		return true;
//	}
//	return false;
//}
//bool DirectInput::RightStickDown(int lYNum)
//{
//	if (!devPad)return false;
//	if (32767 + lYNum <= padState.lRz)
//	{
//		return true;
//	}
//	return false;
//}
//
//
//#pragma endregion
//
//
//
//void DirectInput::Release()
//{
//	//DirectInput�����
//	/*keyDev->Unacquire();
//	keyDev->Release();
//	dInput8->Release();*/
//
//	devKeyBoard->Unacquire();
//	devKeyBoard->Release();
//	/*if (devPad)
//	{
//		devPad->Unacquire();
//		devPad->Release();
//	}*/
//	dInputDevice->Release();
//
//}
//
//#pragma region �}�E�X
//
//void DirectInput::SetMatrixAndNearFar(DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix, float nearNumber, float farNumber)
//{
//	viewMat = viewMatrix;
//	projectionMat = projectionMatrix;
//	nearNum = nearNumber;
//	farNum = farNumber;
//}
//
//Vector2 DirectInput::GetMousePosition()
//{
//	POINT p;
//
//	//�}�E�X�̃X�N���[�����W���擾
//	GetCursorPos(&p);
//
//	//�N���C�A���g���W�ɕϊ�
//	ScreenToClient(mHwnd, &p);
//
//	RECT rect;//���݂̃E�B���h�E�T�C�Y������
//	Vector2 pos = { (float)p.x,(float)p.y };
//	float num;
//
//	//���ꂢ��Ȃ�?
//	/*p.x = p.x <= 0 ? 0 : p.x;
//	p.y = p.y <= 0 ? 0 : p.y;*/
//
//	//���݂̃E�B���h�E�T�C�Y���擾
//	GetClientRect(mHwnd, &rect);
//
//	//�E�B���h�E�T�C�Y��ς��Ă����W�̏�����ς��Ȃ��悤�ɕϊ�
//	num = (float)rect.right / (float)winWidth;
//	pos.x = (float)pos.x / num;
//
//	num = (float)rect.bottom / (float)winHeight;
//	pos.y = (float)pos.y / num;
//
//	return pos;
//
//}
//
//Vector2 DirectInput::GetCenterToMouseVector()
//{
//	Vector2 point = { static_cast<float>(winWidth) / 2.0f, static_cast<float>(winHeight) / 2.0f };
//	return Vector2Normalize(GetMousePosition() - point);
//}
//
//float DirectInput::GetMouseAngle()
//{
//	return LibMath::Vecto2ToAngle(GetCenterToMouseVector(), false);
//}
//
//void DirectInput::GetMouse3DLine(Vector3& nearPoint, Vector3& farPoint)
//{
//	Vector2 mousePos = GetMousePosition();
//
//	DirectX::XMMATRIX mousePosMatrix;
//	DirectX::XMMATRIX viewportMatrix;
//	DirectX::XMMATRIX worldMousePosMatrix;
//
//	//�t�s��
//	DirectX::XMMATRIX invViewPortMatrix;
//	DirectX::XMMATRIX invViewMatrix;
//	DirectX::XMMATRIX invProjectionMatrix;
//
//
//	//�r���[�|�[�g�s����쐬
//	viewportMatrix = DirectX::XMMatrixIdentity();
//	viewportMatrix.r[0].m128_f32[0] = (float)winWidth / 2.0f;
//	viewportMatrix.r[1].m128_f32[1] = -(float)winHeight / 2.0f;
//	viewportMatrix.r[3].m128_f32[0] = (float)winWidth / 2.0f;
//	viewportMatrix.r[3].m128_f32[1] = (float)winHeight / 2.0f;
//
//	//�}�E�X���W�̍s��
//	mousePosMatrix = DirectX::XMMatrixIdentity();
//
//	invViewPortMatrix = DirectX::XMMatrixInverse(nullptr, viewportMatrix);
//	invViewMatrix = DirectX::XMMatrixInverse(nullptr, viewMat);
//	invProjectionMatrix = DirectX::XMMatrixInverse(nullptr, projectionMat);
//
//#pragma region Z = 0(�ŋߓ_)
//
//	//z = 0�̏ꍇ
//	mousePosMatrix = DirectX::XMMatrixTranslation(mousePos.x, mousePos.y, 0.0f);
//	//�X�N���[�����W�����[���h���W��
//	worldMousePosMatrix = DirectX::XMMatrixIdentity();
//
//	worldMousePosMatrix *= mousePosMatrix;
//	worldMousePosMatrix *= invViewPortMatrix;
//	worldMousePosMatrix *= invProjectionMatrix;
//
//	//W���Z
//	worldMousePosMatrix.r[3].m128_f32[0] /= worldMousePosMatrix.r[3].m128_f32[3];
//	worldMousePosMatrix.r[3].m128_f32[1] /= worldMousePosMatrix.r[3].m128_f32[3];
//	worldMousePosMatrix.r[3].m128_f32[2] /= worldMousePosMatrix.r[3].m128_f32[3];
//	worldMousePosMatrix.r[3].m128_f32[3] /= worldMousePosMatrix.r[3].m128_f32[3];
//
//	worldMousePosMatrix *= invViewMatrix;
//
//	//�ŋߓ_
//	nearPoint =
//	{
//		worldMousePosMatrix.r[3].m128_f32[0] ,
//		worldMousePosMatrix.r[3].m128_f32[1] ,
//		worldMousePosMatrix.r[3].m128_f32[2]
//	};
//#pragma endregion
//
//
//#pragma region z = 1(�ŉ��_)
//
//
//	//z = 1�̏ꍇ
//	mousePosMatrix = DirectX::XMMatrixTranslation(mousePos.x, mousePos.y, 1.0f);
//
//	//�X�N���[�����W�����[���h���W��
//	worldMousePosMatrix = DirectX::XMMatrixIdentity();
//
//	worldMousePosMatrix *= mousePosMatrix;
//	worldMousePosMatrix *= invViewPortMatrix;
//	worldMousePosMatrix *= invProjectionMatrix;
//
//	//W���Z
//	worldMousePosMatrix.r[3].m128_f32[0] /= worldMousePosMatrix.r[3].m128_f32[3];
//	worldMousePosMatrix.r[3].m128_f32[1] /= worldMousePosMatrix.r[3].m128_f32[3];
//	worldMousePosMatrix.r[3].m128_f32[2] /= worldMousePosMatrix.r[3].m128_f32[3];
//	worldMousePosMatrix.r[3].m128_f32[3] /= worldMousePosMatrix.r[3].m128_f32[3];
//
//	worldMousePosMatrix *= invViewMatrix;
//
//
//	//�ŉ��_
//	farPoint =
//	{
//		worldMousePosMatrix.r[3].m128_f32[0] ,
//		worldMousePosMatrix.r[3].m128_f32[1] ,
//		worldMousePosMatrix.r[3].m128_f32[2]
//	};
//
//#pragma endregion
//}
//
//#pragma region �{�^��
//
////�{�^���z��
////0 ���{�^��
////1 �E�{�^��
////2 ���{�^��
////3 ���[�̃{�^��2�̉���(��͔�������)
//
//bool DirectInput::MouseButtonState(MouseButton mouseButton)
//{
//	if (mouseState.rgbButtons[mouseButton]) return true;
//	return false;
//}
//bool DirectInput::MouseButtonTrigger(MouseButton mouseButton)
//{
//	if (!mousePrevious.rgbButtons[mouseButton])
//	{
//		if (MouseButtonState(mouseButton)) return true;
//	}
//	return false;
//}
//bool DirectInput::MouseButtonRelease(MouseButton mouseButton)
//{
//	if (mousePrevious.rgbButtons[mouseButton])
//	{
//		if (!MouseButtonState(mouseButton)) return true;
//	}
//	return false;
//}
//#pragma endregion
//
//
//#pragma endregion
