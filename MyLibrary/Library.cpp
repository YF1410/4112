#include "Library.h"
#include"LibMath.h"

#include"Input.h"

#pragma comment(lib,"winmm.lib")//timeGetTime��
#include"GameObjectManager.h"
#include"SceneManager.h"
#include"GuiValueManager.h"

#include"Audio.h"
#include"Sound.h"

#include"Random.h"
#include"FrameTimer.h"

#include"LibWinAPI.h"
#include"TextWrite.h"

#include"Material.h"
#include"SpriteFont2D.h"


using namespace MelLib;

DirectX12* Library::dx12;
//CreatePolygon* Library::createPolygon;

WNDCLASSEX Library::w;
MSG  Library::msg;
HWND Library::hwnd;
bool Library::isEnd;

UINT Library::count;

int Library::createPointCount;

bool Library::isSetFPS60;
UINT Library::fps;

int Library::loadFontTextureCounter;
int Library::loadTextureCounter;
int Library::createSpriteConter;

int Library::createPipelineCounter;
int Library::createPostEffectPuiperineCounter;

HDC Library::hdc;
int Library::refReat;
DWORD Library::startProsessTime;
DWORD Library::nowTime;

std::vector<std::tuple<ModelDataAndKey, int>> Library::modelDatas;

int Library::winWidth;
int Library::winHeight;
Color Library::clearColor;
std::wstring Library::winName;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);


//���p�҂��E�B���h�E�����삵�Ďg���ꍇ�́A���p�҂��p�ӂ���Proc�ł���Proc���Ăяo���Ă��炦�΁A
//ImGui�̏������s����
LRESULT Library::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam);
	return DefWindowProc(hwnd, msg, wparam, lparam);

}

void Library::Initialize(int windowWidth, int windowHeight, const Color& screenColor, const wchar_t* windowName)
{
	 winWidth = windowWidth;
	 winHeight = windowHeight;
	 clearColor = screenColor;
 	 winName = windowName;

	auto coResult = CoInitialize(NULL);

	dx12 = DirectX12::GetInstance();
	/*createPolygon = CreatePolygon::GetInstance();
	createPolygon->Initialize(windowWidth, windowHeight);*/

	srand((unsigned int)time(NULL));
	Random::Initialize();
	GameObjectManager::GetInstance()->Initialize();

	count = 0;
	fps = 60;
	startProsessTime = timeGetTime();
	nowTime = timeGetTime();

#pragma region �E�B���h�E����

	hwnd = LibWinAPI::CreateNormalWindow
	(
		windowName,
		windowName,
		WS_OVERLAPPEDWINDOW /*| WS_MAXIMIZE *//*| WS_CLIPCHILDREN*/,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		windowWidth,
		windowHeight,
		nullptr,
		WindowProc
	);
#pragma endregion

#pragma region DC

	//DC���擾
	hdc = GetDC(hwnd);
	//���t���b�V�����[�g���擾
	refReat = GetDeviceCaps(hdc, VREFRESH);
#pragma endregion

	

	dx12->SetScreenColor(screenColor);
	dx12->Initialize(hwnd, windowWidth, windowHeight);
	


	Input::Initialize(hwnd, windowWidth, windowHeight);
	Audio::GetInstance()->Initialize();
	SpriteFont2D::GetInstance()->Initialize();


	MelLib::GuiValueManager::GetInstance()->Initialize();

#ifdef _DEBUG


	GameObject::CreateCollisionCheckModelPipelineState();
#endif // _DEBUG
}

void Library::LoopStartProcess()
{
	startProsessTime = timeGetTime();

#pragma region �E�B���h�E����

	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	if (msg.message == WM_QUIT)
	{
		isEnd = true;
	}
	

#pragma endregion


	dx12->LoopStartProcess();

	Input::Update();
	Sound::Update();
	SpriteFont2D::GetInstance()->Update();
	FrameTimer::AllUpdate();
	Camera::AllCalcCameraData();

	GuiValueManager::GetInstance()->Update();

}

void Library::LoopEndProcess()
{
 	dx12->LoopEndProcess();
	
	if (isSetFPS60)
	{

		//60fps�̎���1�t���[���̏������Ԃ��v��(���̐��l�́A���Ɉˑ����Ȃ�)
		//���ꖈ��v������K�v�Ȃ��Bfps�ς������ɕύX����΂���
		float pureTime = 1000.0f / (float)fps * ((float)count - 1.0f);
		float currTime = 1000.0f / (float)fps * (float)count;
		int taikiTime = (int)currTime - (int)pureTime;//�ҋ@���Ȃ��Ƃ����Ȃ�����
		count++;
		count = count >= fps ? 0 : count;

		//�O���[�v����ǂ̂��炢���Ԃ��o�߂��������v��(pre���AstartProsess�̂ق��Ɉڂ��AGetTickCount64�����)
		//previousTime = currentTime;
		nowTime = timeGetTime();
		DWORD keika = nowTime - startProsessTime;
		//sabun /= 1000;

		//�ҋ@���Ԃ��v��(taikiTime��sabun�̍�����slee�őҋ@)
		int sleepTime = taikiTime - keika;
		sleepTime = sleepTime <= 0 ? 0 : sleepTime;

		//�X���[�v
		timeBeginPeriod(1);
		Sleep(sleepTime);
		timeEndPeriod(1);
	}



}

void Library::EndFlagTrue()
{
	isEnd = true;
}

bool Library::GetIsEnd()
{
	return isEnd;
}

void Library::Finalize()
{
	if (hwnd) DestroyWindow(hwnd);
	//LoopEndProcess�Ă�ł���I������̂ŁAdx12���Ƀ����[�X���Ȃ��ƃ��f���̃o�b�t�@�������ďI��������肭���s�ł��Ȃ��Ȃ�
	dx12->Finalize();

	SceneManager::GetInstance()->Finalize();
	GameObjectManager::GetInstance()->Finalize();
	Input::Finalize();

	Sound::Finalize();
	SoundData::Finalize();

	Camera::Finalize();

	UnregisterClass(w.lpszClassName, w.hInstance);

	CoUninitialize();
}


#pragma region �ݒ�

void Library::SetFramesPerSecond60(bool flag)
{
	isSetFPS60 = flag;
}
//
//void Library::setScreenColor(Color color)
//{
//	dx12->setScreenColor(color);
//}

#pragma endregion


