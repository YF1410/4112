#pragma once
#include"Vector.h"
#include"Values.h"
#include"CollisionDetectionData.h"

namespace MelLib 
{
	// A*�p�m�[�h(�}�X)
	struct AStarNode
	{
		using UINT = unsigned int;

		//���W
		Vector3 position;

		Vector3 size;

		//�R�X�g
		UINT cost = 1;

		//�i�s�s�\�I�u�W�F�N�g�Əd�Ȃ��Ă���m�[�h���ǂ���
		bool hitObjectNode = false;

	};


	// ���x�ł�������A�v���C���[��G�̑傫�����l�����Čo�H���v�Z�ł���悤�ɂ���

	// �o�H�T���N���X
	class RouteSearch
	{
		using UINT = unsigned int;
	public:
#pragma region �ŒZ�o�H

		///// <summary>
		///// ��������Ƀm�[�h�̍��W���Z�b�g���܂��B
		///// </summary>
		///// <param name="leftUpPos">������W</param>
		///// <param name="rightDownPos">�E�����W</param>
		///// <param name="nodeNumX">��������</param>
		///// <param name="nodeNumY">�c������</param>
		///// <param name="nodes">�m�[�h��vector(size��0�ł悢)</param>
		///// <param name="upPlus">��������v���X���ǂ���</param>
		//static void SetAStarNodePosition
		//(
		//	const Vector2& leftUpPos,
		//	const Vector2& rightDownPos,
		//	const UINT& nodeNumX,
		//	const UINT& nodeNumY,
		//	std::vector< std::vector<AStarNode>>& nodes,
		//	const bool upPlus
		//);

		////���̊֐��ŃR�X�g��1����Ȃ��I�u�W�F�N�g�Ƃ��̃R�X�g��n���悤�ɂ���?
		///// <summary>
		///// �m�[�h���i�s�s�\�I�u�W�F�N�g�Ƀq�b�g���Ă邩���m�F���܂��B
		///// </summary>
		///// <param name="hitObjectsPos">��Q���̍��W��vector</param>
		///// <param name="hitObjectsSize">��Q���̃T�C�Y��vector</param>
		///// <param name="nodes">�m�[�h��vector(SetAStarNodePosition�ɓn������̔z��)</param>
		//static void SetAStarNodeHitObjectNodeFlag
		//(
		//	const std::vector<Vector2>& hitObjectsPos,
		//	const std::vector<Vector2>& hitObjectsSize,
		//	std::vector< std::vector<AStarNode>>& nodes
		//);

		///// <summary>
		///// �n���ꂽ�f�[�^�����ƂɍŒZ�o�H�����߂܂��B
		///// </summary>
		///// <param name="startPos">�X�^�[�g�n�_�̍��W</param>
		///// <param name="endPos">�S�[���n�_�̍��W</param>
		///// <param name="nodes">�m�[�h��vector(SetAStarNodeHitObjectNodeFlag�ɓn������̔z��)</param>
		///// <param name="routeVector">�S�[���܂ł̃��[�g���i�[����vector(size��0�ł悢)</param>
		///// <returns>�T���������������ǂ���</returns>
		//static bool GetAStarCalcResult
		//(
		//	const Vector2& startPos,
		//	const Vector2& endPos,
		//	std::vector< std::vector<AStarNode>>& nodes,
		//	std::vector<Vector2>& routeVector
		//);

#pragma endregion

#pragma region �V�o�H�T��
		/// <summary>
		/// ��������Ƀm�[�h�̍��W���Z�b�g���܂��B
		/// </summary>
		/// <param name="leftUpPos">������O���W</param>
		/// <param name="rightDownPos">�E�㉜���W</param>
		/// <param name="nodeNumX">XYZ�̕���������</param>
		/// <param name="nodes">�m�[�h��vector(size��0�ł悢)</param>
		static void SetNodePosition
		(
			const Vector3& leftDownNearPos,
			const Vector3& rightUpFarPos,
			const Value3<UINT>& nodeNum,
			std::vector<std::vector<std::vector<AStarNode>>>& nodes
		);

		//�I�u�W�F�N�g�}�l�[�W���[�Ŕ���v�Z����?
		//�I�u�W�F�N�g�}�l�[�W���[���Ɩ��t���[�����������Ⴄ����A��p�������������ɑg�ݍ��񂾂ق�����������
		//�I�u�W�F�N�g�}�l�[�W���[�̔z��擾���Čv�Z���Ă���������
		/// <summary>
		/// �m�[�h���i�s�s�\�I�u�W�F�N�g��R�X�g�Ƀq�b�g���Ă邩���m�F���܂��B
		/// </summary>
		/// <param name="hitObjectsBoxData">�I�u�W�F�N�g�̃f�[�^</param>
		/// <param name="nodes">�m�[�h��vector(SetAStarNodePosition�ɓn������̔z��)</param>
		static void SetHitObjectFlag
		(
			const std::vector<BoxData>& hitObjectsBoxData,
			std::vector<std::vector<std::vector<AStarNode>>>& nodes
		);

		/// <summary>
		/// �R�X�g��ݒ肵�܂��B�R�X�g�͉��Z����Ă����܂��B
		/// </summary>
		/// <param name="hitObjectsBoxData"></param>
		/// <param name="costs"></param>
		/// <param name="nodes"></param>
		static void SetCost
		(
			const std::vector<BoxData>& hitObjectsBoxData,
			const std::vector<UINT>& costs,
			std::vector<std::vector<std::vector<AStarNode>>>& nodes
		);

		/// <summary>
		/// startPos����endPos�܂Œ��i�\���ǂ����𔻒肵�܂��B
		/// </summary>
		/// <param name="startPos">�J�n�ʒu</param>
		/// <param name="endPos">�ړI�n</param>
		/// <param name="nodes">�m�[�h</param>
		/// <param name="straightDis">���i�����Ƃ��ɒʉ߂���m�[�h����i�s�s�\�m�[�h�̋������v�Z���Ē��i�\�����肷��Ƃ��Ɏg�p���鋗��</param>
		/// <param name="startToEnd">startPos����endPos�ւ̃x�N�g��</param>
		/// <returns></returns>
		static bool CheckStraightMove
		(
			const Vector3& startPos,
			const Vector3& endPos,
			std::vector<std::vector<std::vector<AStarNode>>>& nodes,
			float straightDis,
			Vector3& startToEnd
		);

		/// <summary>
		/// �n���ꂽ�f�[�^�����ƂɍŒZ�o�H���v�Z���܂��B
		/// </summary>
		/// <param name="startPos">�X�^�[�g�n�_�̍��W</param>
		/// <param name="endPos">�S�[���n�_�̍��W</param>
		/// <param name="nodes">�m�[�h��vector(SetAStarNodeHitObjectNodeFlag�ɓn������̔z��)</param>
		/// <param name="routeVector">�S�[���܂ł̃��[�g���i�[����vector(size��0�ł悢)</param>
		/// <returns>�T���������������ǂ���</returns>
		static bool CalcRoute
		(
			const Vector3& startPos,
			const Vector3& endPos,
			std::vector<std::vector<std::vector<AStarNode>>>& nodes,
			std::vector<Vector3>* routeVector,
			std::vector<Vector3>* routeNodePos
		);
#pragma endregion


	};
}

