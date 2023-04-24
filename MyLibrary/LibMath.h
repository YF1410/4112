#pragma once
#include"Vector.h"
#include"Matrix.h"
#include"Quaternion.h"
#define _USE_MATH_DEFINES
#include<math.h>
#include"Library.h"
#include"CollisionDetectionData.h"
//Game��Math�N���X�����������疼�O��Math�ɂ���ƃG���[���o��
//Math�ӊO���ƃG���[�o�Ȃ������̂ŁAMath�Ƃ������O���̂������Ȃ��̂�������Ȃ�
//�G���[�o�Ȃ����Ƃ�����
//include����Ɗm���ɃG���[���o��Binclude���Ȃ��Əo�Ȃ�
//lib(�X�^�e�B�b�N���C�u�����̃v���W�F�N�g)�ɍ��Ȃ���΃G���[�o�Ȃ�?

namespace MelLib
{


	class LibMath
	{
	private:
		LibMath() {}
		~LibMath() {}
	public:

		/// <summary>
		/// �~�������擾���܂�
		/// </summary>
		/// <returns></returns>
		static float GetFloatPI();

		/// <summary>
		/// �~�������擾���܂�
		/// </summary>
		/// <returns></returns>
		static double GetDoublePI();

		/// <summary>
		/// �K��
		/// </summary>
		/// <param name="num"></param>
		/// <returns></returns>
		static int Factorial(const int num);

		/// <summary>
		/// �ݏ�
		/// </summary>
		/// <param name="num"></param>
		/// <param name="index"></param>
		/// <returns></returns>
		static int Pow(const float num, const int index);

		enum MultipleClampType
		{
			CLAMP_TYPE_BIG,//�傫���l��Ԃ�
			CLAMP_TYPE_SMALL,//�������l��Ԃ�
			CLAMP_TYPE_NEAR_BIG,//�傫���l�Ə������l�̂����߂��ق���Ԃ��A���������������ꍇ�A�傫���ق���Ԃ��܂�
			CLAMP_TYPE_NEAR_SMALL,//�傫���l�Ə������l�̂����߂��ق���Ԃ��A���������������ꍇ�A�������ق���Ԃ��܂�
		};

		/// <summary>
		/// multiple�̔{����num���N�����v���܂��B�Ԃ��l��MultipleClampType�ŃZ�b�g���܂��B
		/// </summary>
		/// <param name="num">���l</param>
		/// <param name="multiple">�{��</param>
		/// <param name="type">�Ԃ��l�̎��</param>
		/// <returns></returns>
		static float MultipleClamp
		(
			const float num,
			const float multiple,
			const MultipleClampType type
		);

		

		/// <summary>
		/// num1��num2�̒l�̍������߁A������̒l�ȓ���������true��Ԃ��܂�
		/// </summary>
		/// <param name="num1">�l1</param>
		/// <param name="num2">�l2</param>
		/// <param name="difference">��̒l</param>
		/// <returns></returns>
		static bool Difference(const float num1, const float num2, const float difference);


		/// <summary>
		/// angle1��angle2�̒l�̍������߁A������̒l��菬����������true��Ԃ��܂��B
		/// </summary>
		/// <param name="angle1">�p�x</param>
		/// <param name="angle2">�p�x</param>
		/// <param name="difference"></param>
		/// <returns></returns>
		static bool AngleDifference(const float angle1, const float angle2, const float difference);

		/// <summary>
			/// �x���@�̊p�x�����W�A���Ɂ@���W�A����x���@�̊p�x��
			/// </summary>
			/// <param name="pattern">0 �x���@�̊p�x�����W�A����   1 ���W�A����x���@�̊p�x��</param>
			/// <param name="angle">�p�x</param>
		static float AngleConversion(int pattern, float angle);

		/// <summary>
		/// ���E������s���܂��B�_���x�N�g�����E�̏ꍇ��1�A���̏ꍇ��-1�A�x�N�g����̏ꍇ��0��Ԃ��܂��B
		/// </summary>
		/// <param name="vector"></param>
		/// <param name="point"></param>
		/// <returns></returns>
		static char PointLeftRightCheck(const Vector2& vector, const Vector2& point);

		/// <summary>
		/// �\��������s���܂��B�_�����ʂ̕\����������1�A������������-1�A���ʏゾ������0��Ԃ��܂��B
		/// </summary>
		/// <param name="point">�_</param>
		/// <param name="plane">���ʏ��</param>
		/// <returns></returns>
		static char PointPlaneFrontBackCheck(const Vector3& point, const PlaneData& plane);

		/// <summary>
		/// �\��������s���܂��B�_���̕\����������1�A������������-1�A���ʏゾ������0��Ԃ��܂��B
		/// </summary>
		/// <param name="point">�_</param>
		/// <param name="plane">���</param>
		/// <returns></returns>
		static char PointBoardFrontBackCheck(const Vector3& point, const BoardData& board);

		//���̕�Vector�N���X�Ɉڂ�?
#pragma region �x�N�g��


#pragma region vector2


		/// <summary>
		/// 2�̍��W�̋������擾���܂�
		/// </summary>
		/// <param name="pos1">���W1</param>
		/// <param name="pos2">���W2</param>
		/// <returns></returns>
		static float CalcDistance2D(const Vector2& pos1, const Vector2& pos2);

		/// <summary>
		/// 2�̍��W�̊Ԃ̍��W�����߂܂��B
		/// </summary>
		/// <param name="pos1"></param>
		/// <param name="pos2"></param>
		/// <returns></returns>
		static Vector2 CalcCenterPosition2D(const Vector2& pos1, const Vector2& pos2);

		/// <summary>
		/// vec1����vec2�̐��K�������x�N�g����Ԃ��܂��B
		/// </summary>
		/// <param name="vec1"></param>
		/// <param name="vec2"></param>
		/// <returns></returns>
		static Vector2 OtherVector2(const Vector2& vec1, const Vector2& vec2);


		/// <summary>
		/// 2�̃x�N�g�����Ȃ��p�x�����߂܂�
		/// </summary>
		/// <param name="v1"></param>
		/// <param name="v2"></param>
		/// <returns></returns>
		static float TwoVector2Angle(const Vector2& v1, const Vector2& v2);

		/// <summary>
		/// �x�N�g�����p�x�ɕϊ����܂�
		/// </summary>
		/// <param name="v"></param>
		/// <param name="v3">3�������W�n���ǂ���</param>
		/// <returns></returns>
		static float Vector2ToAngle(const Vector2& v, const bool v3);

		/// <summary>
		/// �p�x��Vector2�ɕϊ����܂�
		/// </summary>
		/// <param name="angle"></param>
		/// <param name="v3">3�������W�n���ǂ���</param>
		/// <returns></returns>
		static Vector2 AngleToVector2(const float& angle, const bool v3);

		/// <summary>
		/// Vector2�̃x�N�g�����w�肵���p�x�ɉ����ĉ�]�����܂�
		/// </summary>
		/// <param name="v"></param>
		/// <param name="angle"></param>
		/// <returns></returns>
		static Vector2 RotateVector2(const Vector2& v, const float angle);

		/// <summary>
		/// �ϐ�v�ƁA�����90�x�A180�x�A270�x��]���������W�Ō`�����ꂽ�l�p�`��ŉ�]���������W��Ԃ��܂��B
		/// </summary>
		/// <param name="v"></param>
		/// <param name="angle"></param>
		/// <returns></returns>
		static Vector2 RotateVector2Box(const Vector2& v, const float angle);

#pragma endregion

#pragma region Vector3

		/// <summary>
		/// �����̍��W3����@���x�N�g�������߂܂��Bpos1����pos3�����ԂɒH�����Ƃ��ɁA�E����W�n�̏ꍇ�͉E���A������W�n�̏ꍇ�͍����ɂȂ�悤�ɂ��Ă��������B
		/// </summary>
		/// <param name="pos1"></param>
		/// <param name="pos2"></param>
		/// <param name="pos3"></param>
		/// <returns></returns>
		static Vector3 CalcNormal(const Vector3& pos1, const Vector3& pos2, const Vector3& pos3);

		/// <summary>
		/// 2�̍��W�̋������擾���܂�
		/// </summary>
		/// <param name="pos1">���W1</param>
		/// <param name="pos2">���W2</param>
		/// <returns></returns>
		static float CalcDistance3D(const Vector3& pos1, const Vector3& pos2);

		/// <summary>
		/// 2�̍��W�̒��S���W���擾���܂��B
		/// </summary>
		/// <param name="pos1"></param>
		/// <param name="pos2"></param>
		/// <returns></returns>
		static Vector3 CalcCenterPosition3D(const Vector3& pos1, const Vector3& pos2);

		/// <summary>
		/// vex1����vec2�̃x�N�g�����擾���܂�(���K���͍s��)
		/// </summary>
		/// <param name="myPos"></param>
		/// <param name="otherPos"></param>
		/// <returns></returns>
		static Vector3 OtherVector3(const Vector3& vec1, const Vector3& vec2);

		/// <summary>
		/// �x�N�g������]�����܂�
		/// </summary>
		/// <param name="rotateV"></param>
		/// <param name="vec">���̃x�N�g��</param>
		/// <param name="angle"></param>
		/// <returns></returns>
		static Vector3 RotateVector3(const Vector3& rotateV, const Vector3& vec, const float angle);

		static Vector3 RotateZXYVector3(const Vector3& rotateV, const Vector3& angle);

		/// <summary>
		/// ���W���w�肵���x�N�g��������distance���ړ�������Vector3�^�̒l��Ԃ��܂��B
		/// </summary>
		/// <param name="pos"></param>
		/// <param name="vector"></param>
		/// <param name="distance"></param>
		/// <returns></returns>
		static Vector3 FloatDistanceMoveVector3(const Vector3& pos, const Vector3& vector, const float distance)
		{
			return pos + vector * distance;
		}
#pragma endregion


#pragma endregion

#pragma region �����蔻��

#pragma region 2D

		/// <summary>
		/// �l�p�̓����蔻��ł��B���W������̏ꍇ�̂ݐ������v�Z�ł��܂��B
		/// </summary>
		/// <param name="start1">1�ڂ̍�����W</param>
		/// <param name="size1">1�ڂ̃T�C�Y</param>
		/// <param name="start2">2�ڂ̍�����W</param>
		/// <param name="size2">2�ڂ̃T�C�Y</param>
		/// <returns>�����������ǂ���</returns>
		static bool RectAndRectCollision
		(
			const Vector2& pos1,
			const Vector2& size1,
			const Vector2& pos2,
			const Vector2& size2
		);

		/// <summary>
		/// �~�̓����蔻��ł��B
		/// </summary>
		/// <param name="centerPosition1">1�ڂ̍��W</param>
		/// <param name="r1">1�ڂ̔��a</param>
		/// <param name="centerPosition2">1�ڂ̍��W</param>
		/// <param name="r2">1�ڂ̉~</param>
		/// <returns>�����������ǂ���</returns>
		static bool CircleAndCircleCollision
		(
			const Vector2& centerPosition1,
			const float r1,
			const Vector2& centerPosition2,
			const float r2
		);



		/// <summary>
		/// �~�Ɛ����̔���
		/// </summary>
		/// <param name="spherePos"></param>
		/// <param name="r"></param>
		/// <param name="linePos1"></param>
		/// <param name="linePos2"></param>
		/// <param name="nearPos">�~�Ƃ̍ŋߓ_</param>
		/// <param name="lsHit">���̂ǂ��ɓ���������</param>
		/// <returns></returns>
		static bool CircleAndLineSegmentCollision
		(
			const Vector2& spherePos,
			const float r,
			const Vector2& linePos1,
			const Vector2& linePos2,
			Vector2* nearPos,
			SegmentHitPlace* isHit
		);

#pragma endregion

#pragma region 3D
		/// <summary>
		/// �����m�̓����蔻��
		/// </summary>
		/// <param name="centerPosition1">1�ڂ̍��W</param>
		/// <param name="r1">1�ڂ̔��a</param>
		/// <param name="centerPosition2">1�ڂ̍��W</param>
		/// <param name="r2">1�ڂ̉~</param>
		/// <returns>�����������ǂ���</returns>
		static bool SphereAndSphereCollision
		(
			const Vector3& centerPosition1,
			const float r1,
			const Vector3& centerPosition2,
			const float r2
		);

		/// <summary>
		/// ���ƕ��ʂ̍��W
		/// </summary>
		/// <param name="spherePos"></param>
		/// <param name="r"></param>
		/// <param name="normal">���ʂ̖@���x�N�g��</param>
		/// <param name="planeDist">���ʂ̌��_���畽�ʂ̋���(�}�C�i�X�\)</param>
		/// <param name="hitPos"></param>
		/// <returns></returns>
		static bool SphereAndPlaneCollision
		(
			const Vector3& spherePos,
			const float r,
			const Vector3& normal,
			const float planeDist,
			Vector3* hitPos
		);

		/// <summary>
		/// ���ƕ���ȎO�p�`�̔���
		/// </summary>
		/// <param name="spherePos"></param>
		/// <param name="r"></param>
		/// <param name="triPos1"></param>
		/// <param name="triPos2"></param>
		/// <param name="triPos3"></param>
		/// <param name="normal"></param>
		/// <param name="hitPos"></param>
		/// <returns></returns>
		static bool SphereAndTryangleCollision
		(
			const Vector3& spherePos,
			const float r,
			const Vector3& triPos1,
			const Vector3& triPos2,
			const Vector3& triPos3,
			const Vector3& normal,
			Vector3* hitPos
		);

		static bool SphereAndLineSegmentCollision
		(
			const Vector3& spherePos,
			const float r,
			const Vector3& linePos1,
			const Vector3& linePos2
		);

		/// <summary>
		/// ���Ɨ�����(AABB)�̔���
		/// </summary>
		/// <param name="spherePos"></param>
		/// <param name="r"></param>
		/// <param name="minPos"></param>
		/// <param name="maxPos"></param>
		/// <param name="direction">���́A���̂ǂ��ɓ����������B���͋��������̂ǂ��ɂԂ���������Ԃ�</param>
		/// <returns></returns>
		static bool SphereAndBoxCollision
		(
			const Vector3& spherePos,
			const float& r,
			const Vector3& centerPos,
			const Vector3& size,
			BoxHitDirection* direction
		);

		/// <summary>
		/// ������(AABB)���m�̔���
		/// </summary>
		/// <param name="centerPos1"></param>
		/// <param name="size1"></param>
		/// <param name="centerPos2"></param>
		/// <param name="size2"></param>
		/// <param name="direction1">�����̂ǂ����Ԃ���������Ԃ�</param>
		/// <param name="direction2">�����̂ǂ����Ԃ���������Ԃ�</param>
		/// <returns></returns>
		static bool BoxAndBoxCollision
		(
			const Vector3& centerPos1,
			const Vector3& size1,
			const Vector3& centerPos2,
			const Vector3& size2,
			BoxHitDirection* direction1,
			BoxHitDirection* direction2
		);

		/// <summary>
		/// �����Ɣ̓����蔻��
		/// </summary>
		/// <param name="linePos1">��1</param>
		/// <param name="linePos2">��2</param>
		/// <param name="normal">�̖@���x�N�g��</param>
		/// <param name="pointPos">�̍��W</param>
		/// <param name="vertexPoint">���_�̍��W4��(��]�ȂǓK����)(��]�O�̍����A����A�E���A�E��̏��Ŕz��ɓ����)</param>
		/// <param name="crossPos">���ƃ|���S�����Ԃ������ꏊ</param>
		/// /// <returns></returns>
		static bool LineSegmentAndBoardCollision
		(
			const Vector3& linePos1,
			const Vector3& linePos2,
			const Vector3& normal,
			const Vector3& pointPos,
			const std::vector<Vector3>& vertexPoint,
			Vector3* crossPosition
		);

		/// <summary>
		/// ������(���C)�ƕ��ʂ̔���
		/// </summary>
		/// <param name="layStartPos"></param>
		/// <param name="layDirection"></param>
		/// <param name="normal"></param>
		/// <param name="planeDist"></param>
		/// <param name="distance"></param>
		/// <param name="crossPos"></param>
		/// <returns></returns>
		static bool RayAndPlaneCollision
		(
			const Vector3& layStartPos,
			const Vector3& layDirection,
			const Vector3& normal,
			const float planeDistance,
			float* distance,
			Vector3* crossPos
		);

		/// <summary>
		/// ������(���C)�ƕ���ȎO�p�`�̔���B
		/// </summary>
		/// <param name="layStartPos"></param>
		/// <param name="layDirection"></param>
		/// <param name="triPos1"></param>
		/// <param name="triPos2"></param>
		/// <param name="triPos3"></param>
		/// <param name="normal"></param>
		/// <param name="distance"></param>
		/// <param name="crossPos"></param>
		/// <returns></returns>
		static bool RayAndTryangleCollision
		(
			const Vector3& layStartPos,
			const Vector3& layDirection,
			const Vector3& triPos1,
			const Vector3& triPos2,
			const Vector3& triPos3,
			const Vector3& normal,
			float* distance,
			Vector3* crossPos
		);

		/// <summary>
		/// �������Ƌ��̓����蔻��
		/// </summary>
		static bool RayAndSphereCollision
		(
			const Vector3& layStartPos,
			const Vector3& layDirection,
			const Vector3& spherePos,
			const float r,
			float* distance,
			Vector3* crossPos
		);


#pragma endregion

#pragma endregion


	};

}