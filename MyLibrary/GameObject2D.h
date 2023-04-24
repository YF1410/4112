#pragma once
#include"Vector.h"
#include"Sprite2D.h"
#include"CollisionDetectionData.h"

#include<unordered_map>

namespace MelLib 
{
	class GameObject2D
	{
	private:

		MelLib::Vector2 position;

		CircleData hitCircleData;
		RectData hitRectData;
		Segment2DData hitSegmentData;
		
		void MoveSpritePosition(const Vector2& vec);
		void MoveDataPosition(const Vector2& vec);

	protected:
		std::unordered_map<std::string, MelLib::Sprite2D>sprites;

		std::vector<MelLib::CircleData>circleDatas;
		std::vector<MelLib::RectData>rectDatas;
		std::vector<MelLib::Segment2DData>segmentDatas;

		CollisionDetectionFlag2D collisionFlags;
		std::vector<std::string>tags;
		bool eraseManager = false;
	public:
		//�R���X�g���N�^
		GameObject2D();
		//�f�X�g���N�^
		virtual ~GameObject2D();

		//�X�V
		virtual void Update();
		//�`��
		virtual void Draw();

		/// <summary>
		/// �����������̏���
		/// </summary>
		/// <param name="object">����I�u�W�F�g�̃|�C���^</param>
		/// <param name="collisionType">�����̂ǂ̔���ɓ���������</param>
		/// <param name="arrayNum">�����̉��ڂ̔���ɓ���������</param>
		/// <param name="hitObjColType">����̂ǂ̔���ɓ���������</param>
		/// <param name="hitObjArrayNum">����̉��ڂ̔���ɓ���������</param>
		virtual void Hit
		(
			const GameObject2D* const pObject,
			const ShapeType2D& dataType,
			const int num,
			const ShapeType2D& hitObjDataType,
			const int hitNum
		);



		void AddPosition(const MelLib::Vector2& vec);

#pragma region �Q�b�g

		MelLib::Vector2 GetPosition()const { return position; }

		bool GetEraseManager()const { return eraseManager; }

		std::vector<std::string> GetTags()const { return tags; }
#pragma region ����֌W
		CollisionDetectionFlag2D GetCollisionFlag()const { return collisionFlags; }
		std::vector<CircleData>GetCircleData()const { return circleDatas; }
		std::vector<RectData>GetRectData()const { return rectDatas; }
		std::vector<Segment2DData>GetSegment2DData()const { return segmentDatas; }
#pragma endregion


#pragma endregion

#pragma region �Z�b�g
		void FalseEraseManager();

		void SetPosition(const MelLib::Vector2& pos);

#pragma region ����֌W
		void SetHitCircleData(const CircleData& data) { hitCircleData = data; };
		void SetHitRectData(const RectData& data) { hitRectData = data; };
		void SetHitSegmentData(const Segment2DData& data) { hitSegmentData = data; };
#pragma endregion

#pragma endregion

	};
}
