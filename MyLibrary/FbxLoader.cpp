#include "FbxLoader.h"

#include<cassert>
#include<filesystem>


using namespace MelLib;

FbxLoader::FbxLoader()
{
}


FbxLoader::~FbxLoader()
{
}

FbxLoader* FbxLoader::GetInstance()
{
	static FbxLoader instance;
	return &instance;
}

void FbxLoader::Initialize(ID3D12Device* device)
{
	assert(!fbxManager);

	this->device = device;

	fbxManager = FbxManager::Create();

	FbxIOSettings* ios = FbxIOSettings::Create(fbxManager, IOSROOT);
	fbxManager->SetIOSettings(ios);

	fbxImporter = FbxImporter::Create(fbxManager, "");

	initializeFlag = true;
}

void FbxLoader::Finalize()
{
	//Importer���J�����Ă���Manager���J������
	fbxImporter->Destroy();
	fbxManager->Destroy();

	initializeFlag = false;
}

bool FbxLoader::LoadFbxModel(const std::string& modelPath, ModelData* fbxModel)
{
	std::string modelName;
	ExtractFileName(modelPath, &modelDirectryPath, &modelName);

	if (!fbxImporter->Initialize
	(
		modelPath.c_str(),
		-1,
		fbxManager->GetIOSettings()
	)) return false;

	ModelData* model = fbxModel;

	FbxScene*& fbxScene = model->fbxData.fbxScene;
	fbxScene = FbxScene::Create(fbxManager, "fbxScene");
	fbxImporter->Import(fbxScene);


	model->modelName = modelName;

	//�m�[�h���擾
	int nodeCount = fbxScene->GetNodeCount();
	//parseNodeRecursive()�Ŕz����Q�Ƃ��Ă�̂ŁA���������m�ۂ��Ȃ����Ȃ��悤�ɂ��邽�߂�reserve���Ă�
	model->fbxData.nodes.reserve(nodeCount);


	ParseNodeRecursive(model, fbxScene->GetRootNode());

	return true;
}


void FbxLoader::ParseNodeRecursive
(
	ModelData* fbxModel,
	FbxNode* fbxNode,
	Node* parentNode
)
{
	using namespace DirectX;

	//fbxModel->fbxData.nodes.emplace(,);
	Node& node = fbxModel->fbxData.nodes[fbxNode->GetName()];

	node.nodeName = fbxNode->GetName();

	FbxDouble3 rotation = fbxNode->LclRotation.Get();
	FbxDouble3 scaling = fbxNode->LclScaling.Get();
	FbxDouble3 translation = fbxNode->LclTranslation.Get();

	node.rotation =
	{
		(float)rotation[0],
		(float)rotation[1],
		(float)rotation[2],
		0.0f
	};

	node.scaling =
	{
		(float)scaling[0],
		(float)scaling[1],
		(float)scaling[2],
		0.0f
	};


	node.translation =
	{
		(float)translation[0],
		(float)translation[1],
		(float)translation[2],
		0.0f
	};

	XMMATRIX matScaling, matRotation, matTranslation;
	matScaling = XMMatrixScalingFromVector(node.scaling);
	matRotation = XMMatrixRotationRollPitchYawFromVector(node.rotation);
	matTranslation = XMMatrixTranslationFromVector(node.translation);

	node.transform = XMMatrixIdentity();
	node.transform *= matScaling;
	node.transform *= matRotation;
	node.transform *= matTranslation;

	node.globalTransform = node.transform;

	if (parentNode)
	{
		//�e�m�[�h���
		node.parentNode = parentNode;

		//�e�̍s�����Z
		node.globalTransform *= parentNode->globalTransform;
	}

	//FbxNodeAttribute �m�[�h�̒ǉ����
	FbxNodeAttribute* fbxNodeAttribute = fbxNode->GetNodeAttribute();
	if (fbxNodeAttribute)
	{
		if (fbxNodeAttribute->GetAttributeType() ==
			FbxNodeAttribute::eMesh)
		{
			fbxModel->fbxData.meshNode = &node;
			ParseMesh(fbxModel, fbxNode, &node);
		}
	}

	for (int i = 0; i < fbxNode->GetChildCount(); i++) {
		ParseNodeRecursive(fbxModel, fbxNode->GetChild(i), &node);
	}

}



void FbxLoader::ParseMesh(ModelData* fbxModel, FbxNode* node, Node* meshNode)
{
	FbxMesh* fbxMesh = node->GetMesh();

	//fbxModel->meshGlobalTransform.push_back(meshNode->globalTransform);
	fbxModel->meshGlobalTransform.emplace(meshNode->nodeName, meshNode->globalTransform);

	std::string objectName = meshNode->nodeName;
	fbxModel->objectNames.push_back(objectName);

	// ���胂�f�����m�F
	if (objectName.find("Collision_") != std::string::npos)
	{
		fbxModel->AddLoadCollisionData(objectName);
	}

	ParseMeshVertices(fbxModel, fbxMesh, meshNode, objectName);
	ParseMeshFaces(fbxModel, fbxMesh, objectName);
	ParseMaterial(fbxModel, node, objectName);
	ParseSkin(fbxModel, fbxMesh, node, objectName);
}

void FbxLoader::ParseMeshVertices(ModelData* fbxModel, FbxMesh* fbxMesh, Node* meshNode, const std::string& name)
{
	//���_���擾
	const int vertexNum = fbxMesh->GetControlPointsCount();

	auto& vertices = fbxModel->vertices;
	vertices[name].resize(vertexNum);

	//���W�擾
	FbxVector4* pCount = fbxMesh->GetControlPoints();

	for (int i = 0; i < vertexNum; i++)
	{
		// �R�s�[
		vertices[name][i].pos.x = (float)pCount[i][0];
		vertices[name][i].pos.y = (float)pCount[i][1];
		vertices[name][i].pos.z = (float)pCount[i][2];


		// ����͂�������Ȃ���Map����Ƃ��Ɋ|���邱��
		// �������Ȃ��ƃX�L���A�j���[�V�����o�O��
		// ���������čŏ�����K������Ă�?
	/*	DirectX::XMFLOAT3 pos = vertices[name][i].pos;
		DirectX::XMMATRIX posMat = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
		posMat *= meshNode->transform;

		vertices[name][i].pos.x = posMat.r[3].m128_f32[0];
		vertices[name][i].pos.y = posMat.r[3].m128_f32[1];
		vertices[name][i].pos.z = posMat.r[3].m128_f32[2];*/
	}

}

void FbxLoader::ParseMeshFaces(ModelData* fbxModel, FbxMesh* fbxMesh, const std::string& name)
{
	auto& vertices = fbxModel->vertices;
	auto& indicesVector = fbxModel->indices;
	auto& indices = indicesVector[name];

	assert(indices.size() == 0);

	//�ʐ�
	const int polygonCount = fbxMesh->GetPolygonCount();
	const int textureUVCount = fbxMesh->GetTextureUVCount();

	//UV���̃��X�g
	FbxStringList uvNames;
	fbxMesh->GetUVSetNames(uvNames);

	//�ʂ��Ƃ̏��ǂݍ���
	for (int i = 0; i < polygonCount; i++)
	{
		//�ʂ̃C���f�b�N�X��
		const int polygonSize = fbxMesh->GetPolygonSize(i);
		assert(polygonSize <= 4);

		for (int j = 0; j < polygonSize; j++)
		{
			//vertices�̓Y����
			int index = fbxMesh->GetPolygonVertex(i, j);
			assert(index >= 0);


			//�@���ǂݍ���
			FbxVector4 normal;

			//GetPolygonVertexNormal(�ʔԍ�,�ʂ̉��ڂ̒��_��)
			if (fbxMesh->GetPolygonVertexNormal(i, j, normal))
			{
				vertices[name][index].normal.x = (float)normal[0];
				vertices[name][index].normal.y = (float)normal[1];
				vertices[name][index].normal.z = (float)normal[2];
			}


			//UV�ǂݍ���
			if (textureUVCount > 0)
			{
				FbxVector2 uvs;
				bool lUnmappedUV;

				if (fbxMesh->GetPolygonVertexUV(i, j, uvNames[0], uvs, lUnmappedUV))
				{
					vertices[name][index].uv.x = (float)uvs[0];
					vertices[name][index].uv.y = (float)uvs[1];
				}
			}

			if (j < 3)
				indices.push_back(index);
			else
			{
				int index2 = indices[indices.size() - 1];
				int index3 = index;
				int index0 = indices[indices.size() - 3];
				indices.push_back(index2);
				indices.push_back(index3);
				indices.push_back(index0);
			}
		}

	}
}

void FbxLoader::ParseMaterial(ModelData* fbxModel, FbxNode* fbxNode, const std::string& name)
{
	const int materialCount = fbxNode->GetMaterialCount();

	if (materialCount == 0)return;

	fbxModel->material[name] = std::make_unique<ADSAMaterial>();
	DrawOption dp;
	dp.SetModelDefData();
	fbxModel->material[name]->Create(dp,1);

	fbxModel->pTexture[name] = std::make_unique<Texture>();


	ADSAMaterialData mtl;

	//�e�X�g�p
	/*auto& pbrMaterialVector = fbxModel->pbrMaterials;
	pbrMaterialVector.resize(1);
	PbrMaterial& pbrModelMaterial = pbrMaterialVector[0];*/



	if (materialCount > 0)
	{
		FbxSurfaceMaterial* materials = fbxNode->GetMaterial(0);

		bool textureLoader = false;

		if (materials)
		{

			if (materials->GetClassId().Is(FbxSurfaceLambert::ClassId))
			{
				FbxSurfaceLambert* lambert = static_cast<FbxSurfaceLambert*>(materials);


				FbxPropertyT<FbxDouble3> ambient = lambert->Ambient;
				mtl.ambient.v1 = (float)ambient.Get()[0];
				mtl.ambient.v2 = (float)ambient.Get()[1];
				mtl.ambient.v3 = (float)ambient.Get()[2];

				FbxPropertyT<FbxDouble3> diffuse = lambert->Diffuse;
				mtl.diffuse.v1 = (float)diffuse.Get()[0];
				mtl.diffuse.v2 = (float)diffuse.Get()[1];
				mtl.diffuse.v3 = (float)diffuse.Get()[2];



			}
			else if (materials->GetClassId().Is(FbxSurfacePhong::ClassId))
			{
				FbxSurfacePhong* phong = static_cast<FbxSurfacePhong*>(materials);


				FbxPropertyT<FbxDouble3> ambient = phong->Ambient;
				mtl.ambient.v1 = (float)ambient.Get()[0];
				mtl.ambient.v2 = (float)ambient.Get()[1];
				mtl.ambient.v3 = (float)ambient.Get()[2];

				FbxPropertyT<FbxDouble3> diffuse = phong->Diffuse;
				mtl.diffuse.v1 = (float)diffuse.Get()[0];
				mtl.diffuse.v2 = (float)diffuse.Get()[1];
				mtl.diffuse.v3 = (float)diffuse.Get()[2];

				FbxPropertyT<FbxDouble3> specular = phong->Specular;
				mtl.specular.v1 = (float)specular.Get()[0];
				mtl.specular.v2 = (float)specular.Get()[1];
				mtl.specular.v3 = (float)specular.Get()[2];
			}
			else
			{/*
				const FbxProperty propBaseColor =
					FbxSurfaceMaterialUtils::GetProperty("baseColor", materials);
				if (propBaseColor.IsValid())
				{

					FbxDouble3 baseColor = propBaseColor.Get<FbxDouble3>();

					pbrModelMaterial.baseColor.x = (float)baseColor.Buffer()[0];
					pbrModelMaterial.baseColor.y = (float)baseColor.Buffer()[1];
					pbrModelMaterial.baseColor.z = (float)baseColor.Buffer()[2];
				}

				const FbxProperty propMetalness =
					FbxSurfaceMaterialUtils::GetProperty("metalness", materials);
				if (propMetalness.IsValid())
				{
					pbrModelMaterial.metalness = propMetalness.Get<float>();
				}


				const FbxProperty propSpecular =
					FbxSurfaceMaterialUtils::GetProperty("specular", materials);
				if (propSpecular.IsValid())
				{
					pbrModelMaterial.fSpecular = propSpecular.Get<float>();
				}

				const FbxProperty propSuecularRoughness =
					FbxSurfaceMaterialUtils::GetProperty("specularRoughness", materials);
				if (propSuecularRoughness.IsValid())
				{
					pbrModelMaterial.roughness = propSuecularRoughness.Get<float>();
				}*/

			}


			fbxModel->material[name]->SetMaterialData(mtl);

			const FbxProperty diffuseProperty =
				materials->FindProperty(FbxSurfaceMaterial::sDiffuse);

			if (diffuseProperty.IsValid())
			{
				const FbxFileTexture* texture =
					diffuseProperty.GetSrcObject<FbxFileTexture>();
				if (texture)
				{
					const char* filePath = texture->GetFileName();

					std::string path_str(filePath);
					std::string texName;
					ExtractFileName(path_str, nullptr, &texName);

					//�ǂݍ��ݏ����܂���ModelData�N���X�Ƀp�X��n��������������
					//�t�@�C�����̂݋L�q����Ă�

					//fbxModel->material[0].SetLoadTexture(modelDirectryPath + name);
					fbxModel->pTexture[name]->LoadModelTexture(modelDirectryPath + texName);
					fbxModel->material[name]->SetTexture(fbxModel->pTexture[name].get());


					textureLoader = true;
				}
			}

		}


		//if(!textureLoader)
		//{
		//	//���C�u�����Ŏ������Ă�ׂ��h��e�N�X�`������������悤�ɂ���̂ŁA
		//	//���̂��������ύX����
		//	
		//	fbxModel->pTextures.resize(1);
		//	fbxModel->pTextures[0] = std::make_unique<Texture>();
		//	fbxModel->pTextures[0]->LoadModelTexture("Resources/Texture/WhiteTex.png");
		//}
	}
}

void FbxLoader::ParseSkin(ModelData* fbxModel, FbxMesh* fbxMesh, FbxNode* node, const std::string& name)
{


	//�X�L�j���O���擾
	FbxSkin* fbxSkin =
		static_cast<FbxSkin*>(fbxMesh->GetDeformer(0, FbxDeformer::eSkin));


	//�X�L�j���O��񂪂Ȃ��ꍇreturn
	if (!fbxSkin)
	{
		for (int i = 0; i < fbxModel->vertices[name].size(); i++)
		{
			//�ŏ��̃{�[���̉e���x��100%�ɂ���(�P�ʍs��̌��ʂ𔽉f�����邽��)
			fbxModel->vertices[name][i].boneIndex[0] = 0;
			fbxModel->vertices[name][i].boneWeight[0] = 1.0f;
		}
		return;
	}

	std::vector<ModelData::FbxBone>& bones = fbxModel->fbxData.bones[name];
	

	// �N���X�^�[�J�E���g = �A�[�}�`���A(�{�[��)��
	const int CLUSTER_COUNT = fbxSkin->GetClusterCount();
	if (CLUSTER_COUNT == 0)return;
	bones.reserve(CLUSTER_COUNT);
	fbxModel->boneNum[name] = CLUSTER_COUNT;
	//�{�[���ԍ��ƃX�L���E�F�C�g
	struct WeightSet
	{
		UINT index;
		float weight;
	};
	std::vector<std::list<WeightSet>>weightLists(fbxModel->vertices[name].size());


	for (int i = 0; i < CLUSTER_COUNT; i++)
	{
		//�{�[�����(�N���X�^�[��SDK�Œ�`����Ă���{�[��?)
		FbxCluster* fbxCluster = fbxSkin->GetCluster(i);

		//�{�[���̃m�[�h���擾
		const char* boneName = fbxCluster->GetLink()->GetName();

		//�{�[���ǉ�

		bones.emplace_back(ModelData::FbxBone(boneName));
		ModelData::FbxBone& bone = bones.back();
		//����{�[����fbx�̃{�[���Ƃ̕R�t��
		bone.fbxCluster = fbxCluster;



		//�����p���s��̎擾
		FbxAMatrix fbxMat;
		fbxCluster->GetTransformLinkMatrix(fbxMat);

		//XMMATRIX�ɕϊ�
		DirectX::XMMATRIX initialPose;
		ConvertMatrixFromFbx(&initialPose, fbxMat);

		//�t�s����{�[���ɓn��
		bone.invInitialPose = DirectX::XMMatrixInverse(nullptr, initialPose);




		int controlPointIndicesCount = fbxCluster->GetControlPointIndicesCount();
		int* controlPointIndices = fbxCluster->GetControlPointIndices();
		double* controlPointWeight = fbxCluster->GetControlPointWeights();
		for (int j = 0; j < controlPointIndicesCount; j++)
		{
			const int vertIndex = controlPointIndices[j];
			const float weight = (float)controlPointWeight[j];

			weightLists[vertIndex].emplace_back(WeightSet{ (UINT)i,weight });
		}
	}


	auto& vertices = fbxModel->vertices;
	for (int i = 0; i < vertices[name].size(); i++)
	{
		auto& weightList = weightLists[i];
		weightList.sort
		(
			[](auto const& lhs, auto const& rhs)
			{
				return lhs.weight > rhs.weight;
			}
		);

		int weightArrayIndex = 0;
		for (auto& weightSet : weightList)
		{
			vertices[name][i].boneIndex[weightArrayIndex] = weightSet.index;
			vertices[name][i].boneWeight[weightArrayIndex] = weightSet.weight;


			if (++weightArrayIndex >= FbxVertex::MAX_BONE_INDICES)
			{
				float weight = 0.0f;

				for (int j = 1; j < FbxVertex::MAX_BONE_INDICES; j++) {
					weight += vertices[name][i].boneWeight[j];
				}

				vertices[name][i].boneWeight[0] = 1.0f - weight;
				break;

			}
		}

	}

	// �{�[���̐擪�m�[�h���𗊂�ɐe�{�[�����Z�b�g
	SetParentBone(fbxModel, fbxSkin->GetCluster(0)->GetLink(), nullptr);

}

void MelLib::FbxLoader::SetParentBone(ModelData* fbxModel, FbxNode* node, FbxNode* parentNode)
{
	std::string name = node->GetName();
	for (int i = 0; i < node->GetChildCount(); i++)
	{
		SetParentBone(fbxModel, node->GetChild(i), node);
	}

	if (!parentNode)return;

	// �����Ă����m�[�h�����ɐe�{�[����ݒ�

	// �{�[���̎Q�Ƃ��擾
	std::vector<ModelData::FbxBone>& bones = fbxModel->fbxData.bones[name];

	for (auto& bone : bones)
	{
		if (bone.boneName == node->GetName())
		{
			for (auto& bone2 : bones)
			{
				if (bone2.boneName == parentNode->GetName())
				{
					bone.parentBone = &bone2;
					break;
				}
			}

			break;
		}
	}


}


void FbxLoader::ConvertMatrixFromFbx(DirectX::XMMATRIX* dst, const FbxAMatrix& src)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			dst->r[i].m128_f32[j] = (float)src.Get(i, j);
	}

}

std::string FbxLoader::ExtractFileName
(
	const std::string& path,
	std::string* directry,
	std::string* fileName
)
{
	size_t pos1;

	pos1 = path.rfind('\\');
	if (pos1 != std::string::npos)
	{
		if (fileName)
			*fileName = path.substr(pos1 + 1, path.size() - pos1 - 1);

		if (directry)
			*directry = path.substr(0, pos1 + 1);
	}
	pos1 = path.rfind('/');
	if (pos1 != std::string::npos)
	{
		if (fileName)
			*fileName = path.substr(pos1 + 1, path.size() - pos1 - 1);


		if (directry)
			*directry = path.substr(0, pos1 + 1);
	}

	return path;
}
