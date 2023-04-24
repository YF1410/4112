#pragma once
#include<vector>
#include<d3d12.h>
#include<DirectXMath.h>
#include<wrl.h>
#include<string>

#include"GuiValue.h"
using namespace Microsoft::WRL;

namespace MelLib
{

	//�e�f�[�^�ɃA�N�Z�X���邽�߂̂���
#pragma region �o�b�t�@�f�[�^�L�[

//����DirectXStruct�Ɉڂ��āA���f���̒��_�o�b�t�@���Ƃ���ł���enum�Z�b�g���Ă���������
//����p�C�v���C���Z�b�g����Ƃ��ɐݒ肷��悤�ɂ���?
//���̏ꍇ�A�q�[�v�̍\���̂�������ق�������?
//����V�����w�b�_�[�t�@�C������Ă����ɏ���?(DirectX�ɒ��ڊւ��Ȃ���)
	enum VertexType
	{
		VERTEX_TYPE_NONE,//���ݒ�(�܂����_�o�b�t�@�𐶐����Ă��Ȃ�)
		VERTEX_TYPE_NORMAL,//���W�Auv�A�@���̂�
		VERTEX_TYPE_OBJ_ANIMATION,//���W�Auv�A�@���A�{�[���ԍ�
		VERTEX_TYPE_USER_VERTEX,//���p�҂̎���f�[�^

	};

	//���f���̃f�[�^���������܂��B
	struct ModelDataAndKey
	{
		std::string key;//���f���f�[�^���Ăяo�����߂̕�����
		VertexType type;//���_�\���̂̎��
	};


#pragma endregion

	//DirectX12�ŕ`�悷�邽�߂Ɏg�p����\����
#pragma region �V�F�[�_�[�ɑ�����
//���_�o�b�t�@�ő�����
//Vertex.h����Ă������Ɏ�������
#pragma region ���_�\����




	struct ObjAnimationVertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 uv;
		DirectX::XMFLOAT3 normal;
		UINT boneNumber;
	};

	struct Vertex
	{
		/*Vertex();
		Vertex(const Vertex& vertex);
		Vertex(const ObjAnimationVertex& objVertex);
		~Vertex();*/
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 uv;//�|���S���̂ǂ���ӂ�������킷���́@�|���S����̍��W
		DirectX::XMFLOAT3 normal;
	};

	struct FbxVertex
	{
		static const int MAX_BONE_INDICES = 4;
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 uv;
		DirectX::XMFLOAT3 normal;
		UINT boneIndex[MAX_BONE_INDICES] = { 63,0,0,0 };
		float boneWeight[MAX_BONE_INDICES] = { 1,0,0,0 };
	};

	struct SpriteVertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 uv;
	};


	struct PointVertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 scale;
		//DirectX::XMFLOAT2 uv;
		DirectX::XMFLOAT4 color;

	};



#pragma region PMD


	struct PMDVertex
	{
#pragma pack(2)

		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 uv;
		unsigned short bone[2];
		unsigned char boneWeight;
		unsigned char edgeFlg;

#pragma pack()
	};

	struct PMDHeader
	{
		float version;
		char modelName[20];
		char comment[256];
	};
#pragma endregion

#pragma endregion


#pragma region �萔�\����

	struct ModelConstData
	{
		GuiVector3 position;
		GuiVector3 scale;
		GuiVector3 angle;
		DirectX::XMFLOAT4 addColor = { 0,0,0,0 };
		DirectX::XMFLOAT4 subColor = { 0,0,0,0 };
		DirectX::XMFLOAT4 mulColor = { 1,1,1,1 };
		DirectX::XMFLOAT2 addUV = { 0,0 };
		
		// �ėp�p�[�Z���g
		float par = 0.0f;
		float pushPolygonNum = 0.0f;

	};

	struct SpriteConstData
	{
		DirectX::XMFLOAT3 position = { 0,0,0 };
		DirectX::XMFLOAT2 scale = { 1,1 };
		DirectX::XMFLOAT3 angle = { 0,0,0 };
		DirectX::XMFLOAT4 color = { 0,0,0,0 };//�e�N�X�`���\��Ȃ��ꍇ�̐F(�e�N�X�`���\��t�����͏��0)
		DirectX::XMFLOAT4 addColor = { 0,0,0,0 };
		DirectX::XMFLOAT4 subColor = { 0,0,0,0 };
		DirectX::XMFLOAT4 mulColor = { 1,1,1,1 };
	};

	//�\�����Ă郂�f��1��1���Ⴄ�l�̍\����
	struct BoneData
	{
		//0,0,0����A�ǂ̂��炢����������\����float3
		DirectX::XMFLOAT3 moveVector = { 0,0,0 };
		DirectX::XMFLOAT3 scale = { 1,1,1 };
		DirectX::XMFLOAT3 angle = { 0,0,0 };
	};

	//�{�[���̐e���Z�b�g����Ƃ��ɃZ�b�g����l
	struct ParentBoneData
	{
		int parentBoneNum = -1;//-1�𖢃Z�b�g�ɂ��邽�߁Aint�^
		DirectX::XMFLOAT3 angleImpact = { 1,1,1 };
		DirectX::XMFLOAT3 scaleImpact = { 1,1,1 };
		DirectX::XMFLOAT3 moveVectorImpact = { 1,1,1 };
	};

	struct ModelConstBufferData
	{
		DirectX::XMFLOAT4 lightColor[20];
		DirectX::XMFLOAT4 light[20];
		DirectX::XMFLOAT4 cameraPos;
		DirectX::XMMATRIX mat;
		DirectX::XMMATRIX normalMat;
		DirectX::XMMATRIX worldMat;
		DirectX::XMFLOAT4 mulColor;
		DirectX::XMFLOAT4 addColor;
		DirectX::XMFLOAT4 subColor;
		DirectX::XMFLOAT2 addUV;

		// �ėp�p�[�Z���g
		float par = 0.0f;
		float tex3DStart;
		float tex3DEnd;
		float ex;
	};

	struct CommonConstData
	{
		//float4�ɂ��邩�p�b�N�l�߂Ȃ��Ƒ���Ȃ�(float4�ȊO�̕ϐ��Ǝ��̕ϐ��̊Ԃɓ��荞��)����float4�ɂ��Ă�
		DirectX::XMFLOAT4 lightColor;
		DirectX::XMFLOAT4 light;
		DirectX::XMFLOAT4 cameraPos;
		DirectX::XMMATRIX lightMat;//���C�g�p�s��
	};

	struct BoneConstBufferData
	{
		DirectX::XMMATRIX boneMat[64];
	};


	struct SpriteConstBufferData
	{
		DirectX::XMMATRIX mat;
		DirectX::XMFLOAT4 color;
		DirectX::XMFLOAT4 mulColor;
		DirectX::XMFLOAT4 addColor;
		DirectX::XMFLOAT4 subColor;
	};

	struct PointConstBufferData
	{
		DirectX::XMMATRIX mat;
		DirectX::XMMATRIX billboardMat;
	};


#pragma region OBJ


	////�f�[�^�i�[�p
	//struct MaterialData
	//{
	//	DirectX::XMFLOAT3 ambient;
	//	DirectX::XMFLOAT3 diffuse;
	//	DirectX::XMFLOAT3 specular;
	//	float alpha;


	//	std::string materialName;
	//	std::string textureName;

	//	MaterialData()
	//	{
	//		ambient = { 0.3f,0.3f,0.3f };
	//		diffuse = { 0.7f,0.7f,0.7f };
	//		specular = { 0.0f,0.0f,0.0f };
	//		alpha = 1.0f;
	//	}
	//};


	////�f�[�^Map�p
	//struct MaterialConstBufferData
	//{
	//	DirectX::XMFLOAT3 ambient;
	//	float pad1;
	//	DirectX::XMFLOAT3 diffuse;
	//	float pad2;
	//	DirectX::XMFLOAT3 specular;
	//	float alpha;


	//};


	//struct PbrMaterial
	//{
	//	//�A���x�h
	//	DirectX::XMFLOAT3 baseColor = { 1,1,1 };
	//	//�����x(0�܂���1)
	//	float metalness = 0.0f;
	//	//���ʔ��ˌ�(float 0����1)
	//	float fSpecular = 0.5f;
	//	//�e��
	//	float roughness = 0.0f;
	//};

	//struct PbrMaterialConstBufferData
	//{
	//	DirectX::XMFLOAT3 baseColor;
	//	float metalness;
	//	float fSpecular;
	//	float roughness;
	//	float pad[2];
	//};

#pragma endregion



#pragma endregion

#pragma region �e�N�X�`����
	//�e�N�X�`���o�b�t�@�ő�����
	struct RGBA
	{
		unsigned char r, g, b, a;
	};
#pragma region ���͓ǂݍ���

#pragma pack(2)
	//BMP��ǂݍ��ݎ��Ɏg�p
	struct BMPData
	{
		char fileType[2];//�t�@�C���`��
		long length;//�t�@�C���T�C�Y
		short areaOne;//�\���̈�1
		short areaTwo;//�\���̈�2
		unsigned long offset;//�t�@�C���擪����摜�f�[�^�܂ł̃I�t�Z�b�g

		unsigned long size;//�T�C�Y
		unsigned long width;//��
		unsigned long height;//�c
		short plane;//�v���[����
		short bit;//�F�r�b�g��
		unsigned long compression;//���k�`��
		unsigned long imageSize;//�摜�f�[�^�T�C�Y
		long meterX;//�����𑜓x
		long meterY;//�����𑜓x
		unsigned long parret;//�i�[�p���b�g
		unsigned long important;//�d�v�F�f
		std::vector<RGBA> rgbaData;
	};
#pragma pack()


#pragma endregion

#pragma region �X�v���C�g�t�H���g
	struct SpriteFontData
	{
		int widthLineNum;//����s�̕�����
		int heightLineNum;//����s�̕�����
		int fontSizeX;
		int fontSizeY;
	};

#pragma endregion


#pragma endregion

#pragma region �o�b�t�@��

	struct WorldMatData
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 angle;
		DirectX::XMFLOAT3 scale;
	};

#pragma region �ėp�o�b�t�@�Z�b�g

	//�o�b�t�@�[�AMap���邽�߂̕ϐ��A�r���[�̃Z�b�g
#pragma region ���_


	struct VertexBufferSet
	{
		ComPtr<ID3D12Resource> vertexBuffer;
		Vertex* vertexMap;
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
		std::string materialName;
	};

	struct PMDVertexBufferSet
	{
		ComPtr<ID3D12Resource> vertexBuffer;
		PMDVertex* vertexMap;
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	};
#pragma endregion


	struct IndexBufferSet
	{
		ComPtr<ID3D12Resource> indexBuffer;
		unsigned short* indexMap;
		D3D12_INDEX_BUFFER_VIEW indexBufferView;
	};

	struct ConstBufferSet
	{
		//�������������Ȃ邩�牼�ɔz��g���ĂȂ�

		//obj���̃��f�����ƂɕύX�ł���悤�ɂ��邽�߂ɔz��
		std::vector<ComPtr<ID3D12Resource>> constBuffer;
	};

	struct TextureBufferSet
	{
		//�������������Ȃ邩�牼�ɔz��g���ĂȂ�

		//.obj���̃I�u�W�F�N�g���ƂɃe�N�X�`���Ⴄ�\�����邩��z��
		std::vector<ComPtr<ID3D12Resource>> textureBuffer;
	};


#pragma endregion

#pragma region ���[�U�[�o�b�t�@
	//
	////���ꂢ���?
	//struct UserVertexBufferSet
	//{
	//	ComPtr<ID3D12Resource> vertexBuffer;
	//	void* vertexMap;
	//	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	//};
	//
	//
	//struct UserConstBufferSet
	//{
	//	ComPtr<ID3D12Resource> constBuffer;
	//	void* constMaps;
	//};

#pragma endregion


	struct DepthBufferSet
	{
		ComPtr<ID3D12Resource> depthBuffer;
	};

	//
	//struct Object3DVertData
	//{
	//	PolyData polyData;
	//	std::vector<Vertex> vertices;
	//	std::vector<unsigned short> indices;
	//	VertexBufferSet vertSet;
	//	IndexBufferSet indexSet;
	//};
	//
	//struct Object3DHeapData 
	//{
	//	std::vector<ComPtr<ID3D12Resource>> constBuffer;
	//	ComPtr<ID3D12Resource> textureBuffer;
	//	std::vector<DirectX::XMVECTOR>position;
	//	DirectX::XMFLOAT3 angle;
	//	DirectX::XMFLOAT3 scale;
	//};
	//
	//
	//struct Object2DData
	//{
	//	VertexBufferSet vertSet;
	//	ComPtr<ID3D12Resource> constSet;
	//	ComPtr<ID3D12Resource> textureBuffer;
	//	DirectX::XMVECTOR position;
	//	DirectX::XMFLOAT3 angle;
	//	DirectX::XMFLOAT3 scale;
	//
	//};

#pragma region �|�X�g�G�t�F�N�g
	struct PostEffectConstData
	{
		DirectX::XMMATRIX worldMat;
	};

#pragma endregion


#pragma endregion


#pragma region �V�X�e���n
	struct CameraData
	{
		float fovY;
		float nearNumber;
		float farNumber;
		//�����ʒu
		DirectX::XMFLOAT3 eye;
		DirectX::XMFLOAT3 target;
		DirectX::XMFLOAT3 up;
		//���݈ʒu(�s��Ōv�Z������)
		DirectX::XMFLOAT3 nowEye;
		DirectX::XMFLOAT3 nowTarget;
		DirectX::XMFLOAT3 nowUp;
		DirectX::XMFLOAT3 eyeAngle;
		DirectX::XMFLOAT3 targetAngle;
		DirectX::XMFLOAT3 upAngle;
	};
#pragma endregion



#pragma region �p�C�v���C���ƃ��[�g�V�O�l�`��
	//struct PipelineSet
	//{
	//	ComPtr<ID3D12RootSignature> rootSignature;
	//	ComPtr<ID3D12PipelineState> pipelineState;
	//	ComPtr<ID3DBlob> errorBlob;
	//};
#pragma endregion
}
