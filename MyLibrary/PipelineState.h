#pragma once
#include"DirectXStruct.h"
#include"ShaderData.h"
#include<string>
#include<unordered_map>
#include<memory>

//�p�C�v���C���͕ʂɃ��f���݂�����static�̃A�N�Z�X�֐��Ƃ�����Ȃ�?
//����Ƃ��A�V�[�����܂����Ŏg���\�����邩�炢��?

namespace MelLib
{
	enum class PipelineStateType
	{
		MODEL,
		SPRITE,
		RENDER_TARGET,
	};
	class PipelineState
	{

	private:
		static std::unordered_map<std::string, std::unique_ptr<PipelineState>>pPipelineState;
		static ID3D12Device* device;


		static void SetPipelineDesc
		(
			const DrawOption& drawData,
			D3D12_GRAPHICS_PIPELINE_STATE_DESC& desc,
			const PipelineStateType type,
			const int renderTargetNum
		);

		static ID3D12RootSignature* modelRootSignature;
		static ID3D12RootSignature* spriteRootSignature;
		static ID3D12RootSignature* renderTargetRootSignature;



		ComPtr<ID3D12PipelineState>pipelineState;
		DrawOption drawData;
		ShaderDataSet shaderSet;
		PipelineStateType pipelineType = PipelineStateType::MODEL;
		std::unique_ptr<std::vector<InputLayoutData>> inputLayoutData = nullptr;
		int renderTargetNum = 1;
	public:
		PipelineState() {}
		PipelineState(PipelineState& pso);
		PipelineState& operator= (PipelineState& pso);

		~PipelineState() {}

		static bool Initialize
		(
			ID3D12Device* dev
		);

		/// <summary>
		/// �p�C�v���C���𐶐����܂�
		/// </summary>
		/// <param name="drawData">�p�C�v���C�����</param>
		/// <param name="shaderSet">�V�F�[�_�[�̃f�[�^���܂Ƃ߂�����</param>
		/// <param name="pipelineType">���̃p�C�v���C���𐶐����邩</param>
		/// <param name="inputLayoutData">�C���v�b�g���C�A�E�g���(�w�肵�Ȃ��ꍇ�́Anullptr��n��)</param>
		/// <param name="renderTargetNum">�����ɂǂ̂��炢�o�͂��邩</param>
		/// <param name="name">�o�^��</param>
		/// <returns></returns>
		static void Create
		(
			const DrawOption& drawData,
			const ShaderDataSet& shaderSet,
			const PipelineStateType pipelineType,
			const std::vector<InputLayoutData>* inputLayoutData,
			const int renderTargetNum,
			const std::string& name
		);

		static PipelineState* Get(const std::string& name) { return pPipelineState[name].get(); }
		static void Delete(const std::string& name);

		/// <summary>
		/// �p�C�v���C���𐶐����܂�
		/// </summary>
		/// <param name="drawData">�p�C�v���C�����</param>
		/// <param name="shaderSet">�V�F�[�_�[�̃f�[�^���܂Ƃ߂�����</param>
		/// <param name="pipelineType">���̃p�C�v���C���𐶐����邩</param>
		/// <param name="inputLayoutData">�C���v�b�g���C�A�E�g���(�w�肵�Ȃ��ꍇ�́Anullptr��n��)</param>
		/// <param name="renderTargetNum">�����ɂǂ̂��炢�o�͂��邩</param>
		/// <returns></returns>
		bool CreatePipeline
		(
			const DrawOption& drawData,
			const ShaderDataSet& shaderSet,
			const PipelineStateType pipelineType,
			const std::vector<InputLayoutData>* inputLayoutData,
			const int renderTargetNum
		);





		ComPtr<ID3D12PipelineState> GetPipelineState()
		{
			return pipelineState;
		}

		/// <summary>
		/// ���C�u�����Ŏg�p���Ă���p�C�v���C���̏�������data�ɃZ�b�g���܂��B
		/// </summary>
		/// <param name="data"></param>
		/// <param name="type"></param>
		static DrawOption GetDefaultDrawData( const PipelineStateType type);

		static void SetModelRootSignature(ID3D12RootSignature* sig) { modelRootSignature = sig; }
		static void SetSpriteRootSignature(ID3D12RootSignature* sig) { spriteRootSignature = sig; }
		static void SetRenderTargetRootSignature(ID3D12RootSignature* sig) { renderTargetRootSignature = sig; }

	};
}
