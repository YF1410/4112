#pragma once
#include"DirectXStruct.h"
#include"Texture.h"
#include"Vector.h"
#include"PipelineState.h"
#include"Camera.h"


#include<d3d12.h>
#include<dxgi.h>
#include<d3dx12.h>
#include<vector>
#include<array>

//���_�g���܂킻�����l�������A
//size�Ƃ�uv�Ƃ�float4�ɂ��Ȃ��Ƃ����Ȃ��āA
//�����I�Ɏg�p�����������������疈�񐶐�����
//�g���܂킳�Ȃ���uv��萔�o�b�t�@�Ŏw�肵�Ȃ��Ƃ����Ȃ�����A
//�������ق�����������
//�l�p�`GPU�ɑ����Ă������番�������ق�����������
//�X�v���C�g��������e�b�Z���[�^�ŕ������ăo���o���ɂ����ق�����������


//������p������2D�A3D�X�v���C�g�N���X�����
//Draw�͉��z�֐��B���ꂼ���map����������
//

//�r���Ńe�N�X�`���ς�����A�ׂ��h��ɂ�����ł���悤�ɂ���
//�������Ȃ��ƃX�v���C�g�t�H���g�̃X�v���C�g���ɗp�ӂł��Ȃ�

//�s�N�Z���P�ʂŊg�k�ł���悤��(������ǂ̂��炢�s�N�Z�����Z���邩���߂���A���ڃs�N�Z�������߂���悤�ɂ��Ă���������)

namespace MelLib
{
	class Sprite
	{
	public:
		enum class SpriteDrawMode
		{
			DRAW_TEXTURE,//�e�N�X�`���`��
			DRAW_COLOR,//�F�`��
		};
	private:

	

		static const UINT MAX_TEXTURE_LOAD_NUM = 256 * 20;


	protected:
		SpriteDrawMode drawMode = SpriteDrawMode::DRAW_COLOR;

		static ID3D12GraphicsCommandList* cmdList;
		static ID3D12Device* device;
		ComPtr<ID3D12PipelineState> pipeline;
		std::array<SpriteVertex, 4> vertices;
		static ComPtr<ID3D12RootSignature>rootSignature;

#pragma region �o�b�t�@

		//�C���f�b�N�X�́A�X�v���C�g���o���o���ɂ����肷�鏈���Ɏg������������������Ƃ�
		//�W�I���g���ł�邩�������
		VertexBufferSet vertexBufferSet;
		ComPtr<ID3D12Resource> constBuffer;

		static std::vector<ComPtr<ID3D12Resource>> textureBuffer;
		static ComPtr<ID3D12DescriptorHeap> textureHeap;
#pragma endregion


		//�`�悷��Ƃ��̍���̍��W
		Vector2 drawLeftUpPosition = 0;
		//�`�悷��Ƃ��̉E���̍��W
		Vector2 drawRightDownPosition = 1;

		//���W�Ȃǂ̏����܂Ƃ߂��\����
		SpriteConstData constData;


		//Color��AddColor�Ƃ��������ɂ܂Ƃ߂邽�߂�Map�֐���Map����̂ŁA�ϐ��p�ӂ���
		Color color;
		Texture* pTexture = nullptr;

	protected:


		void CreateBuffer();

		//�P�F�X�v���C�g�������ɐF���Z�b�g����֐�
		void SetOneColorSpriteColor(const Color& color);

		void ConstDataMat();
		void SetCmdList();

		void MapVertexBuffer(void** data);
		void UnmapVertexBuffer();


		void InitializeVertices();



		virtual void Create(){}

		/// <summary>
		/// �������܂��B
		/// </summary>
		/// <param name="color">�F</param>
		virtual void Create(const Color& color) {}

		/// <summary>
		/// �������܂��B�����_�[�^�[�Q�b�g�̐����͍s���܂���B
		/// </summary>
		/// <param name="pTexture">�e�N�X�`���̃|�C���^</param>
		virtual void Create(Texture* pTexture) {}

	public:
		Sprite();
		~Sprite();

		Sprite(const Sprite& sprite);

		//���������ɒ�`���Ȃ��Ă���
		//�����_�[�^�[�Q�b�g�Ŏg��Ȃ�
		//Draw���烌���_�[�^�[�Q�b�g�Z�b�g�Ȃ����āASprite2D��3D�ɃZ�b�g�֐����?
		virtual void Draw(const std::string& rtName = "");

#pragma region �Z�b�g

		/// <summary>
		/// �\���͈�(UV���W)�̍�����Z�b�g���܂��B1,1�ŉ摜�̈�ԉE�����w�肵�܂��B
		/// </summary>
		/// <param name="leftUpPos"></param>
		void SetDrawLeftUpPosition(const Vector2& leftUpPos)
		{
			drawLeftUpPosition = leftUpPos;
		}

		/// <summary>
		/// �\���͈�(UV���W)�̉E�����Z�b�g���܂��B1,1�ň�ԉE�����w�肵�܂��B
		/// </summary>
		/// <param name="rightDownPos">�E���̍��W</param>
		void SetDrawRigthDownPosition(const Vector2& rightDownPos)
		{
			drawRightDownPosition = rightDownPos;
		}


		void SetDrawMode(const SpriteDrawMode mode);
		

#pragma endregion

#pragma region �Q�b�g
		Vector2 GetDrawLeftUpPosition()const { return drawLeftUpPosition; }
		Vector2 GetDrawRigthDownPosition()const { return drawRightDownPosition; }


#pragma endregion


#pragma region �J���җp�֐�

		static void Initialize(ID3D12Device* dev, ID3D12GraphicsCommandList* cmd);

		static void CreateTextureBuffer(Texture* texture);



#pragma endregion

#pragma region �����ڑ���
		void SetAddColor(const Color& color);
		void SetSubColor(const Color& color);
		void SetMulColor(const Color& color);
#pragma endregion

#pragma region Get
		Color GetColor()const { return color; }
		const Texture* GetTexture()const { return pTexture; }
#pragma endregion


#pragma region �p�C�v���C��

		void SetPipeline(PipelineState* pipelineState);

#pragma endregion



	};
}
