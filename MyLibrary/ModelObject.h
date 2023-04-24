#pragma once
#include"Vector.h"
#include"Color.h"
#include"DirectXStruct.h"

#include<DirectXMath.h>
#include<vector>
#include<memory>
#include<d3d12.h>
#include<d3dx12.h>
#include<dxgi.h>


#include"ModelData.h"
#include"PipelineState.h"
#include"Texture.h"
#include"Camera.h"
#include"RenderTarget.h"
#include"BufferData.h"
#include"CollisionDetectionData.h"
#include"Material.h"

#include"GuiValue.h"
//AddColor�Ƃ��܂Ƃ߂悤!

namespace MelLib
{
	//���f���̍��W�Ȃǂ��܂Ƃ߂�����
	class ModelObject
	{
	private:
		// ���O,������
		static std::unordered_map<std::string, int>createCount;

	private:

		static std::unordered_map<std::string, std::unique_ptr<ModelObject>>pModelObjects;

		static ID3D12Device* device;
		static std::vector<ID3D12GraphicsCommandList*>cmdLists;
		static ComPtr<ID3D12RootSignature>rootSignature;
	
		std::vector<std::string>objectNames;

		//[���f�����̃I�u�W�F�N�g����]
		std::unordered_map < std::string, Material*>materials;

#pragma region �{�[���ƃA�j���[�V�����̏��
		// �{�[���̍ő吔
		static const UINT BONE_MAX = 64;
		struct SkinConstBufferData
		{
			DirectX::XMMATRIX bones[BONE_MAX];
		};

		struct FbxAnimationTimes
		{
			FbxTime startTime;
			FbxTime endTime;
			//1�t���[���̎���
			FbxTime frameTime;
		};

		//fbx���f���̃A�j���[�V�����p�̏����܂Ƃ߂�����
		struct FbxAnimationData
		{
			FbxAnimationTimes animationTimes;
			FbxTime currentTime;
			int timeMag = 1;
			std::string currentAnimationName;
		};


		//[�{�[������]
		// ���ꂨ���炭obj�p
		std::vector<BoneData>boneDatas;
		std::vector<ParentBoneData> parentBoneDatas;//�e�{�[���Ɖe���x

		FbxAnimationData fbxAnimationData;
		bool isAnimation = false;
		bool animationEndStop = false;
		bool animationReverse = false;
		bool animationEnd = false;
#pragma endregion

		//�萔�o�b�t�@
		static const int CONST_BUFFER_REGISTER = 0;
		std::vector<ComPtr<ID3D12Resource>> constBuffer;//���C��(��{�I�ȏ��)

		static const int MATERIAL_BUFFER_REGISTER = 2;

		static const int COLOR_MATERIAL_BUFFER_REGISTER = 4;

		static const int USER_BUFFER_REGISTER = 1;
		std::vector<ComPtr<ID3D12Resource>> userConstBuffer;//���[�U�[
		ConstBufferData userConstBufferData;

		static const int MODEL_BUFFER_REGISTER = 3;
		// ���f���̃I�u�W�F�N�g���Ƃɐ�������悤�ɕύX���邱�� 2021 12/8
		std::vector<ComPtr<ID3D12Resource>> modelConstBuffer;//���f�����L(�A�j���[�V�����֌W�̏��)
		ConstBufferData modelConstBufferData;

		static const int TEXURE_ROOTPARAM_NUM = 5;

		//�萔�ɃZ�b�g������W�Ȃǂ̒l
		//[���f�����̃I�u�W�F�N�g��]
		std::unordered_map < std::string , ModelConstData > modelConstDatas;

		//���f��(�N���G�C�g�����犄�蓖�Ă�)
		ModelData* pModelData = nullptr;
		UINT modelFileObjectNum = 0;



		//�e�N�X�`�����Z�b�g���ɃZ�b�g���铧���̃e�N�X�`���o�b�t�@
		static ComPtr<ID3D12Resource>colorZeroTexBuffer;

		//�����錾����ModelObject��vector�Ƃ�resize����ƃG���[�o��B�΍􂵂�
		//ModelData���������Ȃ��Ń��f���f�[�^�n�������Ɏg���f�[�^���������Ă���΂���?
		//�ꎞ�I��ModelData�����ModelObject�ɃZ�b�g���Ă����������Ⴄ
		//�n���ƂȂ�ƁA�������邲�Ƃɒ��_�̃R�s�[���K�v�ɂȂ�(�R�s�[���Ȃ��ƃJ�b�g�������f���̒��_�Q�Ƃł��Ȃ�)
		//����ModelObject�ɒ��_�Ƃ��n���č���悤�ɂ��Ă���������
		//���_�ƃC���f�b�N�X�֌W�̂��̂������܂Ƃ߂��N���X������āA���������Ɏ�������̂�����
		
		// //ModelData�Ȃ��ƃq�[�v�p�ӂł��Ȃ�����A
		//�}�e���A���Ƃ��̃R�s�[�R���X�g���N�^������ق�����������
		std::unique_ptr<ModelData> catFrontModelData;
		std::unique_ptr<ModelData> catBackModelData;
		//ModelData* catFrontModelData;
		//ModelData* catBackModelData;

		std::string objectName;

		/*GuiVector3 guiPosition;
		GuiVector3 guiAngle;
		GuiVector3 guiScale;*/
		// ������GUI�Ƀf�[�^���Z�b�g���邩�ǂ���(�R�s�[�R���X�g���N�^��R�s�[������Z�q���g���ꍇ�R�s�[��̖��O���g���������߁Afalse�ɂ���)
		//bool autoSetGuiData = true;

	private:

		void CreateConstBuffer();

		void DrawCommonProcessing(const std::string& rtName);
		void MapConstData(const Camera* camera);
		void SetCmdList();

		void FbxAnimation();

	public:

		//nullptr�n�����\�����l����ƁAbool��return�ł���悤�ɂ����ق�������?
		ModelObject() 
		{
		}

		ModelObject(ModelObject& obj,const std::string& name);
		ModelObject(ModelObject& obj);
		//ModelObject& operator= (ModelObject& obj);
		~ModelObject() {}

		enum class CopyModelObjectContent 
		{
			ALL,
			NUMBER_FLAG,
		};
		bool CopyModelObject(const ModelObject& obj, const std::string& name, CopyModelObjectContent copyContent);


		static bool Initialize(ID3D12Device* dev, const std::vector<ID3D12GraphicsCommandList*>& cmdList);


		static ModelObject* Get(const std::string& name) { return pModelObjects[name].get(); }
		static void Delete(const std::string& name);

		/// <summary>
		/// �A�j���[�V�����Ȃǂ̍X�V�������s���܂�
		/// </summary>
		void Update();
		void Draw(const std::string& rtName = RenderTarget::GetMainRenderTargetNama());

#pragma region 

#pragma region ���f���Ő���
		ModelObject(ModelData* pModelData, const std::string& objectName,  ConstBufferData* userConstBufferData = nullptr);
		static bool Create(ModelData* pModelData, const std::string& objectName , ConstBufferData* userConstBufferData, const std::string& name);
		bool Create(ModelData* pModelData,const std::string& objectName , ConstBufferData* userConstBufferData = nullptr);
#pragma endregion 


#pragma region �v���~�e�B�u���f������
	

#pragma endregion 


#pragma endregion 

#pragma region ���b�V���J�b�g
		/// <summary>
		/// ���f���𕽖ʂɉ����Đؒf���܂��B
		/// </summary>
		/// <param name="plane">����</param>
		/// <param name="pFront">���ʂ̕\���ɂ��郂�f�������i�[����ModelData�̃|�C���^</param>
		/// <param name="pBack">���ʂ̗����ɂ��郂�f�������i�[����ModelData�̃|�C���^<</param>
		/// <param name="createCrossSection">�f�ʂ��`�����邩�ǂ���</param>
		/// <returns>�ؒf�ł������ǂ���</returns>
		bool MeshCat(const PlaneData& plane,ModelData*& pFront, ModelData*& pBack,const bool createCrossSection);
#pragma endregion


#pragma region �Z�b�g

#pragma region ���쌩���ڕύX

#pragma region ����


		void SetPosition(const Vector3& position,const std::string& name = "");
		void SetScale(const Vector3& scale, const std::string& name = "");
		void SetAngle(const Vector3& angle, const std::string& name = "");

		void SetAddUV(const MelLib::Vector2& uv, const std::string& name = "");

#pragma endregion

#pragma region �F
		/// <summary>
		/// �F�����Z���܂��B
		/// </summary>
		/// <param name="color">�F</param>
		/// <param name="name">���f�����̃I�u�W�F�N�g��(�w�肵�Ȃ��ꍇ�A1�Ԗڂ̃I�u�W�F�N�g���w��)</param>
		void SetAddColor(const Color& color, const std::string& name = "");
		void SetSubColor(const Color& color, const std::string& name = "");
		void SetMulColor(const Color& color, const std::string& name = "");
#pragma endregion


#pragma region �A�j���[�V����

		void SetAnimationPlayFlag(const bool flag) { isAnimation = flag; }

		/// <summary>
		/// �A�j���[�V���������Z�b�g���܂��B
		/// </summary>
		void SetAnimationFrameStart() {fbxAnimationData.currentTime = 0;}

		/// <summary>
		/// �A�j���[�V�����̌��݂̃t���[�����A�j���[�V�����I�����̃t���[���ɂ��܂��B
		/// </summary>
		void SetAnimationFrameEnd() { fbxAnimationData.currentTime = fbxAnimationData.animationTimes.endTime; }

		/// <summary>
		/// �A�j���[�V�����̃t���[�����Z�b�g���܂��B
		/// </summary>
		/// <param name="fream"></param>
		void SetCurrentFream(const UINT fream);

		/// <summary>
		/// �A�j���[�V�����̍Đ����x���Z�b�g���܂��B
		/// </summary>
		/// <param name="magnification"></param>
		void SetAnimationSpeedMagnification(const unsigned int magnification) { fbxAnimationData.timeMag = magnification; }

		/// <summary>
		/// �A�j���[�V�������t�Đ����邩�ǂ�����ݒ肵�܂��B
		/// </summary>
		/// <param name="flag"></param>
		void SetAnimationReversePlayBack(const bool flag);

		/// <summary>
		/// �A�j���[�V�������w�肵�܂��B
		/// </summary>
		/// <param name="name"></param>
		void SetAnimation(const std::string& name);

		/// <summary>
		/// �A�j���[�V�����I�����ɍĐ����I�����邩���w�肵�܂��B
		/// </summary>
		/// <param name="flag"></param>
		void SetAnimationEndStopFlag(const bool flag) { animationEndStop = flag; }

		/// <summary>
		/// �A�j���[�V�����̃t���[�����Z�b�g���܂��B
		/// </summary>
		/// <param name="frame"></param>
		void SetAnimationFrame(const unsigned int frame) { fbxAnimationData.currentTime.SetFrame(frame, FbxTime::eFrames60); }

#pragma endregion

		void SetGUIValueData(const std::string& name);

#pragma endregion

		/// <summary>
		/// �V�F�[�_�[�Ŕėp�I�Ɏg����p�[�Z���g�̒l��ύX���܂��B
		/// </summary>
		void SetPar(float par, const std::string& name = "");

		/// <summary>
		/// �}�e���A�����Z�b�g���܂��B
		/// </summary>
		/// <param name="mtl"></param>
		/// <param name="name"></param>
		void SetMaterial(Material* mtl,const std::string& name = "");




#pragma endregion

#pragma region �Q�b�g

		//���̕�const�ɂ���

		ModelData* GetPModelData() { return pModelData; }
		Material* GetPMaterial(const std::string& name = "");

#pragma region ���쌩���ڕύX

#pragma region ����
		
		//���͂Ƃ肠�����S���l���ꏏ�Ȃ̂ŁA0�̂��Ԃ��Ă�
		Vector3 GetPosition(const std::string& name = "")const;

		Vector3 GetAngle(const std::string& name = "")const;
		Vector3 GetScale(const std::string& name = "")const;

	    MelLib::Vector2 GetAddUV( const std::string& name = "")const;
#pragma endregion
#pragma endregion


#pragma region �A�j���[�V����


		std::string GetCurrentAnimationName()const { return fbxAnimationData.currentAnimationName; }

		/// <summary>
		/// �A�j���[�V�������t���������ǂ������擾���܂��B
		/// </summary>
		/// <returns></returns>
		bool GetAnimationReversePlayBack()const { return animationReverse; }

		/// <summary>
		/// �A�j���[�V�������I�����Ă��邩�ǂ������擾���܂��B
		/// </summary>
		/// <returns></returns>
		bool GetAnimationEndFlag()const { return animationEnd; }
		//{ return fbxAnimationData.currentTime == fbxAnimationData.animationTimes.endTime; }


		/// <summary>
		///	�A�j���[�V�����̌��݂̃t���[�����擾���܂��B
		/// </summary>
		/// <returns></returns>
		unsigned int GetAnimationFrame()const { return static_cast<unsigned int>(fbxAnimationData.currentTime.GetFrameCount(FbxTime::eFrames60)); }
		
		/// <summary>
		/// �A�j���[�V�����̃t���[�������擾���܂��B
		/// </summary>
		/// <returns></returns>
		unsigned int GetAnimationFrameCount()const { return static_cast<unsigned int>(fbxAnimationData.animationTimes.endTime.GetFrameCount(FbxTime::eFrames60)); }
#pragma endregion

		//�R���s���[�g�V�F�[�_�[�Ōv�Z�����ق��������B
		//�ł������Ȃ�`�掞�ɒ��_�V�F�[�_�[�Ōv�Z�������ʂ������Ă����ق�������?
		// �߂�l��umap�ł̂����Ă���������
		/// <summary>
		/// ���_���W���擾���܂��B
		/// </summary>
		/// <param name="scaleImpact"></param>
		/// <param name="angleImpact"></param>
		/// <param name="transformImpact"></param>
		/// <param name=""></param>
		/// <returns></returns>
		std::vector<std::vector<Vector3>>GetVerticesData
		(
			const bool scaleImpact,
			const bool angleImpact,
			const bool transformImpact
		);

		/// <summary>
		/// ���f���̃|���S���𓖂��蔻��Ɏg����f�[�^�ɕϊ����ĕԂ��܂��B
		/// </summary>
		void GetModelTriangleData(std::vector<std::vector<TriangleData>>& vec)const;
#pragma endregion

		/// <summary>
		/// �w�肵���{�[���ƃ��b�V���̍s���n�������W�ɏ�Z���ĕԂ��܂��B
		/// </summary>
		/// <param name="pos">���W</param>
		/// <param name="boneName">�{�[����</param>
		/// <param name="meshName">���b�V����</param>
		/// <returns>�s���Z��̍��W</returns>
		Vector3 CalcAnimationPosition
		(
			const Vector3& pos,float weigth, const std::string& boneName,const std::string& meshName
		, const MelLib::Vector3& startPos, const MelLib::Vector3& startAngle, const MelLib::Vector3& startScale)const;

	};
}
