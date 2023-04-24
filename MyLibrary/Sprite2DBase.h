#pragma once
#include"Sprite.h"
#include"PipelineState.h"

namespace MelLib
{
	//�����_�[�^�[�Q�b�g�𕪗������������
	//�X�v���C�g2D���N���X
	class Sprite2DBase :public Sprite
	{
		
	private:

		//2D�̃J�����͌Œ肾����A�s�񐶐��֐����̃N���X�Ɏ������Ă���������
		static DirectX::XMMATRIX cameraMatrix;

		DirectX::XMFLOAT2 scalingPoint = DirectX::XMFLOAT2(0,0);
		DirectX::XMFLOAT2 rotationPoint = DirectX::XMFLOAT2(0,0);
	protected:


		static PipelineState defaultPipeline;


		void MatrixMap(Texture* texture);

		//void Create(const Color& color)override;
		//void Create(Texture* pTexture)override;

		//��̊֐���Sprite2D�Ɉړ�
		//�V���������֐�(Sprite2D�ART�Ɏg����֐�)
		void SpriteInitialize();
	public:
		Sprite2DBase() {}
		//Sprite2DBase(const Color& color);
		//Sprite2DBase(Texture* pTexture);
		virtual ~Sprite2DBase() {}


		static bool Initialize(const int winWidth, const int winHeight);


		//void Draw(const std::string& rtName = "")override;



#pragma region ����
		void SetPosition(const Vector2& pos) { constData.position = { pos.x,pos.y,0 }; }

		void SetAngle(const float& angle) { constData.angle = { 0,0,angle }; }

		/// <summary>
		/// ��]����Ƃ��̊���W��ݒ肵�܂��B
		/// </summary>
		/// <param name="point">��]�(0,0�ō������ɉ�])</param>
		void SetRotationPoint(const Vector2& point) { rotationPoint = point.ToXMFLOAT2(); }

		void SetScale(const Vector2& scale) { constData.scale = scale.ToXMFLOAT2(); }
		
		/// <summary>
		/// �g�k����Ƃ��̊���W��ݒ肵�܂��B
		/// </summary>
		/// <param name="point">�g�k�(0,0�ō������Ɋg�k)</param>
		void SetScalingPoint(const Vector2& point) { scalingPoint = point.ToXMFLOAT2(); }
#pragma endregion

#pragma region �Q�b�g
		Vector2 GetPosition()const { return Vector2(constData.position.x, constData.position.y); }

		float GetAngle()const { return constData.angle.z; }
		Vector2 GetScale()const { return Vector2(constData.scale.x, constData.scale.y); }
#pragma endregion



		static PipelineState GetDefaultPipeline() { return defaultPipeline; }
	};
}
