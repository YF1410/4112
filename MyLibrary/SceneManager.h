#pragma once
#include<vector>
#include<unordered_map>
#include"Scene.h"

namespace MelLib 
{
	class SceneManager final
	{

	private:

		SceneManager();
		~SceneManager();

		Scene* currentScene = nullptr;

		//bool stopUpdate = false;

		/// <summary>
		/// �V�[���̐؂�ւ��������s��
		/// </summary>
		void Change();
	public:

		SceneManager(const SceneManager& sceneManager) = delete;//�R�s�[�R���X�g���N�^
		SceneManager& operator=(const SceneManager& sceneManager) = delete;//�R�s�[������Z�q
		static SceneManager* GetInstance();

		void SetStartScene(Scene* startScene);
		void Update();
		void Draw();
		void Finalize();


		/// <summary>
		/// �V�[����؂�ւ��鏈�����s���܂�
		/// </summary>
		/// <param name="nextScene"></param>
		//void ChengeScene(Scene* nextScene);

		//void SetStopFlag(bool flag) { stopUpdate = flag; }
		//void ChangeStopFlag() { stopUpdate = !stopUpdate; }

		//void ResetScene();

		Scene* GetCurrentScene();
		std::string GetCurrentSceneName();

		/// <summary>
		/// �V�[�����ēǂݍ��݂��܂��B
		/// </summary>
		void ReLoadScene();
	};

}