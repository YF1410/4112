#pragma once
#include<d3d12.h>
#pragma comment(lib,"d3d12.lib")
#include<wrl.h>
#include<tuple>
#include<map>

#include"Color.h"
#include"Values.h"
#include"Texture3D.h"
#include"PipelineState.h"

namespace MelLib
{

	// �����G���A
	//Material���p�����ăp�����[�^�𑝂₹��悤�ɂ���
	//�p�����Ă��ǂ�����ăV�F�[�_�[�ɓn��?���p�҂�Map���Ă��炤�����Ȃ�?
	//�萔�o�b�t�@�����R�ɍ���悤�ɂ����ق�������?

	//�}�e���A�����̒萔�o�b�t�@���}�e���A���N���X�Ɏ�������B
	//��������΁A���f���ɕ����̃}�e���A����񎝂�����K�v���Ȃ��B
	//�}�e���A���̊��N���X����Ă����ɒ萔�o�b�t�@�������Ȃ��Ƃ����Ȃ�

	//�q�[�v�͂ǂ��Ɏ�������?
	//�e�N�X�`���o�b�t�@�ȊO�Ɏ���������̂��肻���������烂�f����

	//ModelData����������ǂݍ��񂾂�A�����ŏ����}�e���A�����B

	//�}�e���A���̃f�[�^�̍\���̂����������A
	//�}�e���A���\���̂Ƀe���v���[�g�g�����}�e���A����񎝂�����?�ǂݎ���?�}�b�v�͓ǂݎ��Ȃ��Ă��ł���?
	//�z��ɂ܂Ƃ߂��Ȃ����番����
	//���C�u�����Œ�`����Ă���̂��������ŊǗ�����d�l�ɂ���?
	//���p�҂��p�ӂ����}�e���A���͎����ŊǗ����Ă��炤
	//�e���v���[�g��������������킯��

	//�}�e���A���̌^�ɍ��킹�ăp�C�v���C���������Ɏg�p����V�F�[�_�[�ς���?
	//�}�e���A���Ƀp�C�v���C����������?
	//�p�C�v���C���Ƀ����_�����O�̏��(�J�����O�Ƃ������_�[�^�[�Q�b�g��)�Ƃ������āA
	//�}�e���A���͂����܂Ō����ڂ̏�����������炢���B

	//�}�e���A���̃N���X�w�肷��ƁA���̃}�e���A���ɓK�����V�F�[�_�[�Ńp�C�v���C�������@�\�͂����Ă���������
	//�}�e���A�����Ƃɒ萔�o�b�t�@�ƃe�N�X�`���o�b�t�@�̐��ς�邩��A���[�g�V�O�l�`�����������Ⴄ? 
	//�e���v���[�g�̂��Create�ĂׂȂ��B
	//�p�����č���Ă��炤?

	//�}�e���A���Ƀp�C�v���C����������
	//�p�C�v���C����protected�Ŏ�������B�J�����O�����̐ݒ�Ƃ��̍\���̂�private�Ɏ�������B

	//�Ƃ肠�������̐i�߂悤



	//���ʂ̃}�e���A�������܂Ƃ߂����N���X�B
	class Material
	{
	public:

		enum class MaterialConstBufferType
		{
			MATERIAL_DATA,
			COLOR
		};

	private:

		template<class T>
		using ComPtr = Microsoft::WRL::ComPtr<T>;

		static ID3D12Device* device;

		//�}�e���A���̒萔�o�b�t�@
		ComPtr<ID3D12Resource>colorBuffer = nullptr;
		ComPtr<ID3D12Resource>materialBuffer = nullptr;

		//(�}�e���A���̃J���[ + AddColor + SubColor)*MulColor = �ǉ��F
		Color color = Color(255, 255, 255, 255);

		//�e�N�X�`���ǉ�������f�X�g���N�^�Ƃ���Load�֐��œǂݍ��񂾃e�N�X�`���̊J����������������
		//�����ꏊ�́A�f�X�g���N�^�ASet�֐��ALoad�֐��̎��s���ɓ���if����3�ӏ�
		//�e�N�X�`�����|�C���^��SetTextureType��������tuple�ɂ���ΖY��Ȃ�����
		//�e�N�X�`��
		static const UINT TEXTURE_HANDLE_NUM = 0;
		Texture* pTexture = nullptr;
		// �V�F�[�_�[���Ŏ󂯎��Ƃ���map�̕����w�肵�₷�����Ȃ̂ŁAmap�ɂ���
		std::map<std::string, Texture*>pTextures;

		static const UINT NORMAL_MAP_HANDLE_NUM = 1;
		Texture* pNormalMapTexture = nullptr;

		/*static const UINT TEXTURE_3D_HANDLE_NUM = 2;
		Texture3D* pTexture3D = nullptr;*/


		//�e�N�X�`���N���X�Ɏ������邽�߁A�R�����g�A�E�g
		//ComPtr<ID3D12Resource>textureBuffer;

		//�ǂ��Ɏ�������̂��x�X�g���킩��Ȃ����߁A�ۗ�
		//�Ƃ肠�����}�e���A���Ɏ�������(���f�����Ƃɗp�ӂ���̂͂��������Ȃ�)
		//UAV���e�N�X�`���Ɠ����q�[�v�ɂ܂Ƃ߂�K�v���o�Ă�����A
		//�R���s���[�g�V�F�[�_�[���g���I�u�W�F�N�g�̂ݐ�p�̃q�[�v���?
		//�����ɍ��ƕ����̃}�e���A���̃e�N�X�`�����܂Ƃ߂��Ȃ�
		//�t�@�C���ɕ������f�������Ă������_�����O����^�C�~���O���Ⴄ����܂Ƃ߂Ȃ��Ă����Ȃ�
		ComPtr<ID3D12DescriptorHeap>textureHeap = nullptr;

		//�e�N�X�`�����Z�b�g���ɃZ�b�g����e�N�X�`���o�b�t�@
		static ComPtr<ID3D12Resource> textureNoneTextureBuffer;

		// �e�N�X�`�������̍ő�l
		static const unsigned int TEXTURE_MAX = 8;
	private:
		void MapColorBuffer(const Color& color);

		//���C���̃e�N�X�`�����Z�b�g�܂��͓ǂݍ��񂾂Ƃ��ɕK���Ăяo������
		void SetOrLoadTextureProcess();
	protected:
		std::unique_ptr<PipelineState> pipelineState;
		DrawOption drawData;
		unsigned int textureNumMax = 0;

	protected:
		// �G���W���Ƃ����ƃ}�e���A���������Ƀe�N�X�`���ǂݍ���ŃZ�b�g�ł���B
		// ������Č����邽�߂ɁA�e�N�X�`�����ォ��Z�b�g�ł���悤�ɂ���B
		// �������A��Ƀq�[�v�̐����Z�b�g���Ȃ��Ƃ����Ȃ����߁A�����Ɏw�肷��悤�ɂ���
		// ���Ƃ������̂ق������������������邩��s������

		/// <summary>
		/// �}�e���A���������̏������������s���܂��B
		/// </summary>
		/// <param name="mtlByte">�\���̂̃o�C�g��</param>
		void CreateInitialize(const size_t& mtlByte, const unsigned int textureNum);


		void MapMaterialData(void** pData);
		void UnmapMaterialData();
	public:

		Material() {}
		virtual ~Material() {}


		static void Initialize(ID3D12Device* dev);

		virtual void Create(const DrawOption& drawData, const unsigned int textureNum) {}
		virtual void Create(const DrawOption& drawData, const unsigned int textureNum, const ShaderDataSet& shader) {}

		ID3D12DescriptorHeap* GetPTextureHeap() { return textureHeap.Get(); }
		ID3D12Resource* GetPConstBuffer(const MaterialConstBufferType type)const;
		Texture* GetPTexture(const std::string& name = "");
		void GetPTextures(std::vector<Texture*>& refTex) {};
		Texture* GetPNormalTexture() { return pNormalMapTexture; }
		//Texture3D* GetPTexture3D() { return pTexture3D; }
		PipelineState* GetPPipelineState() { return pipelineState.get(); }

		const std::map<std::string, Texture*>& GetRefPTextures()const { return pTextures; }

		void SetColor(const Color& color);
		/// <summary>
		/// �e�N�X�`���̓o�^�A�܂��͌������s���܂��B
		/// </summary>
		/// <param name="pTex">�e�N�X�`����</param>
		/// <param name="name">�o�^��</param>
		/// <returns></returns>
		bool SetTexture(Texture* pTex, const std::string& name = "Default");
		//void SetTexture(const std::vector<Texture*>& pTex);
		//void SetTexture(const std::vector<Texture*>& pTex);
		//void SetNormalMapTexture(Texture* pNormalMapTex);
		//void SetTexture3D(Texture3D* pTex);


		unsigned int GetTextureNum()const { return textureNumMax; }
		static unsigned int GetTextureNumMax() { return TEXTURE_MAX; }
	};

	//// ���R�ɐݒ�ł���}�e���A��
	//class OriginalMaterial :public Material
	//{
	//public:
	//	OriginalMaterial() {}
	//	OriginalMaterial(OriginalMaterial& mtl);
	//	OriginalMaterial& operator=(OriginalMaterial& mtl);

	//	void Create(const DrawOption& drawData, const unsigned int textureNum);


	//};

#pragma region ADSA

	struct ADSAMaterialData
	{


		//����
		Value4<float>ambient = 0.3f;
		//�g�U���ˌ�
		Value4<float>diffuse = 0.7f;
		//���ʔ��ˌ�
		Value4<float>specular = 0.0f;
		//�A���t�@�l
		float alpha = 1.0f;

	};


	//Ambient�ADiffuse�ASupecular�A���f������ǂݎ����Alpha�����������}�e���A��
	class ADSAMaterial :public Material
	{
	private:
		//static std::unordered_map<std::string, std::unique_ptr<ADSAMaterial>>pMaterials;


		ADSAMaterialData materialData;
		void Map();
	public:
		ADSAMaterial() {}
		ADSAMaterial(ADSAMaterial& mtl);
		ADSAMaterial& operator=(ADSAMaterial& mtl);

		void Create(const DrawOption& drawData, const unsigned int textureNum)override;
		void Create(const DrawOption& drawData, const unsigned int textureNum, const ShaderDataSet& shader)override;
#pragma region �Z�b�g
		void SetMaterialData(const ADSAMaterialData& data);
#pragma endregion

#pragma region �Q�b�g
		ADSAMaterialData GetMaterialData() { return materialData; }
#pragma endregion


	};
#pragma endregion

#pragma region PBR

	struct PBRMaterialData
	{
		//�A���x�h��Material�ɒ�`

		//�����x(0�܂���1)
		float metalness = 1.0f;
		//���ʔ��ˌ�(float 0����1)
		float fSpecular = 0.5f;
		//�e��
		float roughness = 0.0f;
	};

	//PBR�}�e���A��
	class PBRMaterial :public Material
	{
	private:
		PBRMaterialData materialData;

		void Map();
	public:
		PBRMaterial() {}
		PBRMaterial(PBRMaterial& mtl);
		PBRMaterial operator=(PBRMaterial& mtl);

		void Create(const DrawOption& drawData, const unsigned int textureNum)override;

#pragma region �Z�b�g
		void SetMaterialData(const PBRMaterialData& data);
#pragma endregion

#pragma region �Q�b�g
		PBRMaterialData GetMaterialData() { return materialData; }
#pragma endregion


	};

#pragma endregion


}
