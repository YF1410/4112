#pragma once
#include"Vector.h"
#include"Values.h"
namespace MelLib
{

	//CalcResult��CollisionResult�ɂ���?
	//CollisionDetectionData.h����PrimitiveType�ɕς���?
	//�Փˊm�F�Ɏg���f�[�^�����炱�̂܂܂ł���?�v���~�e�B�u����Ȃ��f�[�^(����)���邵
	//ShapeType�̖��O�ς���?

	//����f�[�^�ɏՓˊm�F�֐��������Ă�����?

	//CalcResult��Data�Ɏ������Ȃ��Ă���������
	//Segment3D�Ȃǂ�hit���W1�ł�������

	//����w�b�_�[�t�@�C���������ق�����������

	// ���OHitCheckData�Ƃ��ɂ��Ă���������

	//�Փˊm�F�t���O
	struct CollisionDetectionFlag
	{
		bool sphere = true;
		bool box = true;
		bool obb = true;
		bool ray = true;
		bool segment = true;
		bool plane = true;
		bool board = true;
		bool capsule = true;
		bool triangle = true;
	};

	struct CollisionDetectionFlag2D
	{
		bool circle = true;
		bool rect = true;
		bool segment = true;
	};

	//2D�̌`��̗�
	enum class ShapeType2D
	{
		RECT,
		CIRCLE,
		SEGMENT,
	};

	//3D�̌`��̗�
	enum class ShapeType3D
	{
		SPHERE,
		BOX,
		OBB,
		PLANE,
		TRIANGLE,
		BOARD,
		SEGMENT,
		RAY,
		CAPSULE,
	};


	enum class BoxHitDirection
	{
		BOX_HIT_DIRECTION_NO_HIT,
		BOX_HIT_DIRECTION_UP,
		BOX_HIT_DIRECTION_DOWN,
		BOX_HIT_DIRECTION_LEFT,
		BOX_HIT_DIRECTION_RIGHT,
		BOX_HIT_DIRECTION_FRONT,
		BOX_HIT_DIRECTION_BACK,
	};

	//�f�[�^�ƃ��U���g�܂Ƃ߂��\���̗p�ӂ���?�������T�C�Y����̖ʓ|����

#pragma region 2D


#pragma region �l�p�`

//�l�p�`
	class RectData
	{
	private:
		//�l�p�`�̍���
		Vector2 position;
		//�ӂ̒���
		Vector2 size;

	public:

		Vector2 GetPosition()const { return position; }
		Vector2 GetSize()const { return size; }

		void SetPosition(const Vector2& pos) { position = pos; }
		void SetSize(const Vector2& size) { this->size = size; }
	};
#pragma endregion

#pragma region �~

	//�~�̌v�Z����
	struct CircleCalcResult
	{
		Vector2 lineSegmentNearPosition;
	};

	//�~
	class CircleData
	{
	private:
		Vector2 position;
		float r = 0.0f;

		CircleCalcResult result;
	public:
		Vector2 GetPosition()const { return position; }
		float GetRadius()const { return r; }
		CircleCalcResult GetCalcResult() { return result; }

		void SetPosition(const Vector2& pos) { position = pos; }
		void SetRadius(const float r) { this->r = r; }
		void SetCalcResult(const CircleCalcResult& result) { this->result = result; }
	};

#pragma endregion

#pragma region ��`
	class CircularSectorData
	{
	private:
		CircleData circleData;

		//����
		Vector2 direction;
		//�p�x(���v)
		float angle = 0.0f;

	public:
		CircleData GetCircleData()const { return circleData; }
		CircleData& GetRefCircleData() { return circleData; }
		Vector2 GetDirection()const { return direction; }
		float GetAngle()const { return angle; }

		void SetCircleData(const CircleData& data) { circleData = data; }
		void SetDirection(const Vector2 direction) { this->direction = direction; }
		void SetAngle(const float angle) { this->angle = angle; }
	};
#pragma endregion


#pragma region ����2D
	enum class SegmentHitPlace
	{
		LS_HIT_POSITION_NOT_HIT,//�Փ˂��ĂȂ�
		LS_HIT_POSITION_LE_START_END,//���̒[(�n�_�I�_)
		LS_HIT_POSITION_LE_LINE,//���̒[�ȊO
	};

	struct Segment2DCalcResult
	{
		//�������Ă���W
		Vector2 hitPos;
		//���������ꏊ
		SegmentHitPlace hitPlace;

		//�ŋߓ_
		Vector2 nearPos;
	};

	class Segment2DData
	{
	private:
		//���W(�n�_�I�_)
		Value2<Vector2> position;

		Segment2DCalcResult result;
	public:

		Value2<Vector2> GetPosition()const { return position; }
		Segment2DCalcResult GetCalcResult()const { return result; }

		void SetPosition(const Value2<Vector2>& pos) { position = pos; }
		void SetCalcResult(const Segment2DCalcResult& result) { this->result = result; }
	};




#pragma endregion


#pragma endregion

#pragma region 3D

#pragma region ��


	class SphereCalcResult
	{
		//���̂ǂ̕����ɂԂ��������Ƃ������
		BoxHitDirection boxHitDirection = BoxHitDirection::BOX_HIT_DIRECTION_NO_HIT;
		

		Vector3 boxHitSurfaceNormal;
		Vector3 obbHitSurfaceNormal;


	public:
		/// <summary>
		/// ��������BOX�̂ǂ��ɓ������������擾���܂��B
		/// </summary>
		/// <returns></returns>
		BoxHitDirection GetBoxHitDirection()const { return boxHitDirection; }
		
		/// <summary>
		/// ��������BOX�̖ʂ̖@�����擾���܂��B
		/// </summary>
		/// <returns></returns>
		Vector3 GetBoxHitSurfaceNormal()const { return boxHitSurfaceNormal; }
		
		/// <summary>
		/// ��������OBB�̖ʂ̖@�����擾���܂��B
		/// </summary>
		/// <returns></returns>
		Vector3 GetOBBHitSurfaceNormal()const { return obbHitSurfaceNormal; }

		void SetBoxHitDirection(BoxHitDirection dir) { boxHitDirection = dir; }
		void SetBoxHitSurfaceNormal(const Vector3& normal) { boxHitSurfaceNormal = normal; }
		void SetOBBHitSurfaceNormal(const Vector3& normal) { obbHitSurfaceNormal = normal; }
	};

	//��
	class SphereData
	{
	private:
		//���W
		Vector3 position;

		//���a
		float r = 0.0f;

		SphereCalcResult result;

	public:
		Vector3 GetPosition()const { return position; }
		float GetRadius()const { return r; }
		SphereCalcResult GetCalcResult() const { return result; }

		void SetPosition(const Vector3& pos) { position = pos; }
		void SetRadius(const float radius) { r = radius; }
		void SetCalcResult(const SphereCalcResult& result) { this->result = result; }
	};


#pragma endregion

#pragma region AABB
	struct BoxCalcResult
	{

		//���肪�ǂ��ɂԂ�������
		BoxHitDirection boxHitDistance = BoxHitDirection::BOX_HIT_DIRECTION_NO_HIT;
	};

	//��
	class BoxData
	{
	private:
		Vector3 position;
		Vector3 size;
		BoxCalcResult result;

	public:
		Vector3 GetPosition()const { return position; }
		Vector3 GetSize()const { return size; }
		BoxCalcResult GetCalcResult() { return result; }

		void SetPosition(const Vector3& pos) { position = pos; }
		void SetSize(const Vector3& size) { this->size = size; }
		void SetCalcResult(const BoxCalcResult& result) { this->result = result; }
	};



#pragma endregion

#pragma region OBB
	
	class OBBData
	{
	private:
		BoxData boxData;
		Vector3 angle;
	public:
		
		BoxData GetBoxData()const { return boxData; }
		BoxData& GetRefBoxData() { return boxData; }

		Vector3 GetPosition()const { return boxData.GetPosition(); }
		Vector3 GetSize()const { return boxData.GetSize(); }
		Vector3 GetAngle()const { return angle; }


		void SetPosition(const Vector3& pos) { boxData.SetPosition(pos); }
		void SetSize(const Vector3& size) { boxData.SetSize(size); }
		void SetAngle(const Vector3& angle) { this->angle = angle; }


	};

#pragma endregion

#pragma region ����
	

	//����
	class PlaneData
	{
	private:
		Vector3 position;
		Vector3 normal;
	public:
		Vector3 GetPosition()const { return position; }
		Vector3 GetNormal()const { return normal; }

		void SetPosition(const Vector3& pos) { position = pos; }
		void SetNormal(const Vector3& normal) { this->normal = normal; }
	};
#pragma endregion

#pragma region ��

	struct BoardCalcResult
	{
		Vector3 lineSegment3DHitPos;
	};

	//��
	//�̔���B����]���́A0,0,1�̕����������܂��B
	class BoardData
	{
	private:
		Vector3 position;
		Vector2 size;
		Vector3 angle;
		BoardCalcResult result;

		Vector3 normal = Vector3(0, 0, -1);
		Vector3 leftDownPos;
		Vector3 leftUpPos;
		Vector3 rightUpPos;
		Vector3 rightDownPos;

	private:
		void CalcRotateDirPosition();

	public:
		Vector3 GetPosition()const { return position; }
		Vector2 GetSize()const { return size; }
		Vector3 GetAngle()const { return angle; }
		BoardCalcResult GetCalcResult()const { return result; }

		Vector3 GetNormal()const { return normal; }
		//�����A����A�E���A�E��̏��Ŋi�[����Ă���
		Value4<Vector3>GetVertexPosition()const { return Value4<Vector3>(leftDownPos, leftUpPos, rightDownPos, rightUpPos); }


		void SetPosition(const Vector3& pos);
		void SetSize(const Vector2& size);
		void SetAngle(const Vector3& angle);
		void SetCalcResult(const BoardCalcResult& result) { this->result = result; }
	};


#pragma endregion

#pragma region �O�p�`
	struct TriangleCalcResult
	{
		Vector3 lineSegment3DHitPos;
	};

	class TriangleData
	{
	private:
		// ���W��3�܂Ƃ߂Ă���
		Value3<Vector3>position;

		Vector3 scale;
		Value3<Vector3> scalingPos;


		// �p�x
		Vector3 angle;
		// ��]�̍��W
		Value3<Vector3>rotPos;

		//���s�ړ������
		Vector3 translationVec;
		//���s�ړ���̍��W
		Value3<Vector3>translationPos;

		// �@��
		Vector3 normal;

		// ���ݖ��g�p
		TriangleCalcResult result;
	private:
		void CalcNormal();

	public:

		Value3<Vector3> GetPosition()const { return position; }
		
		/// <summary>
		/// ��]��ɕ��s�ړ�����ړ��ʂ��擾���܂��B
		/// </summary>
		/// <returns></returns>
		Value3<Vector3> GetTranslationPosition()const { return translationPos; }
		Vector3 GetAngle()const { return angle; }
		Vector3 GetTranslationVector()const { return translationVec; }
		
		/// <summary>
		/// �@�����擾���܂��B�@���ɂ́A��]���K�����ꂽ���̂��Ԃ��Ă��܂��B
		/// </summary>
		/// <returns></returns>
		Vector3 GetNormal()const { return normal; }

		/// <summary>
		/// �O�p�`�̒��S���W���擾���܂��B
		/// </summary>
		Vector3 GetCenter()const { return (position.v1 + position.v2 + position.v3) / 3; }

		/// <summary>
		/// ��]�������O�p�`�̒��S���W���擾���܂��B
		/// </summary>
		Vector3 GetRotCenter()const { return (rotPos.v1 + rotPos.v2 + rotPos.v3) / 3; }

		/// <summary>
		/// ��]�ƕ��s�ړ������O�p�`�̒��S���W���擾���܂��B
		/// </summary>
		Vector3 GetRotTranceFormCenter()const { return (translationPos.v1 + translationPos.v2 + translationPos.v3) / 3; }

		TriangleCalcResult GetCalcResult()const { return result; }

		/// <summary>
		/// ���W���Z�b�g���܂��B���_�̏����Ŗ@�����ς��܂��B
		/// </summary>
		/// <param name="pos"></param>
		void SetPosition(const Value3<Vector3>& pos);
		void SetAngle(const Vector3& angle);
		void SetScale(const Vector3& scale);
		/// <summary>
		/// ��]��ɂǂ̂��炢���s�ړ������邩���w�肵�܂��B
		/// </summary>
		/// <param name="vec"></param>
		void SetTranslationPosition(const Vector3& vec);
		
		void SetCalcResult(const TriangleCalcResult& result) { this->result = result; }
	};
#pragma endregion


#pragma region ����3D
	
	struct Segment3DCalcResult
	{
		Vector3 lineSegment3DHitPos;
		Vector3 boardHitPos;
		Vector3 triangleHitPos;
		Vector3 planeHitPos;
	};

	//����
	class Segment3DData
	{
	private:
		Value2<Vector3> position;
		Vector3 angle = 0;
		//��]������߂�ϐ�(0.0f��v1��A0.5f�Œ��S�A1.0f��v2�)
		float rotatePoint = 0.5f;

		Vector3 axisAngle;


		Vector3 axisXVector = MelLib::Vector3(-1, 0, 0);
		Vector3 axisYVector = MelLib::Vector3(0, -1, 0);
		Vector3 axisZVector = MelLib::Vector3(0, 0, -1);

		//angle��K�����������W
		Value2<Vector3> rotatePosition;

		Segment3DCalcResult result;



	private:
		//angle�K���������p�x��rotatePosition�ɑ������֐�
		void CalcRotatePosition();


	public:
		Value2<Vector3> GetPosition()const { return position; }
		Value2<Vector3> GetRotatePosition()const { return rotatePosition; }
		Vector3 GetAngle()const { return angle; }
		float GetRotatePoint()const { return rotatePoint; }
		Segment3DCalcResult GetCalcResult()const { return result; }


		void SetPosition(const Value2<Vector3>& pos);
		void SetAngle(const Vector3& angle);
		/// <summary>
		/// //��]������߂鐔�l(0.0f��v1��A0.5f�Œ��S�A1.0f��v2�)���Z�b�g���܂��B
		/// </summary>
		/// <param name="num">��]������߂鐔�l(0.0f��v1��A0.5f�Œ��S�A1.0f��v2�)</param>
		void SetRotatePoint(const float num);

		/// <summary>
		/// ��������]������̂Ɏg�p����ZXY�����̂̊p�x��ݒ肵�܂��B
		/// </summary>
		/// <param name="angle"></param>
		void SetAxisAngle(const Vector3& angle);

		void SetCalcResult(const Segment3DCalcResult& result) { this->result = result; }

	};



#pragma endregion

#pragma region ���C

	struct RayCalcResult
	{
		Vector3 hitPosition;
		Vector3 hitObjectNormal;
	};

	//��
	class RayData
	{
	private:
		Vector3 position;
		Vector3 direction;

	public:
		Vector3 GetPosition()const { return position; }
		Vector3 GetDirection()const { return direction; }

		void SetPosition(const Vector3& pos) { position = pos; }
		void SetDirection(const Vector3& dir) { direction = dir; }
	};
#pragma endregion


#pragma region �J�v�Z��


	//�J�v�Z��
	class CapsuleData
	{
		// �����̎Q�Ǝ擾���Ȃ��Ă��������悤�ɂ���
		// �p���͂��Ȃ��Ă���
		// �ʂɃJ�v�Z��������̔z��ɓ��ꂽ���킯����Ȃ���
		// GameObject�݂����ɔz��ɂ܂Ƃ߂邽��I�[�o�[���C�h������肪�Ȃ��Ȃ�p�����Ȃ��ق����悩�����͂�

	private:
		Segment3DData segmentData;
		float r = 0.0f;

	public:
		Segment3DData GetSegment3DData()const { return segmentData; }
		float GetRadius()const { return r; }

		Segment3DData& GetRefSegment3DData() { return segmentData; }

		void SetSegment3DData(const Segment3DData& data) { segmentData = data; }
		void SetRadius(const float radius) { r = radius; }

	};

#pragma endregion

#pragma region ����
	
	class FrustumData
	{
		//Board���Ƒ�`�\���ł��Ȃ��Ƃ����Ȃ�

	private:
		Vector3 position;

		Vector3 angle;
		//�㉺�̊p�x
		Vector2 xyAngle = 1.0f;
		//��O
		float nearNum = 0.0001f;
		//��
		float farNum = 1000.0f;

		//������̕���(���ɂ��邩���m�F���邽�߂̂���)
		BoardData nearBoard;
		BoardData farBoard;
		BoardData leftBoard;
		BoardData rightBoard;
		BoardData upBoard;
		BoardData downBoard;

		Vector3 leftAngle;
		Vector3 rightAngle;
		Vector3 upAngle;
		Vector3 downAngle;

		/// <summary>
		/// xyAngle�ɂ���]
		/// </summary>
		void XYRotate();

		/// <summary>
		/// angle�ɂ���]
		/// </summary>
		void AngleRotate();

		void SetBoardPosition();

		/// <summary>
		/// ���ʂ̋����̐ݒ�
		/// </summary>
		void SetPlaneDistance();

	public:
		FrustumData();

		Vector3 GetPosition()const { return position; }
		Vector3 GetAngle()const { return angle; }
		Vector2 GetXYAngle()const { return xyAngle; }
		Vector2 GetNear()const { return xyAngle; }
		Vector2 GetFar()const { return xyAngle; }
		std::vector<BoardData>GetBoardDatas()const;

		
		void SetPosition(const Vector3& pos);
		void SetAngle(const Vector3& angle);
		void SetXYAngle(const Vector2& angle);
		void SetNear(const float nearNum);
		void SetFar(const float farNum);

	
	};
#pragma endregion



#pragma region ��`3D
	//class CircularSector3DData
	//{
	//private:
	//	SphereData sphereData;

	//	//����
	//	Vector3 direction;
	//	//���̊p�x(���v)
	//	float widthAngle = 0.0f;
	//	//�c�̊p�x(���v)
	//	float heightAngle = 0.0f;
	//public:
	//	SphereData GetSphereData()const { return sphereData; }
	//	SphereData& GetRefSphereData() { return sphereData; }
	//	Vector3 GetDirection()const { return direction; }
	//	float GetWidthAngle()const { return widthAngle; }
	//	float GetHeightAngle()const { return heightAngle; }

	//	void SetSphereData(const SphereData& data) { sphereData = data; }
	//	void SetDirection(const Vector3 direction) { this->direction = direction; }
	//	void SetHeightAngleAngle(const float angle) { this->heightAngle = angle; }
	//};
#pragma endregion



#pragma endregion

}