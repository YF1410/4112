#pragma once
#include"ModelObject.h"

//���_���߂�̂��������Ȃ�����A
//�����ł����A�E�g�v�b�g�X�g���[���g���Ē��_�����炤�ׂ�?
//���_��������Ζ��Ȃ���

//����͂ǂ�����?���b�V���R���C�_�[?


namespace MelLib 
{

	//�z�V�~�����[�^���s���N���X
	class ClothSimulator
	{
	public:
		//�e�b�Z���[�^�ł̕����������ɁA�����蔻����Z�b�g����֐�
		static void SetClothCollisionData(ModelObject& modelObj);


	};
}

