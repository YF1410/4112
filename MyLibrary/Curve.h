#pragma once
#include<vector>
#include"Vector.h"

namespace MelLib
{
	//�x�W�F�A�X�v���C���Ȃǂ̋Ȑ��N���X
	class Curve
	{

	private:
		//�Ȑ���̂ǂ��ɂ��邩(0����1)
		float t = 0.0f;
		//�_�̍��W
		std::vector<Vector3> points;
		//t >= 1�̎���t��0�ɂ��邩�ǂ���
		bool isMaxNumberReset = false;

		/// <summary>
		/// ���W�v�Z
		/// </summary>
		Vector3 CalcPosition();

	public:
		Curve() {}
		~Curve() {}

		/// <summary>
		/// �n�_�I�_����_���Z�b�g���܂��B
		/// </summary>
		/// <param name="points">�_�̔z��</param>
		void SetPoints(const std::vector<Vector2>& points);


		/// <summary>
		/// �n�_�I�_����_���Z�b�g���܂��B
		/// </summary>
		/// <param name="points">�_�̔z��</param>
		void SetPoints(const std::vector<Vector3>& points);

		/// <summary>
		/// t(�Ȑ���̈ʒu��\���l(0����1))�ɉ��Z���鐔�l�B
		/// </summary>
		/// <param name="num"></param>
		void AddNumber(const float num);

		/// <summary>
		/// �Ȑ���̍��W���擾���܂��B
		/// </summary>
		/// <returns>Vector2�^�̍��W</returns>
		Vector2 GetVector2Position();

		/// <summary>
		/// �Ȑ���̍��W���擾���܂��B
		/// </summary>
		/// <returns>Vector3�^�̍��W</returns>
		Vector3 GetVector3Position();

		/// <summary>
		/// �I�_�ɂ��ǂ蒅�����Ƃ��Ɏn�_�߂邩�ǂ������Z�b�g���܂��B
		/// </summary>
		/// <param name="flag"></param>
		void SetIsMaxNumberReset(const bool flag) { isMaxNumberReset = flag; }
	};
}

