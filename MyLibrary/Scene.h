#pragma once
#include<string>

namespace MelLib 
{
	class Scene
	{

	protected:
		bool isEnd = false;

		bool isEdit = false;
	public:
		Scene();
		virtual ~Scene();
		void FalseIsEnd();
		virtual void Initialize() = 0;//������
		virtual void Update() = 0;
		virtual void Draw() = 0;
		virtual void Finalize() = 0;//�I������
		virtual Scene* GetNextScene() = 0;//���̃V�[���̎w��
		bool GetIsEnd();//�V�[�����I��������ǂ���
		void* GetPtr();


		// �ȉ��J���җp�֐�

		void Edit();
		
		bool GetIsEdit()const { return isEdit; }

		/// <summary>
		/// �ꊇ�폜�̑ΏۂɂȂ��Ă��郊�\�[�X���폜���܂��B���̊֐��͎����ŌĂяo����邽�߁A���p�҂��Ăяo���K�v�͂���܂���B
		/// </summary>
		void ResourceBatchDeletion();

	};
}
