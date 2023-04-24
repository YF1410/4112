#include "SceneManager.h"
#include"SceneEditer.h"
#include<assert.h>
#include<typeinfo>
#include"GameObjectManager.h"

using namespace MelLib;

SceneManager::SceneManager()
{
}


SceneManager::~SceneManager()
{

}

void MelLib::SceneManager::Change()
{
	// �����폜
	// �G�f�B�^�[�ł̃V�[���̓ǂݍ��ݒ������Ɍ��݂̃V�[���ɐ����Ȃǂ̏������Ȃ��Ə����Ă��܂����߁A
	// ��U�����I�ɏ����Ė���ǂݍ��܂���悤�ɂ���
	GameObjectManager::GetInstance()->AllEraseObject();

	//�V�[���擾
	Scene* newScene = currentScene->GetNextScene();


	//�I������
	currentScene->Finalize();

	// �Ⴄ�V�[���ɐ؂�ւ���ꍇ�A���\�[�X������
	if (typeid(currentScene) != typeid(newScene))
	{
		//�ꊇ�폜�Ώۃ��\�[�X���폜
		currentScene->ResourceBatchDeletion();
	}

	//�V�[���؂�ւ�

	//�����|�C���^�Z�b�g�h�~
	if (newScene == currentScene)assert(0);
	//�V�[�����폜
	delete currentScene;
	//����ւ�
	currentScene = newScene;

	//������
	currentScene->FalseIsEnd();
	currentScene->Initialize();
}

SceneManager* SceneManager::GetInstance()
{
	static SceneManager inst;
	return &inst;
}

void SceneManager::SetStartScene(Scene* startScene)
{

	if (!startScene) assert(0);

	//SceneEditer::GetInstance().

	currentScene = startScene;
	currentScene->Initialize();
}

void SceneManager::Update()
{

	if (!currentScene 
	|| SceneEditer::GetInstance()->GetIsEdit() 
		&& SceneEditer::GetInstance()->GetReleaeEdit() 
		&& SceneEditer::GetInstance()->GetEditerFlag())return;
	
	if (currentScene->GetIsEnd())Change();

	currentScene->Update();

}

void SceneManager::Draw()
{
	if (!currentScene)return;
	currentScene->Draw();
}

void SceneManager::Finalize()
{
	if (currentScene)
	{
		currentScene->Finalize();
		delete currentScene;
	}
	
}

Scene* SceneManager::GetCurrentScene()
{
	return currentScene;
}

std::string MelLib::SceneManager::GetCurrentSceneName()
{
	std::string name = typeid(*currentScene).name();

	// �擪6����("class ")���폜
	name.erase(name.begin(), name.begin() + 6);

	return name;
}

void MelLib::SceneManager::ReLoadScene()
{
	GameObjectManager::GetInstance()->AllEraseObject();
	if (!currentScene)return;
	currentScene->Initialize();
}
