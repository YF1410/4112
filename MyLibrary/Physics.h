#pragma once
#include"Vector.h"
#include"Values.h"

//�����W��float�ł��悳������������float�ɕς���

// ���ꂾ�Ɗ֐������Ȃ�̗ʂɂȂ邩��A���ڂ��Ƃɕ������ق�����������

namespace MelLib
{
	//�������Z�N���X
	class Physics
	{
	public:

		//�d�͉����x(����)
		static const float GRAVITATIONAL_ACCELERATION_MERCURY;
		//�d�͉����x(����)
		static const float GRAVITATIONAL_ACCELERATION_VENUS;
		//�d�͉����x(�n��)
		static const float GRAVITATIONAL_ACCELERATION_EARTH;
		//�d�͉����x(�ΐ�)
		static const float GRAVITATIONAL_ACCELERATION_MARS;
		//�d�͉����x(�ؐ�)
		static const float GRAVITATIONAL_ACCELERATION_JUPITER;
		//�d�͉����x(�y��)
		static const float GRAVITATIONAL_ACCELERATION_SATURN;
		//�d�͉����x(�V����)
		static const float GRAVITATIONAL_ACCELERATION_URANUS;
		//�d�͉����x(�C����)
		static const float GRAVITATIONAL_ACCELERATION_NEPTUNE;
		//�d�͉����x(������)
		static const float GRAVITATIONAL_ACCELERATION_PLUTO;
		//�d�͉����x(��)
		static const float GRAVITATIONAL_ACCELERATION_MOON;


		//�v�Z���łɂ��̂܂܍��W�ɉ��Z�ł���悤�ɂ��邽�߂�return���Ă�B
		//�v�Z���e�͏����ǉ����Ă���
		/// <summary>
		/// �ړ��Ɋւ���v�Z���s���܂��B
		/// </summary>
		/// <param name="vel">���x(�v�Z��v�Z�������x�ɏ��������܂�)</param>
		/// <param name="acc">�����x(�v�Z��v�Z���������x�ɏ��������܂�)</param>
		/// <param name="force">��</param>
		/// <param name="mass">�d��(kg)</param>
		/// <param name="graviAcc">�d�ʉ����x</param>
		/// <param name=""></param>
		/// <returns>���x</returns>
		static Vector3 CalcMoveResult
		(
			Vector3& vel,
			Vector3& acc,
			const Vector3& force,
			const float mass
		);

		/// <summary>
		/// ���R�����̑��x���v�Z���܂��B
		/// </summary>
		/// <param name="startVel">�����x</param>
		/// <param name="gravAcc">�d�͉����x�x</param>
		/// <param name="t">����(�b)</param>
		/// <returns></returns>
		static float CalcFallVelocity
		(
			const float startVel,
			const float gravAcc,
			const float t
		);

		/// <summary>
		/// ���̂��Փ˂����Ƃ���velocity��Ԃ��܂��B
		/// </summary>
		/// <param name="position">���W</param>
		/// <param name="velocity">���݂̑��x</param>
		/// <param name="mass">�d��</param>
		/// <param name="e">�����W��(0����1)</param>
		/// <returns>�v�Z���ʁB</returns>
		static Value2<Vector3> CalcRepulsionVelocity
		(
			const Value2<Vector3>& position,
			const Value2<Vector3>& velocity,
			const Value2<float>& mass,
			const Value2<Vector3>& e
		);


		/// <summary>
		/// �΂˒萔���v�Z���܂��B
		/// </summary>
		/// <param name="modulesOfRigidity">���e���W��(N/mm2�܂���MPa)</param>
		/// <param name="linear">���a(mm)</param>
		/// <param name="turnsNum">�L��������</param>
		/// <param name="averageCoilDiameter">���σR�C���a(mm)</param>
		/// <returns>�΂˒萔</returns>
		static float CalcSpringConstant
		(
			const float modulesOfRigidity,
			const float linear,
			const float turnsNum,
			const float	averageCoilDiameter
		)
		{
			return
				(modulesOfRigidity * (linear * linear * linear * linear))
				/ (8 * turnsNum * (averageCoilDiameter * averageCoilDiameter * averageCoilDiameter));
		}

		/// <summary>
		/// rootPos�ɕ��̂��΂˂łȂ������̑��x���v�Z���܂��B
		/// </summary>
		/// <param name="currentPos">���݂̍��W</param>
		/// <param name="currentVel">���݂̑��x</param>
		/// <param name="prePos">���̂��Ȃ����Ă���ꏊ�̍��W</param>
		/// <param name="naturalDis">���R��</param>
		/*/// <param name="maxDis">�ő勗��</param>*/
		/// <param name="mass">���̂̏d��</param>
		/// <param name="gravitationalAcceleration">�d�͉����x(���̃N���X�Œ�`����Ă���萔�܂��͎����ōl�����l)</param>
		/// <param name="springConstant">�΂˒萔</param>
		/// <param name="viscousDragCoefficient">�S����R�W��</param>
		/// <returns>���x</returns>
		static Vector3 CalcSpringVelocity
		(
			const Vector3& currentPos,
			const Vector3& currentVel,
			const Vector3* prePos,
			const Vector3* nextPos,
			const float naturalDis,
			/*const float maxDis,*/
			const float mass,
			const float gravitationalAcceleration,
			const float springConstant,
			const float viscousDragCoefficient
		);


	};

}