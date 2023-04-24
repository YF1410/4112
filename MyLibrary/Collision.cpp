#include "Collision.h"
#include"LibMath.h"
#include"Values.h"

using namespace MelLib;

Vector3 MelLib::Collision::CalcHitBoxSurfaceNormal(BoxHitDirection dir)
{
	MelLib::Vector3 normal;
	switch (dir)
	{
	case MelLib::BoxHitDirection::BOX_HIT_DIRECTION_NO_HIT:
		break;
	case MelLib::BoxHitDirection::BOX_HIT_DIRECTION_UP:
		normal = MelLib::Vector3(0, 1, 0);
		break;
	case MelLib::BoxHitDirection::BOX_HIT_DIRECTION_DOWN:
		normal = MelLib::Vector3(0, -1, 0);
		break;
	case MelLib::BoxHitDirection::BOX_HIT_DIRECTION_LEFT:
		normal = MelLib::Vector3(-1, 0, 0);
		break;
	case MelLib::BoxHitDirection::BOX_HIT_DIRECTION_RIGHT:
		normal = MelLib::Vector3(1, 0, 0);
		break;
	case MelLib::BoxHitDirection::BOX_HIT_DIRECTION_FRONT:
		normal = MelLib::Vector3(0, 0, -1);
		break;
	case MelLib::BoxHitDirection::BOX_HIT_DIRECTION_BACK:
		normal = MelLib::Vector3(0, 0, 1);
		break;
	default:
		break;
	}
	return normal;
}

Vector3 MelLib::Collision::CalcSphereArea(const Vector3& spherePos, const Value3<Vector3>& triPos)
{
	Vector3 nearPos;

	Vector3 p1ToSphere = spherePos - triPos.v1;
	Vector3 p2ToSphere = spherePos - triPos.v2;
	Vector3 p3ToSphere = spherePos - triPos.v3;

	// 1
	Vector3 p1ToP2 = triPos.v2 - triPos.v1;
	Vector3 p1ToP3 = triPos.v3 - triPos.v1;

	// 1
	float p1P2DotP1Sphere = Vector3::Dot(p1ToP2, p1ToSphere);
	// 2
	float p1P3DotP1Sphere = Vector3::Dot(p1ToP3, p1ToSphere);
	// 3
	float p1P2DotP2Sphere = Vector3::Dot(p1ToP2, p2ToSphere);
	// 4
	float p1P3DotP2Sphere = Vector3::Dot(p1ToP3, p2ToSphere);
	// 5
	float p1P2DotP3Sphere = Vector3::Dot(p1ToP2, p3ToSphere);
	// 6
	float p1P3DotP3Sphere = Vector3::Dot(p1ToP3, p3ToSphere);


	if (p1P2DotP1Sphere < 0 && p1P3DotP1Sphere < 0)
	{
		nearPos = triPos.v1;
		return nearPos;
	}

	// 2
	Vector3 p2ToP1 = triPos.v1 - triPos.v2;
	Vector3 p2ToP3 = triPos.v3 - triPos.v2;
	if (p1P2DotP2Sphere >= 0 && p1P3DotP2Sphere < p1P2DotP2Sphere)
	{
		nearPos = triPos.v2;
		return nearPos;
	}

	// 4
	Vector3 p3ToP1 = triPos.v1 - triPos.v3;
	Vector3 p3ToP2 = triPos.v2 - triPos.v3;
	if (p1P3DotP3Sphere >= 0 && p1P2DotP3Sphere < p1P3DotP3Sphere)
	{
		nearPos = triPos.v3;
		return nearPos;
	}

	// 3
	float check3 =
		p1P2DotP1Sphere
		* p1P3DotP2Sphere
		- p1P2DotP2Sphere
		* p1P3DotP1Sphere;

	if (p1P2DotP1Sphere >= 0 && p1P2DotP2Sphere < 0)
	{
		if (check3 <= 0.0f)
		{
			nearPos =
				triPos.v1
				+ p1P2DotP1Sphere
				/ (p1P2DotP1Sphere
				- p1P2DotP2Sphere)
				* p1ToP2;

			return nearPos;
		}
	}

	// 5
	float check5 =
		p1P2DotP3Sphere
		* p1P3DotP1Sphere
		- p1P2DotP1Sphere
		* p1P3DotP3Sphere;
	if (p1P3DotP1Sphere >= 0 && p1P3DotP3Sphere < 0)
	{
		if (check5 <= 0.0f)
		{
			nearPos =
				triPos.v1
				+ p1P3DotP1Sphere
				/ (p1P3DotP1Sphere
					- p1P3DotP3Sphere)
				* p1ToP3;
			return nearPos;
		}
	}

	// 6
	float check6 =
		p1P2DotP2Sphere
		* p1P3DotP3Sphere
		- p1P2DotP3Sphere
		* p1P3DotP2Sphere;
	if (p1P3DotP2Sphere - p1P2DotP2Sphere >= 0 && p1P2DotP3Sphere - p1P3DotP3Sphere >= 0)
	{
		if (check6 <= 0.0f)
		{
			float w = (p1P3DotP2Sphere - p1P2DotP2Sphere) / ((p1P3DotP2Sphere - p1P2DotP2Sphere) + (p1P2DotP3Sphere - p1P3DotP3Sphere));

			nearPos =
				triPos.v2
				+ w
				*( triPos.v3 - triPos.v2);
			return nearPos;
		}
	}

	// 7
	float denom = 1.0f / (check3 + check5 + check6);
	float v = check5 * denom;
	float w = check3 * denom;
	nearPos = triPos.v1 + p1ToP2 * v + p1ToP3 * w;
	return nearPos;
}


#pragma region 2D





bool Collision::RectAndRect(const RectData& rect1, const RectData& rect2)
{
	Vector2 pos1 = rect1.GetPosition();
	Vector2 pos2 = rect2.GetPosition();
	Vector2 size1 = rect1.GetSize();
	Vector2 size2 = rect2.GetSize();


	return size2.x + pos2.x > pos1.x &&
		pos2.x < size1.x + pos1.x &&
		pos2.y < size1.y + pos1.y &&
		size2.y + pos2.y > pos1.y;
}

bool Collision::CircleAndCircle(const CircleData& circle1, const CircleData& circle2)
{
	Vector2 pos1 = circle1.GetPosition();
	Vector2 pos2 = circle2.GetPosition();
	float r1 = circle1.GetRadius();
	float r2 = circle2.GetRadius();

	return (pos1.x - pos2.x) *
		(pos1.x - pos2.x) +
		(pos1.y - pos2.y) *
		(pos1.y - pos2.y) <=
		(r1 + r2) * (r1 + r2);
}

bool MelLib::Collision::PointAndCircle(const Vector2& pointPos, const CircleData& circle)
{
	Vector2 circlePos = circle.GetPosition();
	float circleR = circle.GetRadius();

	//���a�ȓ���������true
	return LibMath::CalcDistance2D(pointPos, circlePos) <= circleR;
}

bool MelLib::Collision::PointAndCircularSector(const Vector2& pointPos, const CircularSectorData& circularSector)
{
	//�_�Ɖ~�̔���
	if (!PointAndCircle(pointPos, circularSector.GetCircleData()))return false;

	Vector2 circlePos = circularSector.GetCircleData().GetPosition();
	float centerDirAngle = LibMath::Vector2ToAngle(circularSector.GetDirection(), false);
	float pointAngle = LibMath::Vector2ToAngle(LibMath::OtherVector2(circlePos, pointPos), false);
	float centerAngle = circularSector.GetAngle();

	//centerAngle�ȓ���������true
	return LibMath::AngleDifference(centerDirAngle, pointAngle, centerAngle / 2);
}

bool Collision::RectAndCircle(const RectData& rect, const CircleData& circle)
{
	return false;
}

bool Collision::CircleAndSegment2D
(
	const CircleData& circle,
	CircleCalcResult* circleCalcResult,
	const Segment2DData& lineSegment,
	Segment2DCalcResult* lineSegmentCalcResult
)
{
	SegmentHitPlace hitPlace = SegmentHitPlace::LS_HIT_POSITION_NOT_HIT;
	if (lineSegmentCalcResult)
	{
		lineSegmentCalcResult->hitPlace = hitPlace;
	}

	Value2<Vector2>segmentPos = lineSegment.GetPosition();
	Vector2 circlePos = circle.GetPosition();
	float circleR = circle.GetRadius();

	//���̒[����[�ւ̃x�N�g��
	Vector2 lineVector = segmentPos.v2 - segmentPos.v1;
	//���̍��W1����~�̒��S
	Vector2 linePos0ToSphere = circlePos - segmentPos.v1;
	//�~�Ɛ��̍ŒZ���������߂�
	float distance = abs(Vector2::Cross(lineVector, linePos0ToSphere) / Vector2::Length(lineVector));


	//�~�Ɛ����̍ŋߓ_�����߂�
	//����֐�������

	//linePos1����ŋߓ_�̋��������߂�
	float linePos0ToNearPosDis = Vector2::Dot(Vector2::Normalize(lineVector), linePos0ToSphere);
	if (lineSegmentCalcResult)
	{
		lineSegmentCalcResult->nearPos = segmentPos.v1 + (Vector2::Normalize(lineVector) * linePos0ToNearPosDis);
	}

	//�����̂ق����傫��������v�Z�I��
	if (distance > circleR)return false;

	//���̍��W2����~�̒��S
	Vector2 linePos2ToSphere = circlePos - segmentPos.v2;
	//���̐����ǂ����𔻕ʂ���t���O
	bool linePos1ToSpherePositive = false;
	bool linePos2ToSpherePositive = false;

	//���ς����߂�
	float linePos1ToSphereDot = Vector2::Dot(lineVector, linePos0ToSphere);
	float linePos2ToSphereDot = Vector2::Dot(lineVector, linePos2ToSphere);

	//���������m�F
	if (linePos1ToSphereDot >= 0)
		linePos1ToSpherePositive = true;
	if (linePos2ToSphereDot >= 0)
		linePos2ToSpherePositive = true;


	//�ǂ�������܂��͕������������
	if (linePos1ToSpherePositive
		&& linePos2ToSpherePositive
		|| !linePos1ToSpherePositive
		&& !linePos2ToSpherePositive)
	{
		//�傫�������߂�
		float linePos1ToSphereLength = Vector2::Length(linePos0ToSphere);
		float linePos2ToSphereLength = Vector2::Length(linePos2ToSphere);

		//���a���傫�������瓖�����ĂȂ�
		if (linePos1ToSphereLength > circleR
			&& linePos2ToSphereLength > circleR)
		{
			return false;
		}

		hitPlace = SegmentHitPlace::LS_HIT_POSITION_LE_START_END;
	}
	else
	{
		hitPlace = SegmentHitPlace::LS_HIT_POSITION_LE_LINE;
	}

	if (lineSegmentCalcResult)
	{
		lineSegmentCalcResult->hitPlace = hitPlace;
	}

	return true;
}


#pragma endregion

#pragma region 3D

bool Collision::SphereAndSphere(const SphereData& sphere1, const SphereData& sphere2)
{
	Vector3 sphere1Pos = sphere1.GetPosition();
	float sphere1R = sphere1.GetRadius();
	Vector3 sphere2Pos = sphere2.GetPosition();
	float sphere2R = sphere2.GetRadius();

	return (sphere2Pos.x - sphere1Pos.x) *
		(sphere2Pos.x - sphere1Pos.x) +
		(sphere2Pos.y - sphere1Pos.y) *
		(sphere2Pos.y - sphere1Pos.y) +
		(sphere2Pos.z - sphere1Pos.z) *
		(sphere2Pos.z - sphere1Pos.z) <=
		(sphere1R + sphere2R) * (sphere1R + sphere2R);
}

bool Collision::BoxAndBox
(
	const BoxData& box1,
	BoxCalcResult* boxCalcResult1,
	const BoxData& box2,
	BoxCalcResult* boxCalcResult2
)
{
	Vector3 box1Pos = box1.GetPosition();
	Vector3 box1Size = box1.GetSize();
	Vector3 box2Pos = box2.GetPosition();
	Vector3 box2Size = box2.GetSize();

	Vector3 minPos1 = box1Pos - box1Size / 2;
	Vector3 maxPos1 = box1Pos + box1Size / 2;
	Vector3 minPos2 = box2Pos - box2Size / 2;
	Vector3 maxPos2 = box2Pos + box2Size / 2;
	bool isHit = false;

	//1��2��min��max��XYZ�S������͈͓̔��������瓖�����Ă�
	if (minPos1.x >= minPos2.x &&
		minPos1.x < maxPos2.x ||
		maxPos1.x >= minPos2.x &&
		maxPos1.x < maxPos2.x ||
		minPos2.x >= minPos1.x &&
		minPos2.x < maxPos1.x ||
		maxPos2.x >= minPos1.x &&
		maxPos2.x < maxPos1.x)
	{

		if (minPos1.y >= minPos2.y &&
			minPos1.y < maxPos2.y ||
			maxPos1.y >= minPos2.y &&
			maxPos1.y < maxPos2.y ||
			minPos2.y >= minPos1.y &&
			minPos2.y < maxPos1.y ||
			maxPos2.y >= minPos1.y &&
			maxPos2.y < maxPos1.y)
		{
			if (minPos1.z >= minPos2.z &&
				minPos1.z < maxPos2.z ||
				maxPos1.z >= minPos2.z &&
				maxPos1.z < maxPos2.z ||
				minPos2.z >= minPos1.z &&
				minPos2.z < maxPos1.z ||
				maxPos2.z >= minPos1.z &&
				maxPos2.z < maxPos1.z)
			{
				isHit = true;
			}

		}
	}

	if (boxCalcResult1 || boxCalcResult2)
	{
		BoxHitDirection direction1 = BoxHitDirection::BOX_HIT_DIRECTION_NO_HIT;
		BoxHitDirection direction2 = BoxHitDirection::BOX_HIT_DIRECTION_NO_HIT;

		if (!isHit)
		{
			if (boxCalcResult1) boxCalcResult1->boxHitDistance = BoxHitDirection::BOX_HIT_DIRECTION_NO_HIT;
			if (boxCalcResult2) boxCalcResult2->boxHitDistance = BoxHitDirection::BOX_HIT_DIRECTION_NO_HIT;
			return false;
		}

		//1 X������
		//2 Y������
		//3 Z������
		char top = 0;
		//�Ώۂւ̃x�N�g��
		Vector3 targetToVector = box2Pos - box1Pos;

		if (abs(targetToVector.x) > abs(targetToVector.y) &&
			abs(targetToVector.x) > box2Size.x / 2)
		{
			top = 1;
			if (abs(targetToVector.z) > abs(targetToVector.x) &&
				abs(targetToVector.z) > box2Size.z / 2)
				top = 3;
		}
		else
		{
			top = 2;
			if (abs(targetToVector.z) > abs(targetToVector.y) &&
				abs(targetToVector.z) > box2Size.z / 2)
				top = 3;
		}


		if (top == 1)
		{
			if (targetToVector.x >= 0)
			{

				direction1 = BoxHitDirection::BOX_HIT_DIRECTION_RIGHT;
				direction2 = BoxHitDirection::BOX_HIT_DIRECTION_LEFT;
			}
			else
			{
				direction1 = BoxHitDirection::BOX_HIT_DIRECTION_LEFT;
				direction2 = BoxHitDirection::BOX_HIT_DIRECTION_RIGHT;
			}
		}
		else if (top == 2)
		{
			if (targetToVector.y >= 0)
			{
				direction1 = BoxHitDirection::BOX_HIT_DIRECTION_UP;
				direction1 = BoxHitDirection::BOX_HIT_DIRECTION_DOWN;
			}
			else
			{
				direction1 = BoxHitDirection::BOX_HIT_DIRECTION_DOWN;
				direction1 = BoxHitDirection::BOX_HIT_DIRECTION_UP;
			}
		}
		else if (top == 3)
		{
			if (targetToVector.z >= 0)
			{
				direction1 = BoxHitDirection::BOX_HIT_DIRECTION_BACK;
				direction2 = BoxHitDirection::BOX_HIT_DIRECTION_FRONT;
			}
			else
			{
				direction1 = BoxHitDirection::BOX_HIT_DIRECTION_FRONT;
				direction2 = BoxHitDirection::BOX_HIT_DIRECTION_BACK;
			}
		}

		if (boxCalcResult1) boxCalcResult1->boxHitDistance = direction1;
		if (boxCalcResult2) boxCalcResult2->boxHitDistance = direction2;
	}

	return isHit;
}

bool Collision::Segment3DAndSegment3D
(
	const Segment3DData& lineSegment1,
	Segment3DCalcResult* lineSegment1CalcResult,
	const Segment3DData& lineSegment2,
	Segment3DCalcResult* lineSegment2CalcResult
)
{
	Value2<Vector3>segmentPos1 = lineSegment1.GetRotatePosition();

	Value2<Vector3>segmentPos2 = lineSegment2.GetRotatePosition();


	Vector3 d1 = segmentPos1.v2 - segmentPos1.v1;
	Vector3 d2 = segmentPos2.v2 - segmentPos2.v1;
	//�J�v�Z���̏����̈ꕔ���g���΋��߂���?
	Vector3 r = segmentPos1.v1 - segmentPos2.v1;
	float a = Vector3::Dot(d1, d1);
	float b = Vector3::Dot(d1, d2);
	float c = Vector3::Dot(d1, r);
	float e = Vector3::Dot(d2, d2);
	float f = Vector3::Dot(d2, r);
	float d = a * e - b * b;

	float s = 0.0f;
	if (d != 0.0f)
	{
		s = (b * f - c * e) / d;

		if (s < 0.0f)s = 0.0f;
		if (s > 1.0f)s = 1.0f;
	}

	Vector3 p1 = segmentPos1.v1;
	Vector3 p2 = segmentPos2.v1;
	float t = Vector3::Dot(((p1 + d1 * s) - p2), d2) / Vector3::Dot(d2, d2);

	s = (t * b - c) / a;
	if (t < 0.0f)
	{
		t = 0.0f;
		s = -c / a;
	}
	else if (t > 1.0f)
	{
		t = 1.0f;
		s = (b - c) / a;
	}
	if (s < 0.0f)s = 0.0f;
	if (s > 1.0f)s = 1.0f;

	//����Ɉ�ԋ߂��ꏊ
	Vector3 c1 = segmentPos1.v1 + s * d1;
	Vector3 c2 = segmentPos2.v1 + t * d2;

	//�߂��ꏊ�̋�����0�������瓖�����Ă���(�d�Ȃ��Ă���A�������Ă���)
	bool isHit = LibMath::CalcDistance3D(c1, c2) == 0.0f;

	if (lineSegment1CalcResult)lineSegment1CalcResult->lineSegment3DHitPos = c1;
	if (lineSegment2CalcResult)lineSegment2CalcResult->lineSegment3DHitPos = c2;
	return isHit;

}

bool Collision::CapsuleAndCapsule(const CapsuleData& capsule1, const CapsuleData& capsule2)
{
	//��]�K��.
	Value2<Vector3>capsule1LineSegmentPos = capsule1.GetSegment3DData().GetRotatePosition();

	Value2<Vector3>capsule2LineSegmentPos = capsule2.GetSegment3DData().GetRotatePosition();


	Vector3 d1 = capsule1LineSegmentPos.v2 - capsule1LineSegmentPos.v1;
	Vector3 d2 = capsule2LineSegmentPos.v2 - capsule2LineSegmentPos.v1;

	Vector3 r = capsule1LineSegmentPos.v1 - capsule2LineSegmentPos.v1;
	float a = Vector3::Dot(d1, d1);
	float b = Vector3::Dot(d1, d2);
	float c = Vector3::Dot(d1, r);
	float e = Vector3::Dot(d2, d2);
	float f = Vector3::Dot(d2, r);
	float d = a * e - b * b;

	float s = 0.0f;
	if (d != 0.0f)
	{
		s = (b * f - c * e) / d;

		if (s < 0.0f)s = 0.0f;
		if (s > 1.0f)s = 1.0f;
	}

	Vector3 p1 = capsule1LineSegmentPos.v1;
	Vector3 p2 = capsule2LineSegmentPos.v1;
	float t = Vector3::Dot(((p1 + d1 * s) - p2), d2) / Vector3::Dot(d2, d2);

	s = (t * b - c) / a;
	if (t < 0.0f)
	{
		t = 0.0f;
		s = -c / a;
	}
	else if (t > 1.0f)
	{
		t = 1.0f;
		s = (b - c) / a;
	}
	if (s < 0.0f)s = 0.0f;
	if (s > 1.0f)s = 1.0f;

	//����J�v�Z���Ɉ�ԋ߂��ꏊ
	Vector3 c1 = p1 + s * d1;
	Vector3 c2 = p2 + t * d2;

	return LibMath::CalcDistance3D(c1, c2) < capsule1.GetRadius() + capsule2.GetRadius();
}

bool Collision::SphereAndBox
(
	const SphereData& sphere,
	SphereCalcResult* sphereCalcResult,
	const BoxData& box,
	BoxCalcResult* boxCalcResult
)
{
	float dir2 = 0.0f;

	Vector3 minPos = box.GetPosition() - box.GetSize() / 2;
	Vector3 maxPos = box.GetPosition() + box.GetSize() / 2;

	//x
	if (sphere.GetPosition().x < minPos.x)
		dir2 += (minPos.x - sphere.GetPosition().x) * (minPos.x - sphere.GetPosition().x);
	if (sphere.GetPosition().x > maxPos.x)
		dir2 += (sphere.GetPosition().x - maxPos.x) * (sphere.GetPosition().x - maxPos.x);

	//y
	if (sphere.GetPosition().y < minPos.y)
		dir2 += (minPos.y - sphere.GetPosition().y) * (minPos.y - sphere.GetPosition().y);
	if (sphere.GetPosition().y > maxPos.y)
		dir2 += (sphere.GetPosition().y - maxPos.y) * (sphere.GetPosition().y - maxPos.y);

	//z
	if (sphere.GetPosition().z < minPos.z)
		dir2 += (minPos.z - sphere.GetPosition().z) * (minPos.z - sphere.GetPosition().z);
	if (sphere.GetPosition().z > maxPos.z)
		dir2 += (sphere.GetPosition().z - maxPos.z) * (sphere.GetPosition().z - maxPos.z);

	bool isHit = dir2 < sphere.GetRadius()* sphere.GetRadius();

	//�ǂ��ɓ�����������Ԃ�
	if (sphereCalcResult || boxCalcResult)
	{
		//���̂ǂ̕����ɂԂ��������Ƃ������
		BoxHitDirection hitDirection = BoxHitDirection::BOX_HIT_DIRECTION_NO_HIT;
		if (!isHit)
		{
			sphereCalcResult->SetBoxHitDirection(hitDirection);
			boxCalcResult->boxHitDistance = hitDirection;
			return false;
		}


		//1 X������
		//2 Y������
		//3 Z������
		char top = 0;
		//�{�b�N�X�ւ̃x�N�g��
		Vector3 sphereToVector = box.GetPosition() - sphere.GetPosition();

		if (abs(sphereToVector.x) > abs(sphereToVector.y) &&
			abs(sphereToVector.x) > box.GetSize().x / 2)
		{
			top = 1;
			if (abs(sphereToVector.z) > abs(sphereToVector.x) &&
				abs(sphereToVector.z) > box.GetSize().z / 2)
				top = 3;
		}
		else
		{
			top = 2;
			if (abs(sphereToVector.z) > abs(sphereToVector.y) &&
				abs(sphereToVector.z) > box.GetSize().z / 2)
				top = 3;
		}

		if (top == 1)
		{
			if (sphereToVector.x >= 0)
			{
				hitDirection = BoxHitDirection::BOX_HIT_DIRECTION_LEFT;
			}
			else
			{
				hitDirection = BoxHitDirection::BOX_HIT_DIRECTION_RIGHT;
			}
		}
		else if (top == 2)
		{
			if (sphereToVector.y >= 0)
			{
				hitDirection = BoxHitDirection::BOX_HIT_DIRECTION_DOWN;
			}
			else
			{
				hitDirection = BoxHitDirection::BOX_HIT_DIRECTION_UP;
			}
		}
		else if (top == 3)
		{
			if (sphereToVector.z >= 0)
			{
				hitDirection = BoxHitDirection::BOX_HIT_DIRECTION_FRONT;
			}
			else
			{
				hitDirection = BoxHitDirection::BOX_HIT_DIRECTION_BACK;
			}
		}




		if (sphereCalcResult)
		{
			sphereCalcResult->SetBoxHitDirection(hitDirection);
			sphereCalcResult->SetBoxHitSurfaceNormal(CalcHitBoxSurfaceNormal(hitDirection));
		}
		if (boxCalcResult)boxCalcResult->boxHitDistance = hitDirection;
	}

	return isHit;
}

bool MelLib::Collision::SphereAndOBB(const SphereData& sphere, SphereCalcResult* sphereCalcResult, const OBBData& obb)
{

	// L
	Vector3 sizeHalf = obb.GetBoxData().GetSize() / 2;

	// ��]������3�{(XYZ)�����ɂ͂ݏo�Ă��邩�m�F����
	// XYZ�̎���OBB�̉�]��K��������
	// v1,v2,v3
	Vector3 angle = obb.GetAngle();
	Vector3 axisX = LibMath::RotateZXYVector3(Vector3(1, 0, 0), angle);
	Vector3 axisY = LibMath::RotateZXYVector3(Vector3(0, 1, 0), angle);
	Vector3 axisZ = LibMath::RotateZXYVector3(Vector3(0, 0, 1), angle);

	// GP
	// ���̒��S���狅�̒��S�ւ̃x�N�g��
	Vector3 boxToSphere = sphere.GetPosition() - obb.GetBoxData().GetPosition();

	float s1 = Vector3::Dot(boxToSphere, axisX) / sizeHalf.x;
	s1 = abs(s1);
	if (s1 < 1)s1 = 1;

	float s2 = Vector3::Dot(boxToSphere, axisY) / sizeHalf.y;
	s2 = abs(s2);
	if (s2 < 1)s2 = 1;

	float s3 = Vector3::Dot(boxToSphere, axisZ) / sizeHalf.z;
	s3 = abs(s3);
	if (s3 < 1)s3 = 1;

	float dis = 0.0f;
	dis = (
		(1 - s1) * sizeHalf.x * axisX
		+ (1 - s2)* sizeHalf.y * axisY
		+ (1 - s3) * sizeHalf.z * axisZ
		).Length();

	bool result = dis <= sphere.GetRadius();
	

	
	if (!sphereCalcResult || !result)return result;
	
	// �ȉ��@���v�Z
	
	// �@���擾�ǂ�����
	// �l�p�`�̊p�x��0,0,0�̎��ɖ߂��A���̈ʒu���߂�������]�B(����GetBoxData�Ŏ擾����΂����̂ŁA��]������K�v�͂Ȃ�)
	// Box�Ɠ����悤�ɋ��߂�
	// �@������]
	
	BoxHitDirection hitDirection = BoxHitDirection::BOX_HIT_DIRECTION_NO_HIT;
	
	BoxData box = obb.GetBoxData();
	//1 X������
	//2 Y������
	//3 Z������
	char top = 0;

	// �p�x�̃}�C�i�X����]
	// OBB����ɉ�]�����Ȃ��Ƃ����Ȃ�
	Vector3 rotSphere = LibMath::RotateZXYVector3(boxToSphere, -angle);
	rotSphere += box.GetPosition();

	//�{�b�N�X�ւ̃x�N�g��
	Vector3 sphereToVector = box.GetPosition() - rotSphere;

	if (abs(sphereToVector.x) > abs(sphereToVector.y) &&
		abs(sphereToVector.x) > box.GetSize().x / 2)
	{
		top = 1;
		if (abs(sphereToVector.z) > abs(sphereToVector.x) &&
			abs(sphereToVector.z) > box.GetSize().z / 2)
			top = 3;
	}
	else
	{
		top = 2;
		if (abs(sphereToVector.z) > abs(sphereToVector.y) &&
			abs(sphereToVector.z) > box.GetSize().z / 2)
			top = 3;
	}

	if (top == 1)
	{
		if (sphereToVector.x >= 0)
		{
			hitDirection = BoxHitDirection::BOX_HIT_DIRECTION_LEFT;
		}
		else
		{
			hitDirection = BoxHitDirection::BOX_HIT_DIRECTION_RIGHT;
		}
	}
	else if (top == 2)
	{
		if (sphereToVector.y >= 0)
		{
			hitDirection = BoxHitDirection::BOX_HIT_DIRECTION_DOWN;
		}
		else
		{
			hitDirection = BoxHitDirection::BOX_HIT_DIRECTION_UP;
		}
	}
	else if (top == 3)
	{
		if (sphereToVector.z >= 0)
		{
			hitDirection = BoxHitDirection::BOX_HIT_DIRECTION_FRONT;
		}
		else
		{
			hitDirection = BoxHitDirection::BOX_HIT_DIRECTION_BACK;
		}
	}

	
	// �@�������߂ăZ�b�g
	Vector3 normal = CalcHitBoxSurfaceNormal(hitDirection);
	sphereCalcResult->SetOBBHitSurfaceNormal(LibMath::RotateZXYVector3(normal, angle));
	
	return result;
}

bool MelLib::Collision::SphereAndRay(const SphereData& sphere, SphereCalcResult* sphereCalcResult, const RayData& ray, RayCalcResult* rayResult)
{
	Vector3 spherePos = sphere.GetPosition();
	float sphereR = sphere.GetRadius();

	Vector3 rayPos = ray.GetPosition();
	Vector3 rayDir = ray.GetDirection();

	Vector3 sphereToRay = rayPos - spherePos;
	
	float b = Vector3::Dot(sphereToRay, rayDir);
	float c = Vector3::Dot(sphereToRay, sphereToRay) - (sphereR * sphereR);

	float d = (b * b) - c;

	if (d < 0)return false;

	float t = -b - sqrt((b * b) - c);
	if (t < 0)return false;
	
	if (d > 0)
	{
		if (rayResult)
		{
			Vector3 hitPosition = rayPos + t * rayDir;
			rayResult->hitPosition = hitPosition;

			rayResult->hitObjectNormal = (hitPosition) / Vector3::Normalize(hitPosition);
		}
	}
	return true;
}

bool Collision::SphereAndCapsule(const SphereData& sphere, const CapsuleData& capsule)
{
	Value2<Vector3>capsuleLineSegmentPos = capsule.GetSegment3DData().GetRotatePosition();
	Vector3 spherePos = sphere.GetPosition();

	Vector3 capsulePos0ToSphere = spherePos - capsuleLineSegmentPos.v1;
		//LibMath::OtherVector3(capsuleLineSegmentPos.v1, spherePos);

	//������n
	Vector3 capsuleLineSegmentVector = capsuleLineSegmentPos.v2 - capsuleLineSegmentPos.v1;
		//LibMath::OtherVector3(capsuleLineSegmentPos.v1, capsuleLineSegmentPos.v2);

	float t = Vector3::Dot(capsulePos0ToSphere, capsuleLineSegmentVector.Normalize());

	//����������ɐ����ɉ���������̍��W ������Ps��Pn
	Vector3 startToLineUpVector = capsuleLineSegmentVector.Normalize() * t;
	Vector3 onTheLinePos = capsuleLineSegmentPos.v1 + startToLineUpVector;

	float lenghtRate = t / capsuleLineSegmentVector.Length();
	float sphereAndCupsuleDis = 0.0f;
	if (lenghtRate < 0.0f)
	{
		sphereAndCupsuleDis = LibMath::CalcDistance3D(spherePos, capsuleLineSegmentPos.v1);
	}
	else if (lenghtRate > 1.0f)
	{
		sphereAndCupsuleDis = LibMath::CalcDistance3D(spherePos, capsuleLineSegmentPos.v2);
	}
	else
	{
		sphereAndCupsuleDis = LibMath::CalcDistance3D(spherePos, onTheLinePos);
	}

	return sphereAndCupsuleDis < sphere.GetRadius();
}

bool MelLib::Collision::SphereAndTriangle(const SphereData& sphere, const TriangleData& triangle)
{
	Vector3 nearPos;
	nearPos = CalcSphereArea(sphere.GetPosition(), triangle.GetTranslationPosition());

	float dis = LibMath::CalcDistance3D(nearPos, sphere.GetPosition());
	dis *= dis;
	float radius = sphere.GetRadius() * sphere.GetRadius();
	return dis <= radius;
}

bool MelLib::Collision::PlaneAndSegment3D(const PlaneData& plane, const Segment3DData& segment, Segment3DCalcResult* segmentResult)
{
	Vector3 planeToSegV1 = segment.GetPosition().v1 - plane.GetPosition();
	Vector3 planeToSegV2 = segment.GetPosition().v2 - plane.GetPosition();

	bool result = Vector3::Dot(planeToSegV1, plane.GetNormal()) * Vector3::Dot(planeToSegV2, plane.GetNormal()) <= 0;

	if (segmentResult)
	{
		MelLib::Vector3 vec1 = segment.GetPosition().v1 - plane.GetPosition();
		MelLib::Vector3 vec2 = segment.GetPosition().v2 - plane.GetPosition();

		//����������߁A�Փ˓_�����߂�
		float planeToSegV1Dis = abs(Vector3::Dot(plane.GetNormal(), vec1)) / plane.GetNormal().Length();
		float planeToSegV2Dis = abs(Vector3::Dot(plane.GetNormal(), vec2)) / plane.GetNormal().Length();
		float naibunhi = planeToSegV1Dis / (planeToSegV1Dis + planeToSegV2Dis);

		segmentResult->planeHitPos = (1 - naibunhi) * segment.GetPosition().v1 + naibunhi * segment.GetPosition().v2;
	}
	return result;
}

bool Collision::BoardAndSegment3D
(
	const BoardData& board,
	BoardCalcResult* boardCalcResult,
	const Segment3DData& lineSegment,
	Segment3DCalcResult* lineSegmentCalcResult
)
{

	Value2<Vector3> segmentPos = lineSegment.GetRotatePosition();
	Value4<Vector3>boardVertexPos = board.GetVertexPosition();
	Vector3 leftDownPos = boardVertexPos.v1;
	Vector3 leftUpPos = boardVertexPos.v2;
	Vector3 rightDownPos = boardVertexPos.v3;
	Vector3 rightUpPos = boardVertexPos.v4;

	Vector3 v1;
	Vector3 v2;

	v1 = segmentPos.v1 - board.GetPosition();
	v2 = segmentPos.v2 - board.GetPosition();

	//�����|���ƕ��s�ł͂Ȃ����𒲂ׂ�(���s��������return)
	if (Vector3Dot(v1, board.GetNormal()) * Vector3Dot(v2, board.GetNormal()) > 0)
		return false;


	//�����Ƀ|���S���͈͓̔����ǂ����̏���������

	//���̒[ - �|���S���̊p
	v1 = segmentPos.v1 - leftDownPos;
	v2 = segmentPos.v2 - leftDownPos;

	Vector3 vec1 = segmentPos.v1 - board.GetPosition();
	Vector3 vec2 = segmentPos.v2 - board.GetPosition();

	//�����̗��[�Ɣ|���̋��������߂�
	//0�Ŋ���̂�h�~���邽�߂�if
	float kyori1 = 0;
	/*if (board.GetNormal().x != 0)kyori1 += abs(Vector3Dot(board.GetNormal(), vec1)) / abs(board.GetNormal().x);
	if (board.GetNormal().y != 0)kyori1 += abs(Vector3Dot(board.GetNormal(), vec1)) / abs(board.GetNormal().y);
	if (board.GetNormal().z != 0)kyori1 += abs(Vector3Dot(board.GetNormal(), vec1)) / abs(board.GetNormal().z);*/
	kyori1 = abs(Vector3Dot(board.GetNormal(), vec1)) / board.GetNormal().Length();
	float kyori2 = 0;
	/*if (board.GetNormal().x != 0)kyori2 += abs(Vector3Dot(board.GetNormal(), vec2)) / abs(board.GetNormal().x);
	if (board.GetNormal().y != 0)kyori2 += abs(Vector3Dot(board.GetNormal(), vec2)) / abs(board.GetNormal().y);
	if (board.GetNormal().z != 0)kyori2 += abs(Vector3Dot(board.GetNormal(), vec2)) / abs(board.GetNormal().z);*/
	kyori2 = abs(Vector3Dot(board.GetNormal(), vec2)) / board.GetNormal().Length();

	//������
	float a = kyori1 / (kyori1 + kyori2);



	//���ƃ|���S�����������Ă���ꏊ�𒲂ׂ�
	Vector3 crossVector;//�|���S���̊p���瓖�����Ă�ꏊ�̍��W�ւ̃x�N�g��
	crossVector.x = (1 - a) * v1.x + a * v2.x;
	crossVector.y = (1 - a) * v1.y + a * v2.y;
	crossVector.z = (1 - a) * v1.z + a * v2.z;
	Vector3 crossPos = leftDownPos + crossVector;




	//�O�p�`1�ڂ̔���
	Vector3 normal1;
	normal1 = LibMath::CalcNormal(leftDownPos, leftUpPos, crossPos);
	Vector3 normal2;
	normal2 = LibMath::CalcNormal(leftUpPos, rightDownPos, crossPos);
	Vector3 normal3;
	normal3 = LibMath::CalcNormal(rightDownPos, leftDownPos, crossPos);

	//�|���Ɩ@�������������ׂ�
	bool equal1 = false;//�|���Ɩ@�����������ǂ���

	//�قړ�����������true
	if (LibMath::Difference(board.GetNormal().x, normal1.x, 0.0001f) &&
		LibMath::Difference(board.GetNormal().y, normal1.y, 0.0001f) &&
		LibMath::Difference(board.GetNormal().z, normal1.z, 0.0001f) &&
		LibMath::Difference(board.GetNormal().x, normal2.x, 0.0001f) &&
		LibMath::Difference(board.GetNormal().y, normal2.y, 0.0001f) &&
		LibMath::Difference(board.GetNormal().z, normal2.z, 0.0001f) &&
		LibMath::Difference(board.GetNormal().x, normal3.x, 0.0001f) &&
		LibMath::Difference(board.GetNormal().y, normal3.y, 0.0001f) &&
		LibMath::Difference(board.GetNormal().z, normal3.z, 0.0001f))
	{
		equal1 = true;
	}


	//�O�p�`2�ڂ̔���
	normal1 = LibMath::CalcNormal(rightDownPos, leftUpPos, crossPos);
	normal2 = LibMath::CalcNormal(leftUpPos, rightUpPos, crossPos);
	normal3 = LibMath::CalcNormal(rightUpPos, rightDownPos, crossPos);

	//�|���Ɩ@�������������ׂ�
	bool equal2 = false;//�|���Ɩ@�����������ǂ���

	//�قړ���(�덷0.0001�ȓ�)��������true
	if (LibMath::Difference(board.GetNormal().x, normal1.x, 0.0001f) &&
		LibMath::Difference(board.GetNormal().y, normal1.y, 0.0001f) &&
		LibMath::Difference(board.GetNormal().z, normal1.z, 0.0001f) &&
		LibMath::Difference(board.GetNormal().x, normal2.x, 0.0001f) &&
		LibMath::Difference(board.GetNormal().y, normal2.y, 0.0001f) &&
		LibMath::Difference(board.GetNormal().z, normal2.z, 0.0001f) &&
		LibMath::Difference(board.GetNormal().x, normal3.x, 0.0001f) &&
		LibMath::Difference(board.GetNormal().y, normal3.y, 0.0001f) &&
		LibMath::Difference(board.GetNormal().z, normal3.z, 0.0001f))
	{
		equal2 = true;
	}

	//�ǂ��炩������(�|���̒�)��������if�̒���
	if (equal1 || equal2)
	{
		if (lineSegmentCalcResult) lineSegmentCalcResult->boardHitPos = crossPos;
		if (boardCalcResult)boardCalcResult->lineSegment3DHitPos = crossPos;
		return true;
	}

	//�Փˈʒu�ƒ��S���قړ�����������q�b�g
	if (LibMath::Difference(crossPos.x, board.GetPosition().x, 0.001f)
		&& LibMath::Difference(crossPos.y, board.GetPosition().y, 0.001f)
		&& LibMath::Difference(crossPos.z, board.GetPosition().z, 0.001f)
		|| equal1
		|| equal2)
	{
		if (lineSegmentCalcResult) lineSegmentCalcResult->boardHitPos = crossPos;
		if (boardCalcResult)boardCalcResult->lineSegment3DHitPos = crossPos;
		return true;
	}
	return false;

}

bool MelLib::Collision::BoardAndCapsule(const BoardData& board, BoardCalcResult* boardCalcResult, const CapsuleData& capsule, Segment3DCalcResult* lineSegmentCalcResult)
{
	//�����̔��肪�������Ă���A�܂��́A���������a�ȓ�
	//��҂��������Ă邩�𔻒f����΂���

	//�������ǂ����߂邩

	//��������ł���΁A�ǂƂ̔�����Ƃ��

	//���[�̕ӂ̕����ɔ|���S���𔼌a���������āA����������A
	//�J�v�Z���Ɠ������Ă���Č�����?
	//���S�ł�����?
	//���̕��@����_��������

	//�Փ˓_�͌������ĂȂ��Ă����߂���?(���ʈ������Čv�Z���Ă�Ȃ�ł���͂�)
	//�Փ˓_�Ǝl�̕ӂ̋��������a�ȓ��A�܂��́A���ʂƂ̋��������a�ȓ���������OK?
	//���ꂾ�ƁA�͈͊O�ł����ʂƌ������Ă���A�E�g������_��


	//���Ⴀ�A�l�p�`�͈͓̔���������A���[�ƕ��ʂƂ̋������v�Z
	//�͈͊O��������A�ӂƂ̋������v�Z
	//���ꂾ�ƁA�J�v�Z���Ɣ����s�̎��A�E�g
	//���s��������͈͓��ł����ʂƂ̋������v�Z����?
	//���ƁA�΂߂ɂȂ��ĂĔ͈͊O�ɂ��Ă����S�̋߂��ɂ��Ĕ��N�\�f�J��������A�E�g
	//���̏ꍇ�A���[�ƕ��ʂ̋��������߂�΂���?
	//���������A�߂��Ⴍ���ᒷ��������A���[�̋������߂Ă��Ӗ��Ȃ�?

	//�͈͊O�̎��A�ӂƂ̋������v�Z�B����ł��߂������痼�[�̋��������߂�΂���?(�����݂ɒZ���Ĕ��a�ł�����p)
	//�͈͊O�ŗ��[�̋��������߂�ꍇ�A�Փ˓_�����ɂ��邩�m�F���Ȃ��Ƃ����Ȃ�?
	//���������Փ˓_���O�ɂȂ�ꍇ�́A�������m�̃`�F�b�N�Ɉ��������邩������Ȃ�
	//�N�\�����̂��h�����Ă���A���[�ƕ��ʂ̋������߂Ă��Ӗ��Ȃ��ꍇ���邩��A�h�����Ă邩�ǂ��������f���āA
	//�h�����Ă��瓖�����Ă�B�h�����ĂȂ������痼�[���߂�΂����B
	//���s�̎��A�P���ɋ��������߂�ƁA�������ĂȂ��̂ɂ������Ă邱�ƂɂȂ�


	//���̏ꍇ�A
	//1.�h�����Ă���
	//2.���[�ƕ��ʂ̋��������a�ȓ�
	//�ǂ��炩���������Ă�΂����炭�������Ă���

	//�O�̏ꍇ�A
	//1.�����Ɣ̕ӂ̋��������a�ȓ�
	// 
	//���ʂɗ��[�ƕ��ʂ̋������v�Z����ƁA����߂��Ⴍ���ᗣ��ĂĂ����ʂɋ߂�������_��
	//2.


	//����l������?
	//�h�����Ă邩�m�F���A



	//https://www.antun.net/tips/algorithm/collision.html
	//�����ƎO�p�`�̋��������߂�΂��������B
	//(�l�p�`�͎O�p�`�����킹�����̂Ȃ̂�)
	//������Ă��2D�̂������


	Value2<Vector3> segmentPos = capsule.GetSegment3DData().GetRotatePosition();
	Value4<Vector3>boardVertexPos = board.GetVertexPosition();
	Vector3 leftDownPos = boardVertexPos.v1;
	Vector3 leftUpPos = boardVertexPos.v2;
	Vector3 rightDownPos = boardVertexPos.v3;
	Vector3 rightUpPos = boardVertexPos.v4;

	Vector3 v1;
	Vector3 v2;

	v1 = segmentPos.v1 - board.GetPosition();
	v2 = segmentPos.v2 - board.GetPosition();

	//�����|���ƕ��s�ł͂Ȃ����𒲂ׂ�(���s��������return)
	if (Vector3Dot(v1, board.GetNormal()) * Vector3Dot(v2, board.GetNormal()) > 0)
	{
		//���s�������ꍇ�A���[�ƕ��ʂ̋��������a�ȓ��������瓖�����Ă�
		float planeDir = abs(Vector3::Dot(board.GetNormal(), segmentPos.v1 - board.GetPosition())) / board.GetNormal().Length();
		if (planeDir <= capsule.GetRadius())return true;

		planeDir = abs(Vector3::Dot(board.GetNormal(), segmentPos.v2 - board.GetPosition())) / board.GetNormal().Length();
		if (planeDir <= capsule.GetRadius())return true;

		return false;
	}


	//�N�������邱��
	//�͈͓����O���𒲂ׂď������s��


}

bool MelLib::Collision::PointAndSphere(const Vector3& pointPos, const SphereData& sphere)
{
	return LibMath::CalcDistance3D(pointPos, sphere.GetPosition()) <= sphere.GetRadius();
}

//
//bool MelLib::Collision::PointAndCircularSector3D(const Vector3& pointPos, const CircularSector3DData& circularSector)
//{
//	//���̒��ɂ��邩�ǂ����m�F
//	if (!PointAndSphere(pointPos, circularSector.GetSphereData()))return;
//
//	//����
//	//1.
//	//�c�Ɖ��̊p�x�ɉ����āA�{�[�h����]����?
//	//�ǂ�����Ē��ɂ��邩�m�F�����?
//	//�\���̔�����s���Ίm�F�ł���
//
//	//2.
//	//��`��2��������?
//	//
//
//	BoardData nearBoard;
//	BoardData farBoard;
//	BoardData leftBoard;
//	BoardData rightBoard;
//	BoardData upBoard;
//	BoardData downBoard;
//
//}
//
//#pragma endregion
//
//

bool MelLib::Collision::PointAndFrustum(const Vector3& pointPos, const FrustumData& frustum)
{
	//�_���A����̑S�Ă̖ʂ̗����ɂ�������true

	std::vector<BoardData>boards = frustum.GetBoardDatas();

	for (const auto& b : boards)
	{
		//�\���ɂ�������false
		if (LibMath::PointBoardFrontBackCheck(pointPos, b) == 1)
		{
			return false;
		}
	}
	return true;
}

bool MelLib::Collision::BoxAndRay(const BoxData& box, const RayData& ray, RayCalcResult* rayResult)
{
	// https://plaza.rakuten.co.jp/ootorii/diary/200705140000/

	// https://el-ement.com/blog/2017/08/16/primitives-ray-intersection/

	// http://marupeke296.com/COL_3D_No18_LineAndAABB.html
	
	// �����T�C�g
	// https://tavianator.com/2011/ray_box.html


	Vector3 rayPos = ray.GetPosition();
	Vector3 rayDir = ray.GetDirection();

	MelLib::Vector3 minPos, maxPos;
	minPos = box.GetPosition() - box.GetSize() / 2;
	maxPos = box.GetPosition() + box.GetSize() / 2;

	float t = -FLT_MAX;

	// float�ŕ\���ł���l�̍ŏ��l�ƍő�l���擾
	// x���̔���������(��Ԃ͂��߂̎��Ƃ̔��莞�Ɏg��)
	// ����ȍ~�͈�O�̎��Ƃ̋����Ɣ��肵�Ċe���̃X���u���Ō������Ă��邩����
	float minT = -FLT_MAX;
	float maxT = FLT_MAX;

	for (int i = 0; i < 3; i++) 
	{
		if (LibMath::Difference(rayDir[i], 0.0f, 0.0001f))
		{
			// ���炩�ɏo�Ă��瓖�����ĂȂ�����
			if (rayPos[i] < minPos[i] || rayPos[i] > maxPos[i])return false;
			
			// ���false�Ɉ���������Ȃ������瓖�����Ă邽�߁A����
			continue;
		}

		// �|���Z�̂ق����������߁A�t��(invNum)�����߂Ċ|���Z�Ōv�Z����
		// �t��
		float invNum = 1.0f / rayDir[i];

		// �X���u�Ƃ̋������Z�o
		// nearT���߃X���u�AfarT�����X���u�Ƃ̋���
		float nearT = (minPos[i] - rayPos[i]) * invNum;
		float farT = (maxPos[i] - rayPos[i]) * invNum;

		float tMin = min(nearT, farT);
		float tMax = max(nearT, farT);
		nearT = tMin;
		farT = tMax;

		if (nearT > minT) minT = nearT;
		if (farT < maxT) maxT = farT;

		// �X���u�����`�F�b�N
		if (minT >= maxT)return false;
	}
	
	// �ԈႦ�Ă�����0�ȏォ���f��������Ă邯�ǖ��Ȃ�����?
	// �x�N�g���̋t��OBB������Ƃ��A���T���t�ɂȂ邩�炱��Ŗ��Ȃ�����
	if (minT >= 0 && maxT >= 0) 
	{
		if (rayResult)
		{
			Vector3 hitPos = rayPos + minT * rayDir;
			rayResult->hitPosition = hitPos;


			BoxHitDirection hitDirection = BoxHitDirection::BOX_HIT_DIRECTION_NO_HIT;

			char top = 0;
			Vector3 hitPosVector = hitPos;

			if (abs(hitPosVector.x) >= abs(hitPosVector.y)/* &&
				abs(hitPosVector.x) >= box.GetSize().x / 2*/)
			{
				top = 1;
				if (abs(hitPosVector.z) >= abs(hitPosVector.x) /*&&
					abs(hitPosVector.z) >= box.GetSize().z / 2*/)
					top = 3;
			}
			else
			{
				top = 2;
				if (abs(hitPosVector.z) >= abs(hitPosVector.y)/* &&
					abs(hitPosVector.z) >= box.GetSize().z / 2*/)
					top = 3;
			}

			if (top == 1)
			{
				if (hitPosVector.x >= 0)
				{
					hitDirection = BoxHitDirection::BOX_HIT_DIRECTION_RIGHT;
				}
				else
				{
					hitDirection = BoxHitDirection::BOX_HIT_DIRECTION_LEFT;
				}
			}
			else if (top == 2)
			{
				if (hitPosVector.y >= 0)
				{
					hitDirection = BoxHitDirection::BOX_HIT_DIRECTION_UP;
				}
				else
				{
					hitDirection = BoxHitDirection::BOX_HIT_DIRECTION_DOWN;
				}
			}
			else if (top == 3)
			{
				if (hitPosVector.z >= 0)
				{
					hitDirection = BoxHitDirection::BOX_HIT_DIRECTION_BACK;
				}
				else
				{
					hitDirection = BoxHitDirection::BOX_HIT_DIRECTION_FRONT;
				}
			}


			rayResult->hitObjectNormal = CalcHitBoxSurfaceNormal(hitDirection);
		}
		return true;
	}
	return false;
}

bool MelLib::Collision::BoxAndSegment3D(const BoxData& box, const Segment3DData& segment)
{
	RayData ray;
	ray.SetPosition(segment.GetPosition().v1);
	ray.SetDirection((segment.GetPosition().v2 - segment.GetPosition().v1).Normalize());
	RayCalcResult result;

	if (!BoxAndRay(box, ray, &result))return false;

	// �Փ˓_�������Əd�Ȃ��Ă��瓖�����Ă锻��
	if (PointAndSegment3D(result.hitPosition, segment))return true;
	return false;

}

bool MelLib::Collision::PointAndSegment3D(const Vector3& pointPos, const Segment3DData& segment)
{
	Vector3 segmentPosVector = segment.GetRotatePosition().v2 - segment.GetRotatePosition().v1;
	Vector3 segmentV1ToPoint = pointPos - segment.GetRotatePosition().v1;

	float v1 = segmentV1ToPoint.Length();
	float v2 = segmentPosVector.Length();
	return LibMath::Difference(Vector3::Cross(segmentV1ToPoint, segmentPosVector).Length(), 0.0f, 0.0001f)
		&& segmentV1ToPoint.Length() <= segmentPosVector.Length();

}

bool MelLib::Collision::TriangleAndSegment3D(const TriangleData& triangle, TriangleCalcResult* triangleCalcResult, const Segment3DData& lineSegment, Segment3DCalcResult* lineSegmentCalcResult)
{
	Value2<Vector3> segmentPos = lineSegment.GetRotatePosition();

	Value3<Vector3>triangleVertexPos = triangle.GetTranslationPosition();

	// ��]�ƕ��s�ړ���K���������O�p�`�̒��S���W���擾
	Vector3 centerPos = triangle.GetRotTranceFormCenter();

	Vector3 v1;
	Vector3 v2;

	v1 = segmentPos.v1 - centerPos;
	v2 = segmentPos.v2 - centerPos;

	//�����O�p�`�ƕ��s�ł͂Ȃ����𒲂ׂ�(���s��������return)
	if (Vector3Dot(v1, triangle.GetNormal()) * Vector3Dot(v2, triangle.GetNormal()) > 0) return false;

	//���̒[ - �|���S���̊p
	v1 = segmentPos.v1 - triangleVertexPos.v1;
	v2 = segmentPos.v2 - triangleVertexPos.v1;

	Vector3 vec1 = segmentPos.v1 - centerPos;
	Vector3 vec2 = segmentPos.v2 - centerPos;

	//�����̗��[�Ɣ|���̋��������߂�
	//0�Ŋ���̂�h�~���邽�߂�if
	float kyori1 = 0;
	kyori1 = abs(Vector3Dot(triangle.GetNormal(), vec1)) / triangle.GetNormal().Length();
	float kyori2 = 0;
	kyori2 = abs(Vector3Dot(triangle.GetNormal(), vec2)) / triangle.GetNormal().Length();

	//������
	float a = kyori1 / (kyori1 + kyori2);



	//���ƃ|���S�����������Ă���ꏊ�𒲂ׂ�
	Vector3 crossVector;//�|���S���̊p���瓖�����Ă�ꏊ�̍��W�ւ̃x�N�g��
	crossVector.x = (1 - a) * v1.x + a * v2.x;
	crossVector.y = (1 - a) * v1.y + a * v2.y;
	crossVector.z = (1 - a) * v1.z + a * v2.z;
	Vector3 crossPos = triangleVertexPos.v1 + crossVector;




	//�O�p�`1�ڂ̔���
	Vector3 normal1;
	normal1 = LibMath::CalcNormal(triangleVertexPos.v1, triangleVertexPos.v2, crossPos);
	Vector3 normal2;
	normal2 = LibMath::CalcNormal(triangleVertexPos.v2, triangleVertexPos.v3, crossPos);
	Vector3 normal3;
	normal3 = LibMath::CalcNormal(triangleVertexPos.v3, triangleVertexPos.v1, crossPos);

	//�|���Ɩ@�������������ׂ�
	bool isHit = false;//�|���Ɩ@�����������ǂ���

	//�قړ�����������true
	if (LibMath::Difference(triangle.GetNormal().x, normal1.x, 0.0001f) &&
		LibMath::Difference(triangle.GetNormal().y, normal1.y, 0.0001f) &&
		LibMath::Difference(triangle.GetNormal().z, normal1.z, 0.0001f) &&
		LibMath::Difference(triangle.GetNormal().x, normal2.x, 0.0001f) &&
		LibMath::Difference(triangle.GetNormal().y, normal2.y, 0.0001f) &&
		LibMath::Difference(triangle.GetNormal().z, normal2.z, 0.0001f) &&
		LibMath::Difference(triangle.GetNormal().x, normal3.x, 0.0001f) &&
		LibMath::Difference(triangle.GetNormal().y, normal3.y, 0.0001f) &&
		LibMath::Difference(triangle.GetNormal().z, normal3.z, 0.0001f))
	{
		isHit = true;
	}

	//�Փˈʒu�ƒ��S���قړ�����������q�b�g
	if (LibMath::Difference(crossPos.x, centerPos.x, 0.001f)
		&& LibMath::Difference(crossPos.y, centerPos.y, 0.001f)
		&& LibMath::Difference(crossPos.z, centerPos.z, 0.001f)
		)
	{
		isHit = true;
	}

	if (lineSegmentCalcResult && isHit) lineSegmentCalcResult->triangleHitPos = crossPos;
	if (triangleCalcResult && isHit)triangleCalcResult->lineSegment3DHitPos = crossPos;

	return isHit;
}

bool MelLib::Collision::OBBAndRay(const OBBData& obb, const RayData& ray, RayCalcResult* rayResult)
{

	// Box��Ray�̔��肪����̂ŁA
	// OBB�̊p�x * -1�̊p�x�����C����]�����Ă�����΂���?
	// ����Ȃ��ƂȂ�������
	// ����ς�����?
	// OBB�𒆐S�Ƀ��C�̓_����]������΂�������
	// ���C�̃x�N�g������]�����Ȃ��Ƃ����Ȃ�


	// �Ȃ񂩉�]���~�X���Ă�?

	RayData rotRay;

	// �x�N�g������]
	rotRay.SetDirection(LibMath::RotateZXYVector3(ray.GetDirection(), obb.GetAngle() * -1));
	
	// ���W����]
	Vector3 rotRayPos = ray.GetPosition();
	rotRayPos -= obb.GetPosition();
	rotRayPos = LibMath::RotateZXYVector3(rotRayPos, obb.GetAngle() * -1);
	rotRayPos += obb.GetPosition();
	rotRay.SetPosition(rotRayPos);

	bool result = BoxAndRay(obb.GetBoxData(), rotRay, rayResult);

	if (rayResult) 
	{
		// �p�x����]
		Vector3 rotRayPos = rayResult->hitPosition;
		rotRayPos -= obb.GetPosition();
		rotRayPos = LibMath::RotateZXYVector3(rotRayPos, obb.GetAngle());
		rotRayPos += obb.GetPosition();
		rayResult->hitPosition = rotRayPos;

		rayResult->hitObjectNormal = MelLib::LibMath::RotateZXYVector3(rayResult->hitObjectNormal, obb.GetAngle());
	}

	return result;
}
