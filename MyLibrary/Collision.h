#pragma once
#include"CollisionDetectionData.h"
#include"Values.h"

//�v�Z�Ɏg�����ƁA�v�Z�ɂ���ē���ꂽ���̍\���̕�����?
//�܂Ƃ߂��const�œn���Ȃ����A���p�҂��ǂ���͂���΂����̂��ς��ƌ��킩��ɂ���

namespace MelLib
{
	//�����蔻��(�̌v�Z)�N���X
	class Collision
	{
	private:

		static Vector3 CalcHitBoxSurfaceNormal(BoxHitDirection dir);
		
		static Vector3 CalcSphereArea(const Vector3& spherePos, const Value3<Vector3>& triPos);


	public:

#pragma region 2D

		/// <summary>
		/// �l�p�`���m���������Ă��邩���m�F���܂��B
		/// </summary>
		/// <param name="rect1">�l�p�`</param>
		/// <param name="rect2">�l�p�`</param>
		/// <returns>�������Ă�����true</returns>
		static bool RectAndRect(const RectData& rect1, const RectData& rect2);

		/// <summary>
		/// �~���m���������Ă��邩���m�F���܂��B
		/// </summary>
		/// <param name="circle1">�~</param>
		/// <param name="circle2">�~</param>
		/// <returns>�������Ă�����true</returns>
		static bool CircleAndCircle
		(
			const CircleData& circle1,
			const CircleData& circle2
		);

		static bool PointAndCircle
		(
			const Vector2& pointPos,
			const CircleData& circle
		);

		/// <summary>
		/// �_�Ɛ�`���������Ă��邩���m�F���܂��B
		/// </summary>
		/// <param name="pointPos"></param>
		/// <param name="circularSector"></param>
		/// <returns></returns>
		static bool PointAndCircularSector
		(
			const Vector2& pointPos,
			const CircularSectorData& circularSector
		);

		static bool RectAndCircle(const RectData& rect, const CircleData& circle);

		/// <summary>
		/// �~��2D�̐������������Ă��邩���m�F���܂��B
		/// </summary>
		/// <param name="circle">�~</param>
		/// <param name="lineSegment">2D�̐���</param>
		/// <returns>�������Ă�����true</returns>
		static bool CircleAndSegment2D
		(
			const CircleData& circle,
			CircleCalcResult* circleCalcResult,
			const Segment2DData& lineSegment,
			Segment2DCalcResult* lineSegmentCalcResult
		);


#pragma endregion

#pragma region 3D
		// ����And��������Ȃ��āA����HitCheck�֐����I�[�o�[���[�h�����ق�������

		/// <summary>
		/// �����m���������Ă��邩���m�F���܂��B
		/// </summary>
		/// <param name="sphere1">��</param>
		/// <param name="sphere2">��</param>
		/// <returns>�������Ă�����true</returns>
		static bool SphereAndSphere(const SphereData& sphere1, const SphereData& sphere2);

		/// <summary>
		/// �����m���������Ă��邩���m�F���܂��B
		/// </summary>
		/// <param name="box1">��</param>
		/// <param name="boxCalcResult1">�v�Z���ʂ�����ϐ��̃|�C���^</param>
		/// <param name="box2">��</param>
		/// <param name="boxCalcResult2">�v�Z���ʂ�����ϐ��̃|�C���^</param>
		/// <returns>�������Ă�����true</returns>
		static bool BoxAndBox(const BoxData& box1, BoxCalcResult* boxCalcResult1, const BoxData& box2, BoxCalcResult* boxCalcResult2);

		static bool Segment3DAndSegment3D
		(
			const Segment3DData& lineSegment1,
			Segment3DCalcResult* lineSegment1CalcResult,
			const Segment3DData& lineSegment2,
			Segment3DCalcResult* lineSegment2CalcResult
		);

		/// <summary>
		/// �J�v�Z�����m���������Ă��邩���m�F���܂��B
		/// </summary>
		/// <param name="capsule1">�J�v�Z��</param>
		/// <param name="capsule2">�J�v�Z��</param>
		/// <returns>�������Ă�����true</returns>
		static bool CapsuleAndCapsule(const CapsuleData& capsule1, const CapsuleData& capsule2);

		/// <summary>
		/// ���Ɣ����������Ă��邩���m�F���܂��B
		/// </summary>
		/// <param name="sphere">��</param>
		/// <param name="sphereCalcResult">�v�Z���ʂ�����ϐ��̃|�C���^</param>
		/// <param name="box">��</param>
		/// <param name="boxCalcResult">�v�Z���ʂ�����ϐ��̃|�C���^</param>
		/// <returns>�������Ă�����true</returns>
		static bool SphereAndBox(const SphereData& sphere, SphereCalcResult* sphereCalcResult, const BoxData& box, BoxCalcResult* boxCalcResult);

		static bool SphereAndOBB(const SphereData& sphere, SphereCalcResult* sphereCalcResult, const OBBData& obb);

		static bool SphereAndRay(const SphereData& sphere, SphereCalcResult* sphereCalcResult, const RayData& ray , RayCalcResult* rayResult);

		static bool SphereAndSegmrnt3D(const SphereData& sphere, const Segment3DData& lineSegment);

		static bool SphereAndCapsule(const SphereData& sphere, const CapsuleData& capsule);

		static bool SphereAndTriangle(const SphereData& sphere, const TriangleData& triangle);

		static bool PlaneAndSegment3D(const PlaneData& plane, const Segment3DData& segment,Segment3DCalcResult* segmentResult);

		/// <summary>
		/// ��3D�̐������������Ă��邩���m�F���܂��B
		/// </summary>
		/// <param name="board">��</param>
		/// <param name="boardCalcResult">�v�Z���ʂ�����ϐ��̃|�C���^</param>
		/// <param name="lineSegment">3D�̐���</param>
		/// <param name="lineSegmentCalcResult">�v�Z���ʂ�����ϐ��̃|�C���^</param>
		/// <returns>�������Ă�����true</returns>
		static bool BoardAndSegment3D
		(
			const BoardData& board,
			BoardCalcResult* boardCalcResult,
			const Segment3DData& lineSegment,
			Segment3DCalcResult* lineSegmentCalcResult
		);

		static bool BoardAndCapsule
		(
			const BoardData& board,
			BoardCalcResult* boardCalcResult,
			const CapsuleData& capsule,
			Segment3DCalcResult* lineSegmentCalcResult
		);

		static bool PointAndSphere
		(
			const Vector3& pointPos, 
			const SphereData& sphere
		);

	/*	static bool PointAndCircularSector3D
		(
			const Vector3& pointPos,
		    const CircularSector3DData& circularSector
		);*/

		static bool PointAndFrustum
		(
			const Vector3& pointPos,
			const FrustumData& frustum
		);

		static bool BoxAndRay
		(
			const BoxData& box,
			const RayData& ray,
			RayCalcResult* rayResult
		);

		static bool BoxAndSegment3D
		(
			const BoxData& box,
			const Segment3DData& segment
		);

		static bool PointAndSegment3D
		(
			const Vector3& pointPos,
			const Segment3DData& segment
		);

		static bool TriangleAndSegment3D
		(
			const TriangleData& triangle,
			TriangleCalcResult* triangleCalcResult,
			const Segment3DData& lineSegment,
			Segment3DCalcResult* lineSegmentCalcResult
		);

		static bool OBBAndRay
		(
			const OBBData& obb,
			const RayData& ray,
			RayCalcResult* rayResult
		);
#pragma endregion
	};

}