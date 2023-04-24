#include "Game.h"

#pragma region ライブラリインクルード

#include<Library.h>
#include<SceneManager.h>
#include<GameObjectManager.h>
#include<Input.h>
#include<ImguiManager.h>

#pragma endregion


Game::Game() {}

Game::~Game() {}
Game* Game::GetInstance()
{
	static Game game;
	return &game;
}


void Game::Run()
{
	MelLib::ImguiManager::GetInstance()->SetReleaseDrawFlag(true);

	Initialize();

	while (1)
	{
		MelLib::Library::LoopStartProcess();

		if (MelLib::Input::KeyTrigger(DIK_ESCAPE))MelLib::Library::EndFlagTrue();
		if (MelLib::Library::GetIsEnd())break;
		
		//ImGui::ShowDemoWindow();

		Update();
		Draw();
		MelLib::Library::LoopEndProcess();

	}

	Finalize();
}

void Game::Initialize()
{

	MelLib::Library::Initialize(1920, 1080, MelLib::Color(0,0,255,255),L"MELLib");
	MelLib::Library::SetFramesPerSecond60(true);

#pragma region マネージャー初期化

	/*CollisionFlag initFlag;
	initFlag.board = true;
	initFlag.box = true;
	initFlag.lineSegment = true;
	initFlag.plane = true;
	initFlag.ray = true;
	initFlag.sphere = true;
	GameObjectManager::GetInstance()->SetCollisionFlag3D(initFlag);*/
	MelLib::GameObjectManager::GetInstance()->SetMouseCollisionFlag(true);
	MelLib::GameObjectManager::GetInstance()->ReserveObjectArray(100);

	
#pragma endregion

}


void Game::Finalize()
{
	MelLib::Library::Finalize();//絶対に最後に書く
}


void Game::Update()
{
	//MelLib::SceneManager::GetInstance()->Update();
	//MelLib::SceneEditer::GetInstance()->Update();
	//MelLib::GameObjectManager::GetInstance()->Update();
}

void Game::Draw()
{
	//MelLib::SceneManager::GetInstance()->Draw();
	//MelLib::SceneEditer::GetInstance()->Draw();

	//MelLib::GameObjectManager::GetInstance()->Draw();

}