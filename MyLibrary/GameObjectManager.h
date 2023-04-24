#pragma once
#include"GameObject.h"
#include"GameObject2D.h"
#include"CollisionDetectionData.h"
#include"MouseCursor.h"

#include<vector>
#include<memory>

//�J�[�\������p�̃\�[�g����āA�S�̃\�[�g�Ȃ���
//�J�[�\������p�z��Ɉڂ��ă\�[�g

//�����2��m�F����̏�������(objA��objB�̔��苁�߂�Ƃ��AobjA�����AobjB����Hit���Ăяo���̂����������Ȃ�)����ǂ��ɂ�����B

// shared_ptr�ɂ��Ă闝�R
// ���|�C���^��n�������̏ꍇ�A
// �C���X�^���X�̃|�C���^��n���������ƊԈႦ�ă������m�ۂ��ēn���Ă��܂�(new �Z�Z)���������������Ȃ��Ȃ��Ă��܂��\��������
// (new�Ŋm�ۂ��ĂȂ���������delete�ŊJ������Ɩ�肪���邽�߁A����������}�l�[�W���[�ōs���Ȃ�)
// new�Z�Z��n���ꍇ�A�폜���Ƀ��������J�����Ȃ��Ƃ����Ȃ����A�Ԉ����new�Ŋm�ۂ��ĂȂ����������J�����Ă��܂��\�������邽��
// �����łǂ��炩�w�肵�Ă��炤�悤�ɂ��Ă��A�~�X�ŃG���[���o��\��������
// �G���[���o�Ȃ��A���������������Ȃ��Ƃ�����肪�N���炸�A
// ����2�̃p�^�[��(�C���X�^���X�̃|�C���^��n�������Anew�Z�Z��n������)�𗼗��ł��邽��shared_ptr�ɂ��Ă���

namespace MelLib
{
	class GameObjectManager
	{
	public:
		enum ObjectSortType
		{
			//XYZ�̍��v�l�Ń\�[�g
			OBJECT_SORT_NONE,
			OBJECT_SORT_XYZ_SUM,
			OBJECT_SORT_X,
			OBJECT_SORT_Y,
			OBJECT_SORT_Z,
			//�J��������߂���
			OBJECT_SORT_NEAR_DISTANCE,
			//�J�������牓����
			OBJECT_SORT_FAR_DISTANCE,
			//�\�[�g�ԍ��ł̃\�[�g
			OBJECT_SORT_SORT_NUMBER,
		};


	private:

		std::vector<std::shared_ptr<GameObject>>objects;
		//�ǉ����ꂽ���̂��ꎞ�I�ɓ���Ă����z��
		std::vector<std::shared_ptr<GameObject>>addObjects;
		// ���o�^����Ă��邩�m�F���邽�߂̔ԍ����i�[����z��
		std::unordered_map<std::string, int>objectAddNumber;

		std::vector<std::shared_ptr<GameObject2D>>object2Ds;
		std::vector<std::shared_ptr<GameObject2D>>addObject2Ds;
		//CollisionFlag checkCollision;

		// �ǉ������I�u�W�F�N�g�̔z��
		// shared_ptr���|�C���^�̕����o�C�g�����Ȃ������r���Ȃ��ď�����������|�C���^
		// �ύX�������̏����������������ʓ|�����炱��Ŏ�������̌��
		// ���ꂩ��擾����Ȃ疼�O�ύX���������m�F����t���O�p�ӂ���
		//std::unordered_map<GameObject*, std::string>objectNames;



		//�ǉ������t���[�����ƂɃ\�[�g���邩
		ObjectSortType addObjectSort = OBJECT_SORT_NONE;
		bool addObjectSortOrderType = false;

		//�J�[�\������
		bool checkMouseCollision = false;
		Vector3 cameraPosition;
		std::unique_ptr<MouseCursor>cursor;

		//�f�[�^
		Vector3 nearPos;
		Vector3 farPos;

	private:

		GameObjectManager();
		~GameObjectManager();
		/// <summary>
		/// eraseManager��true���ǂ������m�F���܂��Btrue�̏ꍇ�͍폜����܂�
		/// </summary>
		void EraseObjectCheck();

		/// <summary>
		/// �������O�̃I�u�W�F�N�g�����ɓo�^����Ă��邩���m�F���܂��B
		/// </summary>
		/// <returns></returns>
		bool CheckObjectName(const std::string& name)const;
	public:
		GameObjectManager(const GameObjectManager& obj) = delete;
		GameObjectManager& operator=(const GameObjectManager& obj) = delete;

		static GameObjectManager* GetInstance();

		void Initialize();

		void InitializeObject();

		void Update();

		void Draw();

		void Finalize();

		///// <summary>
		///// �ǂ̔�����s������ݒ肵�܂�
		///// </summary>
		///// <param name="type"></param>
		//void SetCollisionFlag3D(const CollisionFlag& type);

		/// <summary>
		/// �}�E�X�J�[�\���Ƃ̔�����Ƃ邩�ݒ肵�܂�
		/// </summary>
		/// <param name="flag"></param>
		void SetMouseCollisionFlag(const bool flag);

		void CheckHit();

#pragma region �I�u�W�F�N�g�̔z��֌W

		/// <summary>
		/// �I�u�W�F�N�g�̔z��̃��������m�ۂ��܂��B
		/// </summary>
		/// <param name="reserveNum"></param>
		void ReserveObjectArray(const int reserveNum);

		/// <summary>
		/// �I�u�W�F�N�g��ǉ����܂��B
		/// </summary>
		/// <param name="object"></param>
		void AddObject(const std::shared_ptr<GameObject>& object);


		/// <summary>
		/// �I�u�W�F�N�g��ǉ����܂��B
		/// </summary>
		/// <param name="object"></param>
		void AddObject(const std::shared_ptr<GameObject2D>& object);

		/// <summary>
		/// �ǉ������t���[�����ƂɃ\�[�g���邩��ݒ肵�܂��B�ǉ����Ȃ��ꍇ�AorderType�͖�������܂��B
		/// </summary>
		/// <param name="sort">�\�[�g�̎d��</param>
		/// <param name="orderType">true ����  false �~��</param>
		void SetAddObjectSortState(const ObjectSortType& sort, const bool& orderType);


		/// <summary>
		/// �I�u�W�F�N�g�̔z����\�[�g���܂��B
		/// </summary>
		/// <param name="sort">�\�[�g�̎d��</param>
		/// <param name="orderType">true ����  false �~��</param>
		void ObjectSort(const ObjectSortType& sort, const bool& orderType);

		/// <summary>
		/// �I�u�W�F�N�g�}�l�[�W���[�Ɋi�[���ꂽ�I�u�W�F�N�g�̔z���const�Q�ƂŎ󂯎��܂�
		/// </summary>
		/// <returns></returns>
		const std::vector<std::shared_ptr<GameObject>>& GetRefGameObject() { return objects; };

		/// <summary>
		/// �I�u�W�F�N�g�}�l�[�W���[�Ɋi�[���ꂽ�I�u�W�F�N�g2D�̔z���const�Q�ƂŎ󂯎��܂�
		/// </summary>
		/// <returns></returns>
		const std::vector<std::shared_ptr<GameObject2D>>& GetRefGameObject2D() { return object2Ds; };

		void GetObjectNames(std::vector<std::string>& refVector)const;

		// shared_ptr�Ԃ��ƌx���o��
		// �֐��ŎQ�Ƃ�n������o�Ȃ�?
		/// <summary>
		/// 
		/// </summary>
		/// <param name="name"></param>
		/// <returns></returns>
		GameObject* GetPGameObject(const std::string& name)const;
#pragma endregion

		void EraseObject(GameObject* p);

		/// <summary>
		/// �S�ẴI�u�W�F�N�g��z�񂩂�폜���܂�
		/// </summary>
		void AllEraseObject();

		void AllEraseObject2D();
		void AllEraseObject3D();


	};

}