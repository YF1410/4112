#pragma once

#include<memory>
#include<vector>
#include<array>
#include<map>

#include<DirectXMath.h>
#include<d3d12.h>
#include<d3dx12.h>
#include<dxgi.h>

#include"DirectXStruct.h"
#include"Texture.h"
#include"Color.h"
#include"BufferData.h"
#include"CollisionDetectionData.h"

#include"Material.h"

// ����ǂݍ��߂Ȃ��G���[���V�K�v���W�F�N�g�J�����ɏo����x64�ɂȂ��Ă邩�m�F���悤!
#include<fbxsdk.h>

#ifdef _DEBUG
#pragma comment(lib,"libfbxsdk-md.lib")
#pragma comment(lib,"libxml2-md.lib")
#pragma comment(lib,"zlib-md.lib")
#else
#pragma comment(lib,"libfbxsdk-mt.lib")
#pragma comment(lib,"libxml2-mt.lib")
#pragma comment(lib,"zlib-mt.lib")
#endif // _DEBUG


// �����G���A
// 2022 08 08
// �{�[���̍\���̂܂��̓N���X��V���ɍ�肽��
// �����蔻�������������A�ʒu��ς�����悤�ɂ�����
// ModelBone������ɂ��Ă���������

namespace MelLib
{
	struct Node
	{
		std::string nodeName;

		DirectX::XMVECTOR scaling = { 1,1,1,0 };
		DirectX::XMVECTOR rotation = { 1,1,1,0 };
		DirectX::XMVECTOR translation = { 1,1,1,0 };

		// ��3��g�݈��[�������
		DirectX::XMMATRIX transform;

		// �e�m�[�h�̍s�����Z�������
		DirectX::XMMATRIX globalTransform;

		Node* parentNode = nullptr;
	};


	//obj�Ƃ���fbx�̃f�[�^�����Ă�̃��������������Ȃ�����A
	//�\���̂ł܂Ƃ߂āAfbx�̎�����new�Ń������m�ۂ���悤�ɂ���΂��������Ȃ��Ȃ�

	//���f���̒��_�A�C���f�b�N�X�A�e�N�X�`�����܂Ƃ߂��N���X
	class ModelData
	{
	public:
	
		enum class ModelFormat
		{
			MODEL_FORMAT_NONE,
			MODEL_FORMAT_PRIMITIVE,
			MODEL_FORMAT_OBJ,
			MODEL_FORMAT_FBX,
		};


#pragma region fbx�\����
		
		
		// fbx�̃{�[�������܂Ƃ߂�����
		struct FbxBone
		{
			//�{�[����
			std::string boneName;
			//�����p���̋t�s��
			DirectX::XMMATRIX invInitialPose;

			FbxBone* parentBone = nullptr;

			//�{�[�������܂Ƃ߂�����
			FbxCluster* fbxCluster;

			FbxBone(){}
			FbxBone(const std::string& name)
			{
				this->boneName = name;
			}
		};

		//fbx�̃A�j���[�V�����ɕK�v��FbxTime���܂Ƃ߂��\����
		FbxTime freamTime;
	

#pragma endregion


	private:

#pragma region obj�\����

		//obj�̃{�[�������܂Ƃ߂�����
		struct ObjBone
		{
			std::vector<Vector3>bonePosition;
		};

#pragma endregion

#pragma region fbx�\����


		//fbx�̃f�[�^���܂Ƃ߂�����
		struct FbxData
		{
			FbxScene* fbxScene = nullptr;

			//���f���̃m�[�h
			std::unordered_map<std::string,Node>nodes;

			//���b�V�������m�[�h
			Node* meshNode = nullptr;

			// <���b�V����,�{�[���̔z��>
			std::unordered_map<std::string,std::vector<FbxBone>>bones;


			// FbxAnimStack���擾���邽�߂̔ԍ�
			std::unordered_map<std::string, int>animStackNum;
			// �A�j���[�V���������擾���邽�߂̖��O
			std::unordered_map<std::string, std::string>animationDataGetName;

		};

		//// �A�j���[�V�����Ń��f���������̂�h���ׂ̕ϐ�
		// ����1�̃��b�V���ɕ����̃A�[�}�`���A�����蓖�Ă�ꍇ�́A������uint��umap<�A�[�}�`���A��,uint>�ɂ���
		//// ���f���� ���O,�{�[���̖{��
		std::unordered_map<std::string, unsigned int>boneNum;

#pragma endregion

		//���f�����i�[����z��
		static std::unordered_map<std::string, std::unique_ptr<ModelData>>pModelDatas;
		//�v���~�e�B�u���f�����i�[����z��
		static std::unordered_map<ShapeType3D, std::unique_ptr<ModelData>>pPrimitiveModelDatas;

		//�v���~�e�B�u���f���A�}�e���A����nullptr�̃��f���f�[�^�Ɋ��蓖�Ă���}�e���A��
		static std::unique_ptr<ADSAMaterial>defaultMaterial;


		static ID3D12Device* device;

#pragma region �o�b�t�@_�q�[�v

		//���_
		std::unordered_map<std::string, VertexBufferSet> vertexBufferSet;
		//�C���f�b�N�X
		std::unordered_map <std::string, IndexBufferSet> indexBufferSet;
		ComPtr<ID3D12DescriptorHeap>textureDescHeap;
		std::unordered_map<std::string, ComPtr<ID3D12Resource>> textureBuffers;

#pragma endregion

		//��č폜�Ώۃt���O
		bool batchDeletionFlag = false;

#pragma region ���f�����
		ModelFormat modelFormat = ModelFormat::MODEL_FORMAT_NONE;

		// ���f����
		std::string modelName;

		std::string modelPath;

		// ���f���t�@�C���̃I�u�W�F�N�g���ꗗ
		std::vector<std::string>objectNames;

		//�}�e���A���ɃZ�b�g����e�N�X�`��
		std::unordered_map<std::string, std::unique_ptr<Texture>>pTexture;
		//�}�e���A��
		std::unordered_map<std::string, std::unique_ptr<ADSAMaterial>> material;

		//�e�N�X�`�����g��Ȃ��ꍇ�̐F
		Color color;

		//���f���t�@�C���ɉ����f�������邩
		UINT modelFileObjectNum = 1;
		std::unordered_map<std::string, std::vector<FbxVertex>>vertices;
		//�X���[�Y�V�F�[�f�B���O�p�@��
		std::unordered_map<std::string, std::vector<DirectX::XMFLOAT3>> smoothNormal;
		//�㉺���E�O��̍ō�(�Œ�)���W�̒l(�㉺���E�O��̏��Ɋi�[)
		std::unordered_map < std::string, std::array<float, 6>>directionMaxPos;

		std::unordered_map<std::string, std::vector<USHORT>> indices;

		// �������b�V���ɑΉ�������ꍇ�A���ꏇ���ێ��ł���map�ɂ��āA���O�ł��̍s��Ƃ��悤�ɂ���(���W��]�֐��Ŗ��O�w�肵�Ďg�p���邽��)
		// ���b�V���̃O���[�o���g�����X�t�H�[���s��̔z��)
		//std::map<std::string, DirectX::XMMATRIX>meshGlobalTransform;
		// ���O�Ŏ��Ȃ珇���Ӗ��Ȃ������������ǉ�������Ȃ�����Ӗ��Ȃ�
		std::unordered_map<std::string, DirectX::XMMATRIX>meshGlobalTransform;


		ObjBone objData;
		FbxData fbxData;


		// ���f�����O�\�t�g����ǂݎ����������i�[
#pragma region �ǂݎ����������
		// �I�u�W�F�N�g���uCollision_�`��_���薼�v�������ꍇ�A���f���𔻒���Ƃ��ēǂݎ��
		// �����̍��W��������(�J�v�Z���Ȃ�)�́A��L�́u�v���̖��O�ɉ����A�u_�ԍ��v��t����
		// �� ���uCollision_Sphere_Main�v
		// �� �J�v�Z���uCollision_Capsule_Sword_01�v�uCollision_Capsule_Sword_02�v����ŃJ�v�Z��1��
		
		// ModelObject�ňړ��Ƃ������炱�����������
		// �`����ł���悤�ɂ���
		
		std::unordered_map<std::string, SphereData>sphereDatas;
		std::unordered_map<std::string, BoxData>boxDatas;
		std::unordered_map<std::string, Segment3DData>segmentDatas;
		std::unordered_map<std::string, CapsuleData>capsuleDatas;
		std::unordered_map<std::string, PlaneData>planeDatas;
		std::unordered_map<std::string, BoardData>boardDatas;
#pragma endregion


#pragma endregion

#pragma region ���_

		/// <summary>
		/// ���_�o�b�t�@�A�r���[�̐������s���܂��B
		/// </summary>
		/// <param name="vertexSize"></param>
		/// <param name="vertices"></param>
		void CreateVertexBufferSet
		(
			const size_t vertexSize,
			const std::unordered_map < std::string, size_t>& vertexNum
		);

		void MapVertices(void** data, const std::string& name);
		void UnmapVertices(const std::string& name);
#pragma endregion

#pragma region �C���f�b�N�X


		/// <summary>
		/// �C���f�b�N�X�o�b�t�@�A�r���[�̐������s���܂��B
		/// </summary>
		/// <param name="indices"></param>
		void CreateIndexBufferSet
		(
			const std::unordered_map<std::string, std::vector<USHORT>>& indices
		);

		void MapIndices(const std::unordered_map<std::string, std::vector<USHORT>>& indices);
#pragma endregion



		private:

		static void CreatePrimitiveModel();

		// ���C�u���������̃v���~�e�B�u���f���̖@���v�Z�֐�
		static void CalcPrimitiveModelNormal(std::vector<FbxVertex>& vertices,const std::vector<USHORT>& indices);
		
		void CreateModel();

		/// <summary>
		/// ���f���̒��_�f�[�^�A�}�e���A����ǂݍ��݂܂��B
		/// </summary>
		/// <param name="path"></param>
		/// <param name="name"></param>
		bool LoadModel(const std::string& path, const std::string& name);


		/// <summary>
		/// ���_�C���f�b�N�X�o�b�t�@�e�N�X�`���o�b�t�@�̐����ƃC���f�b�N�X�ƃe�N�X�`����Map���s���܂��B
		/// </summary>
		/// <param name="vertexSize"></param>
		/// <param name="vertexNum"></param>
		/// <param name="vertices"></param>
		/// <param name="indices"></param>
		void BufferPreparationSetTexture
		(
			const size_t vertexSize,
			const std::unordered_map < std::string, size_t>& vertexNum,
			const std::unordered_map < std::string, std::vector<USHORT>>& indices
		);

		void BufferPreparationSetColor
		(
			const size_t vertexSize,
			const std::unordered_map < std::string, size_t>& vertexNum,
			const std::unordered_map < std::string, std::vector<USHORT>>& indices
		);


		static std::unordered_map < std::string, std::array<float, 6>>CalcDirectionMaxPosition
		(
			const std::unordered_map < std::string, std::vector<FbxVertex>>& vertices,
			const std::vector<std::string>& objectNames
		);

		/// <summary>
		/// ���f���t�@�C������ǂݎ�����������ǉ�����֐�
		/// </summary>
		void AddLoadCollisionData(const std::string& objectName);

	public:

		ModelData() {}
		ModelData(ModelData& data);
		ModelData& operator=(ModelData& data);

		~ModelData();

		/// <summary>
		/// ���_�������Ƀ��f���f�[�^�𐶐����܂��B
		/// </summary>
		/// <param name="vertices"></param>
		/// <param name="indices"></param>
		/// <param name=""></param>
		/// <param name="batchDeletionFlag"></param>
		/// <param name="name"></param>
		/// <returns></returns>
		static bool Create
		(
			const std::unordered_map < std::string, std::vector<FbxVertex>>&vertices, 
			const std::unordered_map < std::string, std::vector<USHORT>>& indices, 
			const bool batchDeletionFlag, 
			const std::string& name
		);
		
		static bool Create
		(
			const std::vector< std::vector<FbxVertex>>& vertices, 
			const std::vector< std::vector<USHORT>>&indices, 
			const bool batchDeletionFlag, 
			const std::string& name
		);

		void Create
		(
			const std::unordered_map < std::string, std::vector<FbxVertex>>& vertices,
			const std::unordered_map < std::string, std::vector<USHORT>>& indices
		);

		void Create
		(
			const std::vector<std::vector<FbxVertex>>& vertices,
			const std::vector<std::vector<USHORT>>& indices
		);

		/// <summary>
		/// ���f����ǂݍ��݂܂��B
		/// </summary>
		/// <param name="path">�p�X</param>
		/// <param name="batchDeletionFlag">�ꊇ�폜�Ώۂ��ǂ���</param>
		/// <param name="name">�o�^��</param>
		/// <returns></returns>
		static bool Load(const std::string& path,const bool batchDeletionFlag, const std::string& name);

		/// <summary>
		///	���f���f�[�^���擾���܂��B
		/// </summary>
		/// <param name="name">���f���̓o�^��</param>
		/// <returns></returns>
		static ModelData* Get(const std::string& name) { return pModelDatas[name].get(); }

		/// <summary>
		/// ���f���f�[�^���擾���܂��B
		/// </summary>
		/// <param name="type">�擾���������f���̌`��</param>
		/// <returns></returns>
		static ModelData* Get(const ShapeType3D type) { return pPrimitiveModelDatas[type].get(); }

		/// <summary>
		/// ���f���f�[�^���폜���܂��B
		/// </summary>
		/// <param name="name"></param>
		static void Delete(const std::string& name);
		
		/// <summary>
		/// �ꊇ�폜�̑ΏۂɂȂ��Ă��郂�f���f�[�^���폜���܂��B
		/// </summary>
		static void BatchDeletion();



#pragma region �J���җp�֐�
		static void Initialize(ID3D12Device* pDevice);

		/// <summary>
		/// �C���f�b�N�X���擾�B
		/// </summary>
		/// <returns></returns>
		std::vector<std::vector<USHORT>> GetIndices()const;

		/// <summary>
		/// ���_�o�b�t�@���擾�B
		/// </summary>
		/// <returns></returns>
		std::vector<VertexBufferSet> GetVertexBufferSet()const;

		/// <summary>
		/// �C���f�b�N�X�o�b�t�@�̎擾�B
		/// </summary>
		/// <returns></returns>
		std::vector<IndexBufferSet> GetIndexBufferSet()const;

		/// <summary>
		/// �f�B�X�N���v�^�q�[�v�̎擾�B
		/// </summary>
		/// <returns></returns>
		ID3D12DescriptorHeap* GetTextureDesctiptorHeap()const { return textureDescHeap.Get(); }

		void GetAnimationTimeData(int index, FbxTime& start, FbxTime& end);
		void GetAnimationTimeData(const std::string& name, FbxTime& start, FbxTime& end);

		DirectX::XMMATRIX GetMeshGlobalTransform(const std::string& name) { return meshGlobalTransform[name]; }
		std::vector<DirectX::XMMATRIX>GetMeshGlobalTransforms();

#pragma region fbx�֌W


		const std::vector<FbxBone>& GetFbxBones(const std::string& name) const { return fbxData.bones.at(name); }

		/// <summary>
		/// �{�[�����擾���܂��B
		/// </summary>
		/// <param name="name"></param>
		/// <returns></returns>
		void GetFbxBone(const std::string& meshName , const std::string& boneName,FbxBone& bone)const;

		Node GetNode(const std::string& name) {return fbxData.nodes[name]; }

		/// <summary>
		/// ���f����FbxAnimationTimes��Ԃ��܂��B
		/// </summary>
		/// <returns></returns>
		//const FbxAnimationTimes& GetFbxAnimationTimes()const { return fbxData.animationTimes; }


		void SetFbxAnimStack(const std::string& name);

#pragma endregion

#pragma region obj�֌W

		const std::vector<Vector3>& GetObjBonePosition()const { return objData.bonePosition; }
#pragma endregion


#pragma endregion

#pragma region �Z�b�g

#pragma endregion

#pragma region �Q�b�g

		std::string GetModelPath()const { return modelPath; }
		/// <summary>
		/// ���f���Ɋ܂܂�Ă���I�u�W�F�N�g�����擾���܂��B
		/// </summary>
		/// <returns></returns>
		std::vector<std::string>GetObjectNames()const { return objectNames; }

		/// <summary>
		/// ���f���̃t�H�[�}�b�g���擾���܂��B
		/// </summary>
		/// <returns></returns>
		ModelFormat GetModelFormat() const { return modelFormat; }

		/// <summary>
		/// �{�[�������擾���܂��B
		/// </summary>
		/// <returns></returns>
		UINT GetBoneNumber(const std::string& name = "") const;

		/// <summary>
		/// ���f���t�@�C���Ɋ܂܂�Ă���I�u�W�F�N�g(���f��)�̐����擾���܂��B
		/// </summary>
		/// <returns></returns>
		UINT GetModelFileObjectNumber()const { return modelFileObjectNum; }

		//�|�C���^�Ԃ��Ȃ��ƁA2��ڌĂяo��������metadata�������Ƒ���ł��ĂȂ��̂����������ĂȂ��Ƃ��Ɠ����ɂȂ邩��|�C���^�Ԃ��Ă�
		/// <summary>
		/// �ǂݍ��񂾃��f���̃}�e���A���̃|�C���^���擾���܂��B
		/// </summary> 
		/// <returns></returns>
		std::vector<ADSAMaterial*> GetPMaterial();

		static ADSAMaterial* GetDefaultMaterial() { return defaultMaterial.get(); }

		/// <summary>
		/// �㉺���E�O��̍ō��l���擾���܂��B
		/// </summary>
		/// <returns></returns>
		std::vector<std::array<float, 6>>GetDirectionMaxPosition() const;

		/// <summary>
		/// ���_���擾���܂��B
		/// </summary>
		/// <returns></returns>
		std::vector<std::vector<FbxVertex>>GetVertices()const;

		/// <summary>
		/// ���_���W���擾���܂��B
		/// </summary>
		/// <returns>���_���W</returns>
		std::vector<std::vector<Vector3>>GetVerticesPosition()const;

#pragma endregion

		//�t�����h�N���X�́Aprivate�̕ϐ��A�֐��ɃA�N�Z�X�ł���
		friend class FbxLoader;
	};

}