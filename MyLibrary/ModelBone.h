#pragma once
#include"GameObject.h"
#include"Vector.h"

// �����G���A
// 2022 08 08
// �{�[���̍\���̂܂��̓N���X��V���ɍ�肽��
// �����蔻�������������A�ʒu��ς�����悤�ɂ�����
// �����ς��𓮂��������񂶂�!
// ���ƒn�`�ɑ����߂荞�܂Ȃ��悤�ɂ�����

// �e�{�[���Ƃ̍ő勗����ݒ�\�ɂ��Ă���ȏ㗣��Ȃ��悤��
// �n���͒l�����������ē������Ȃ��悤�ɁA�����͒l��傫�����ē�������悤��

namespace MelLib 
{

	class ModelBone :public MelLib::GameObject
	{
	private:
		//�{�[����
		std::string boneName;
		//�����p���̋t�s��
		DirectX::XMMATRIX invInitialPose;

		// �O�̃{�[��
		ModelBone* parentBone = nullptr;

		//�{�[�������܂Ƃ߂�����
		FbxCluster* fbxCluster = nullptr;

		// ���Z�n
		Vector3 addPosition;
		Vector3 addAngle;
		Vector3 addScale;

	public:

		void SetAddPosition(const Vector3& vector) { addPosition = vector; }
		void SetAddAngle(const Vector3& vector) { addAngle = vector; }
		void SetAddScale(const Vector3& vector) { addScale = vector; }

		Vector3 GetAddPosition()const { return addPosition; }
		Vector3 GetAddAngle() const { return addAngle; }
		Vector3 GetAddScale()const { return addScale; }


		// �֐��ŊO�����瓖���蔻����Z�b�g�ł���悤�ɂ���

	};

}