#pragma once
#include<vector>
#include<array>

#include"Sprite2DBase.h"
#include"DirectXStruct.h"
#include"Vector.h"
#include"PipelineState.h"
#include"Camera.h"

// �����_�[�^�[�Q�b�g���ǂ��ɕ`�悷�邩���w��ł���悤�ɂ���
// �Ƃ肠���������_�[�^�[�Q�b�g�D��B�I�u�W�F�N�g�̕`��ꏊ�͌��
// �����_�[�^�[�Q�b�g�Ɏ��̃����_�[�^�[�Q�b�g�̃|�C���^��n���񂶂�Ȃ��āA
// �`�揇���L�q���ēn���悤�ɂ���?
// �����_�����O����摜,�`��Ώۂ̔z����Ċ����Ŏw�肷��?(�z�񂶂�Ȃ���8�ϐ����?)
// �|�C���^�n��?���O�n��?
// ��񂸂n�����ɁA�܂Ƃ߂��̂𑗂��Ă�������ق�������
// 
// �擪���ǂ����L�^�����ق�������?

// �����_�[�^�[�Q�b�g�̃q�[�v�܂Ƃ߂Ă�������
// ����΁A�}���`�e�N�X�`�������R�ɂł���

// �X�v���C�g�N���X�ƕ��������ق�����������
// �X�v���C�g�N���X�Ń����_�[�^�[�Q�b�g�C���N���[�h���Ȃ��ƃ����_�����O��w��ł��Ȃ��Ȃ�\������
//����܂ŁA�����_�[�^�[�Q�b�g�����쐬�s��


namespace MelLib
{
	class RenderTarget
	{
	public:
		struct RenderTargetDrawData
		{
			//�����_�[�^�[�Q�b�g
			RenderTarget* rt = nullptr;
			//�����_�[�^�[�Q�b�g��`�悷�郌���_�[�^�[�Q�b�g
			RenderTarget* renderingRT = nullptr;
		};

	private:
		static DirectX::XMMATRIX cameraMatrix;

		static ID3D12GraphicsCommandList* cmdList;
		static ID3D12Device* device;
		ComPtr<ID3D12PipelineState> pipeline;
		std::array<SpriteVertex, 4> vertices;

		//���W�Ȃǂ̏����܂Ƃ߂��\����
		SpriteConstData constData;

		VertexBufferSet vertexBufferSet;
		ComPtr<ID3D12Resource> constBuffer;

		static std::vector<RenderTargetDrawData> rtDrawData;

		static const int RT_NUM = 1;

		static std::unordered_map<std::string, std::unique_ptr<RenderTarget>>pRenderTargets;
		static UINT createCount;
		static std::string mainRenderTargetNama;

		static float sClearColor[4];
		static PipelineState defaultPipeline;
		static ComPtr<ID3D12RootSignature>rootSignature;

		//���ݕ`��ΏۂɂȂ��Ă��郌���_�[�^�[�Q�b�g(�}���`�Ή��̂��߁A�z��ɂ��Ă���)
		static RenderTarget* pCurrentSelectRTs;

		ComPtr<ID3D12Resource>textureBuffer[RT_NUM];
		ComPtr<ID3D12DescriptorHeap>descHeap;//�e�N�X�`��(�����_�����O����) + �|�X�g�G�t�F�N�g�̒萔�o�b�t�@�r���[

		ComPtr<ID3D12DescriptorHeap>rtvHeap;

		ComPtr<ID3D12Resource>depthBuffer;
		ComPtr<ID3D12DescriptorHeap>depthHeap;

		//�J�����̃|�C���^
		Camera* pCamera = nullptr;
		float clearColor[4] = { 0.0f,0.0f,0.0f,0.0f };

	private:
		void ConstDataMat();
		void MatrixMap();
	public:

		RenderTarget(const Color& color);
		~RenderTarget();

		/// <summary>
		/// �����_�[�^�[�Q�b�g��`�悷�郌���_�[�^�[�Q�b�g���w�肵�܂��B
		/// </summary>
		/// <param name="data">�`����</param>
		static void SetRenderTargetDrawData(const std::vector<RenderTargetDrawData>& data) { rtDrawData = data; }

		static void Create(const Color& initColor, const std::string& name = "");
		static void Delete(const std::string& name);
		static RenderTarget* Get(const std::string& name = mainRenderTargetNama) { return pRenderTargets[name].get(); }

		static bool Initialize(ID3D12Device* dev,ID3D12GraphicsCommandList* list);

		//����private��
		void PreDrawProcess();
		void SetCmdList();
		static void AllDraw();
		//PreDrawProcess���Ăяo���֐�
		static void DrawBegin();
		static void MainRTDraw();

		static void ChangeCurrentRenderTarget(RenderTarget* pRTs);

#pragma region �Z�b�g

		/// <summary>
		/// �J�������Z�b�g���܂��B
		/// </summary>
		/// <param name="name"></param>
		void SetCamera(Camera* pCamera) { this->pCamera = pCamera; }

		void SetPosition(const Vector2& pos) { constData.position = DirectX::XMFLOAT3(pos.x, pos.y, 0); }
		void SetScale(const Vector2& scale) { constData.scale = scale.ToXMFLOAT2(); }
		void SetAngle(const float angle) { constData.angle = DirectX::XMFLOAT3(0, 0, angle); }

#pragma endregion


		static const std::string& GetMainRenderTargetNama() { return mainRenderTargetNama; }

		Camera* GetCamera() { return pCamera; }

	};

}