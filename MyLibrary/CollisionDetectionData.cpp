#include"CollisionDetectionData.h"
#include"LibMath.h"
#include"Interpolation.h"

using namespace MelLib;



void  MelLib::Segment3DData::CalcRotatePosition()
{


	Vector3 v1ToV2 = Interpolation::Lerp(position.v1, position.v2, rotatePoint);

	//rotatePosition = Value2<Vector3>
	//	(
	//		Quaternion::GetZXYRotateQuaternion(position.v1 - v1ToV2, Vector3(angle.x, angle.y, angle.z)).ToVector3(),
	//		Quaternion::GetZXYRotateQuaternion(position.v2 - v1ToV2, Vector3(angle.x, angle.y, angle.z)).ToVector3()
	//	);


	rotatePosition.v1 = Quaternion::GetRotateQuaternion(position.v1 - v1ToV2, axisZVector, angle.z).ToVector3();
	rotatePosition.v1 = Quaternion::GetRotateQuaternion(rotatePosition.v1, axisXVector, angle.x).ToVector3();
	rotatePosition.v1 = Quaternion::GetRotateQuaternion(rotatePosition.v1, axisYVector, angle.y).ToVector3();


	rotatePosition.v2 = Quaternion::GetRotateQuaternion(position.v2 - v1ToV2, axisZVector, angle.z).ToVector3();
	rotatePosition.v2 = Quaternion::GetRotateQuaternion(rotatePosition.v2, axisXVector, angle.x).ToVector3();
	rotatePosition.v2 = Quaternion::GetRotateQuaternion(rotatePosition.v2, axisYVector, angle.y).ToVector3();

	//�߂�
	rotatePosition.v1 += v1ToV2;
	rotatePosition.v2 += v1ToV2;
}

void MelLib::Segment3DData::SetPosition(const Value2<Vector3>& pos)
{
	Value2<Vector3> prePos = position;
	position = pos;

	if (position == prePos)return;
	CalcRotatePosition();
}

void MelLib::Segment3DData::SetAngle(const Vector3& angle)
{
	if (this->angle == angle)return;
	this->angle = angle;
	CalcRotatePosition();
}

void  MelLib::Segment3DData::SetRotatePoint(const float num)
{
	if (rotatePoint == num)return;
	rotatePoint = num;
	CalcRotatePosition();
}

void MelLib::Segment3DData::SetAxisAngle(const Vector3& angle)
{
	if (axisAngle == angle)return;
	axisAngle = angle;

	axisXVector = Quaternion::GetZXYRotateQuaternion(Vector3(-1, 0, 0), angle).ToVector3().Normalize();
	axisYVector = Quaternion::GetZXYRotateQuaternion(Vector3(0, -1, 0), angle).ToVector3().Normalize();
	axisZVector = Quaternion::GetZXYRotateQuaternion(Vector3(0, 0, -1), angle).ToVector3().Normalize();
}


void MelLib::BoardData::CalcRotateDirPosition()
{
	//�����s��ɂ����ق�������
	//�s��g���܂킵���ق����v�Z�R�X�g���Ȃ�

	/*leftDownPos = Quaternion::GetZXYRotateQuaternion(leftDownPos, angle).ToVector3();
	leftUpPos = Quaternion::GetZXYRotateQuaternion(leftUpPos, angle).ToVector3();
	rightDownPos = Quaternion::GetZXYRotateQuaternion(rightDownPos, angle).ToVector3();
	rightUpPos = Quaternion::GetZXYRotateQuaternion(rightUpPos, angle).ToVector3();*/

}

void MelLib::BoardData::SetPosition(const Vector3& pos)
{
	Vector3 prePos = position;
	position = pos;

	Vector3 posSubPre = pos - prePos;

	leftDownPos += posSubPre;
	leftUpPos += posSubPre;
	rightUpPos += posSubPre;
	rightDownPos += posSubPre;
}

void MelLib::BoardData::SetSize(const Vector2& size)
{
	if (this->size == size)return;

	Vector2 preSize = this->size;
	this->size = size;


	//�O�Ƃ̍��������߂āA���W���ړ�������
	//���̑傫�����A���S�����̋t��length���ړ�(length��-�������璆�S�ɋ߂Â�)
	//length���ƃ}�C�i�X����Ȃ�


	//0���ƃx�N�g�������߂��Ȃ��̂ŁA�d���Ȃ���]������
	//if (preSize == 0.0f)
	//{
	//	leftDownPos = MelLib::Vector3(-size.x, -size.y, 0) / 2;
	//	leftUpPos = MelLib::Vector3(-size.x, size.y, 0) / 2;
	//	rightDownPos = MelLib::Vector3(size.x, -size.y, 0) / 2;
	//	rightUpPos = MelLib::Vector3(size.x, size.y, 0) / 2;

	//	//��]
	//	CalcRotateDirPosition();
	//}
	//else
	//{
	//	//��(�}�C�i�X����)�����߂�
	//	Vector2 curSubPre = size - preSize;
	//	Vector3 pos = position;

	//	/// <summary>
	//	/// �T�C�Y��K�������郉���_��
	//	/// </summary>
	//	/// <param name="size"></param>
	//	auto calcMovePos = [&curSubPre, &pos](const Vector3& dirPos)
	//	{
	//		Vector3 posToDirPos = LibMath::OtherVector3(pos, dirPos);
	//		Vector3 returnPos;

	//		//����X���ړ�
	//		returnPos = LibMath::FloatDistanceMoveVector3(dirPos, posToDirPos.x, curSubPre.x);
	//		//����Y���ړ�
	//		returnPos = LibMath::FloatDistanceMoveVector3(returnPos, posToDirPos.y, curSubPre.y);

	//		return returnPos;
	//	};

	//	//�T�C�Y���g�k
	//	leftDownPos = calcMovePos(leftDownPos);
	//	leftUpPos = calcMovePos(leftUpPos);
	//	rightDownPos = calcMovePos(rightDownPos);
	//	rightUpPos = calcMovePos(rightUpPos);
	//}

	leftDownPos = MelLib::Vector3(-size.x, -size.y, 0) / 2;
	leftUpPos = MelLib::Vector3(-size.x, size.y, 0) / 2;
	rightDownPos = MelLib::Vector3(size.x, -size.y, 0) / 2;
	rightUpPos = MelLib::Vector3(size.x, size.y, 0) / 2;

	//��]
	//�����s��ɂ����ق�������
	//�s��g���܂킵���ق����v�Z�R�X�g���Ȃ�
	//CalcRotateDirPosition();
	leftDownPos = Quaternion::GetZXYRotateQuaternion(leftDownPos, angle).ToVector3();
	leftUpPos = Quaternion::GetZXYRotateQuaternion(leftUpPos, angle).ToVector3();
	rightDownPos = Quaternion::GetZXYRotateQuaternion(rightDownPos, angle).ToVector3();
	rightUpPos = Quaternion::GetZXYRotateQuaternion(rightUpPos, angle).ToVector3();


	//�ړ�
	leftDownPos += position;
	leftUpPos += position;
	rightDownPos += position;
	rightUpPos += position;
}

void MelLib::BoardData::SetAngle(const Vector3& angle)
{
	if (this->angle == angle)return;
	

	//��]
	normal = Quaternion::GetZXYRotateQuaternion(Vector3(0, 0, -1), angle).ToVector3();
	//CalcRotateDirPosition();

	//�����s��ɂ����ق�������
	//�s��g���܂킵���ق����v�Z�R�X�g���Ȃ�
	MelLib::Vector3 preSubCrr = angle - this->angle;
	this->angle = angle;
	leftDownPos = Quaternion::GetZXYRotateQuaternion(leftDownPos, preSubCrr).ToVector3();
	leftUpPos = Quaternion::GetZXYRotateQuaternion(leftUpPos, preSubCrr).ToVector3();
	rightDownPos = Quaternion::GetZXYRotateQuaternion(rightDownPos, preSubCrr).ToVector3();
	rightUpPos = Quaternion::GetZXYRotateQuaternion(rightUpPos, preSubCrr).ToVector3();
}



void MelLib::FrustumData::XYRotate()
{
	/*xyAngle�͑S�̂̊p�x�̂��߁A/2������
	xyRotateLeftNormal = Quaternion::GetRotateQuaternion(Vector3(-1, 0, 0), Vector3(0, -1, 0), xyAngle.y / 2).ToVector3();
	xyRotateRightNormal = Quaternion::GetRotateQuaternion(Vector3(1, 0, 0), Vector3(0, -1, 0), xyAngle.y / 2).ToVector3();
	xyRotateUpNormal = Quaternion::GetRotateQuaternion(Vector3(0, 1, 0), Vector3(-1, 0, 0), xyAngle.x / 2).ToVector3();
	xyRotateDownNormal = Quaternion::GetRotateQuaternion(Vector3(0, -1, 0), Vector3(-1, 0, 0), xyAngle.x / 2).ToVector3();

	leftPlane.SetNormal(xyRotateLeftNormal);
	rightPlane.SetNormal(xyRotateRightNormal);
	upPlane.SetNormal(xyRotateUpNormal);
	downPlane.SetNormal(xyRotateDownNormal);*/

	leftAngle = Vector3(0, 90 - xyAngle.x / 2, 0);
	rightAngle = Vector3(0, -90 + xyAngle.x / 2, 0);
	upAngle = Vector3(90 - xyAngle.y / 2, 0, 0);
	downAngle = Vector3(-90 + xyAngle.y / 2, 0, 0);

	/*leftBoard.SetAngle(leftAngle);
	rightBoard.SetAngle(rightAngle);
	upBoard.SetAngle(upAngle);
	downBoard.SetAngle(downAngle);*/
}

void MelLib::FrustumData::AngleRotate()
{
	/*leftPlane.SetNormal(Quaternion::GetZXYRotateQuaternion(xyRotateLeftNormal, angle).ToVector3());
	rightPlane.SetNormal(Quaternion::GetZXYRotateQuaternion(xyRotateRightNormal, angle).ToVector3());
	upPlane.SetNormal(Quaternion::GetZXYRotateQuaternion(xyRotateUpNormal, angle).ToVector3());
	downPlane.SetNormal(Quaternion::GetZXYRotateQuaternion(xyRotateDownNormal, angle).ToVector3());
	nearPlane.SetNormal(Quaternion::GetZXYRotateQuaternion(Vector3(0, 0, -1) , angle).ToVector3());
	farPlane.SetNormal(Quaternion::GetZXYRotateQuaternion(Vector3(0, 0, 1) , angle).ToVector3());*/

	nearBoard.SetAngle(angle);
	farBoard.SetAngle(angle + Vector3(0, 180, 0));
	leftBoard.SetAngle(leftAngle + angle);
	rightBoard.SetAngle(rightAngle + angle);
	upBoard.SetAngle(upAngle + angle);
	downBoard.SetAngle(downAngle + angle);



}

void MelLib::FrustumData::SetBoardPosition()
{
	//�N�H�[�^�j�I����Board�̍��W����]������
	//nearfar�ƃA���O���̃Z�b�g�ŌĂяo��
	//���W�������̂Ɗg�k�͕\�������f�o�b�O���݂̂ł���������
	//���\����Ŏg�����烊���[�X�ł��K�v
	

	nearBoard.SetPosition(LibMath::RotateZXYVector3(Vector3(0, 0, nearNum), angle) + position);
	farBoard.SetPosition(LibMath::RotateZXYVector3(Vector3(0, 0, farNum), angle) + position);

	/// 2����far�̒[�����Ɠ����ꏊ�ɂȂ�̂ŁA/4
	leftBoard.SetPosition
	(LibMath::RotateZXYVector3(Vector3(-xyAngle.x / 4, 0, Interpolation::Lerp(nearNum, farNum, 0.5f)), angle) + position);

	rightBoard.SetPosition
	(LibMath::RotateZXYVector3(Vector3(xyAngle.x / 4, 0, Interpolation::Lerp(nearNum, farNum, 0.5f)), angle) + position);


	upBoard.SetPosition
	(LibMath::RotateZXYVector3(Vector3(0, xyAngle.y / 4, Interpolation::Lerp(nearNum, farNum, 0.5f)), angle) + position);


	downBoard.SetPosition
	(LibMath::RotateZXYVector3(Vector3(0, -xyAngle.y / 4, Interpolation::Lerp(nearNum, farNum, 0.5f)), angle) + position);
}

void MelLib::FrustumData::SetPlaneDistance()
{

	//���ꂶ�ᖳ��
	//farPlane�̋����ɁAfarNum�����̂悭�Ȃ��B���ۂ�-�̕����ɍs���Ă�\�������邩��
	//left�Ƃ����A�����炭���ꂶ�ᐳ�����Ȃ�?
	//���ʂ͖@���̕����ɂ����������Ȃ�����A���܂��������邵���Ȃ�
	//���ꂩ�ABoard�ɕύX����

//	leftPlane.SetDistance(position.Length() + xyAngle.x / 2);
//	rightPlane.SetDistance(position.Length() + xyAngle.x / 2);
//	upPlane.SetDistance(position.Length() + xyAngle.y / 2);
//	downPlane.SetDistance(position.Length() + xyAngle.y / 2);
//
//	nearPlane.SetDistance(position.Length() + nearNum);
//	farPlane.SetDistance(position.Length() + farNum);
	
	/*leftPlane.SetDistance(position.Length() + xyAngle.x / 2);
	rightPlane.SetDistance(position.Length() + xyAngle.x / 2);
	upPlane.SetDistance(position.Length() + xyAngle.y / 2);
	downPlane.SetDistance(position.Length() + xyAngle.y / 2);*/

	
	//nearPlane.SetDistance(LibMath::FloatDistanceMoveVector3(position ,-nearPlane.GetNormal(), nearNum).Length());
	//farPlane.SetDistance(LibMath::FloatDistanceMoveVector3(position, farPlane.GetNormal(), farNum).Length());
	
}


MelLib::FrustumData::FrustumData()
{
	//��]
	XYRotate();
	AngleRotate();

	SetBoardPosition();
	SetPlaneDistance();
}

std::vector<BoardData> MelLib::FrustumData::GetBoardDatas() const
{
	return
	{
		leftBoard,
		rightBoard,
		upBoard,
		downBoard,
		nearBoard,
		farBoard
	};

}

void MelLib::FrustumData::SetPosition(const Vector3& pos)
{
	if (position == pos)return;
	position = pos;
	SetBoardPosition();
}

void MelLib::FrustumData::SetAngle(const Vector3& angle)
{
	//�p�x�̍���������]����悤�ɂ����xy��Normal���L�^����K�v�Ȃ��Ȃ�
	//�ʓr�֐��K�v������

	if (this->angle == angle)return;
	this->angle = angle;

	AngleRotate();
	SetBoardPosition();
}

void MelLib::FrustumData::SetXYAngle(const Vector2& angle)
{
	if (this->xyAngle == angle)return;
	this->xyAngle = angle;

	XYRotate();
	AngleRotate();

	SetBoardPosition();
	SetPlaneDistance();
	
}

void MelLib::FrustumData::SetNear(const float nearNum)
{
	if (this->nearNum == nearNum)return;
	this->nearNum = nearNum;

	SetBoardPosition();
}

void MelLib::FrustumData::SetFar(const float farNum)
{
	if (this->farNum == farNum)return;
	this->farNum = farNum;


	SetBoardPosition();
}

void MelLib::TriangleData::CalcNormal()
{
	//��]�K��
	rotPos.v1 = MelLib::Quaternion::GetZXYRotateQuaternion(scalingPos.v1, angle).ToVector3();
	rotPos.v2 = MelLib::Quaternion::GetZXYRotateQuaternion(scalingPos.v2, angle).ToVector3();
	rotPos.v3 = MelLib::Quaternion::GetZXYRotateQuaternion(scalingPos.v3, angle).ToVector3();

	normal = LibMath::CalcNormal(rotPos.v1, rotPos.v2, rotPos.v3);

}


void MelLib::TriangleData::SetPosition(const Value3<Vector3>& pos)
{
	if (position == pos)return;

	position = pos;
	scalingPos = position * scale;
	CalcNormal();

	translationPos = rotPos + translationVec;
}

void MelLib::TriangleData::SetAngle(const Vector3& angle)
{
	if (this->angle == angle || angle == 0)return;

	this->angle = angle;
	CalcNormal();

	translationPos = rotPos + translationVec;
}

void MelLib::TriangleData::SetScale(const Vector3& scale)
{
	if (this->scale == scale)return;

	this->scale = scale;
	scalingPos = position * scale;
	CalcNormal();

	translationPos = rotPos + translationVec;
}

void MelLib::TriangleData::SetTranslationPosition(const Vector3& vec)
{
	translationVec = vec;
	translationPos = rotPos + vec;

}
