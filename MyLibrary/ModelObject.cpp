#include"ModelObject.h"
#include"CreateBuffer.h"
#include"FbxLoader.h"
#include"DirectionalLight.h"
#include"Matrix.h"
#include"CollisionDetectionData.h"
#include"LibMath.h"
#include"Values.h"
#include"Collision.h"
#include"DrawManager.h"

using namespace MelLib;

std::unordered_map<std::string, int>ModelObject::createCount;
std::unordered_map<std::string, std::unique_ptr<ModelObject>>ModelObject::pModelObjects;


ID3D12Device* ModelObject::device;
std::vector<ID3D12GraphicsCommandList*>ModelObject::cmdLists;
ComPtr<ID3D12RootSignature> ModelObject::rootSignature;

ModelObject::ModelObject(ModelData* pModelData, const std::string& objectName, ConstBufferData* userConstBufferData)
{
	Create(pModelData, objectName, userConstBufferData);
}

void ModelObject::CreateConstBuffer()
{
	ConstBufferData::BufferType modelConstBufferType = modelConstBufferData.bufferType;
	ConstBufferData::BufferType userConstBufferType = userConstBufferData.bufferType;

#pragma region ���T�C�Y

	constBuffer.resize(modelFileObjectNum);


	if (modelConstBufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL)
	{
		modelConstBuffer.resize(1);
	}
	else if (modelConstBufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL_OBJECT)
	{
		modelConstBuffer.resize(modelFileObjectNum);
	}


	if (userConstBufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL)
	{
		userConstBuffer.resize(1);
	}
	else if (userConstBufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL_OBJECT)
	{
		userConstBuffer.resize(modelFileObjectNum);
	}

#pragma endregion


	auto CreateBuffer = [&]
	(
		ID3D12Resource** pBuffer,
		const size_t& dataSize
		)
	{
		CreateBuffer::GetInstance()->CreateConstBuffer
		(
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			dataSize,
			pBuffer
		);
	};


	//���f��
	if (modelConstBufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL)
	{
		CreateBuffer
		(
			&modelConstBuffer[0],
			modelConstBufferData.bufferDataSize
		);
	}

	//���[�U�[
	if (userConstBufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL)
	{
		CreateBuffer
		(
			&userConstBuffer[0],
			userConstBufferData.bufferDataSize
		);
	}

	//���f�����̃I�u�W�F�N�g�����[�v
	for (int i = 0; i < modelFileObjectNum; i++)
	{
#pragma region ����


		//���C��
		CreateBuffer
		(
			&constBuffer[i],
			sizeof(ModelConstBufferData)
		);


		//���f��
		if (modelConstBufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL_OBJECT)
		{
			CreateBuffer
			(
				&modelConstBuffer[i],
				modelConstBufferData.bufferDataSize
			);
		}

		//���[�U�[
		if (userConstBufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL_OBJECT)
		{
			CreateBuffer
			(
				&userConstBuffer[i],
				userConstBufferData.bufferDataSize
			);
		}
#pragma endregion

#pragma region ������

		ModelConstBufferData* constBufferData = nullptr;
		constBuffer[i]->Map(0, nullptr, (void**)&constBufferData);

		//���C�g
		for (int j = 0, size = DirectionalLight::LIGTH_MAX; j < size; j++)
		{
			constBufferData->light[j] = DirectX::XMFLOAT4(0, 0, 0, 0);
			constBufferData->lightColor[j] = DirectX::XMFLOAT4(0, 0, 0, 0);
		}

		constBuffer[i]->Unmap(0, nullptr);
#pragma endregion
	}


}

void ModelObject::DrawCommonProcessing(const std::string& rtName)
{
	if (!pModelData)return;
	MapConstData(RenderTarget::Get(rtName)->GetCamera());

	ModelConstBufferData* constBufferData = nullptr;
	constBuffer[0]->Map(0, nullptr, (void**)&constBufferData);
	

	SetCmdList();
}

void ModelObject::MapConstData(const Camera* camera)
{
	std::vector<DirectX::XMMATRIX>meshGlobalTransforms = pModelData->GetMeshGlobalTransforms();

	ModelConstBufferData* constBufferData = nullptr;
	for (int i = 0; i < objectNames.size(); i++)
	{
		std::string objectName = objectNames[i];
#pragma region ��{�I�ȏ��̃}�b�v

		constBuffer[i]->Map(0, nullptr, (void**)&constBufferData);
		constBufferData->addColor = modelConstDatas[objectName].addColor;
		constBufferData->subColor = modelConstDatas[objectName].subColor;
		constBufferData->mulColor = modelConstDatas[objectName].mulColor;
		constBufferData->addUV = modelConstDatas[objectName].addUV;
		constBufferData->par = modelConstDatas[objectName].par;
		constBufferData->ex = modelConstDatas[objectName].pushPolygonNum;

		std::vector<DirectionalLight*> pLights = DirectionalLight::GetAll();
		for (int i = 0, size = pLights.size(); i < size; i++)
		{
			Vector3 lightDir = pLights[i]->GetDirection();
			constBufferData->light[i] = DirectX::XMFLOAT4(lightDir.x, lightDir.y, lightDir.z, 0);
			Color lightCor = pLights[i]->GetColor();
			constBufferData->lightColor[i] = DirectX::XMFLOAT4
			(
				(float)lightCor.r / 255.0f,
				(float)lightCor.g / 255.0f,
				(float)lightCor.b / 255.0f,
				(float)lightCor.a / 255.0f
			);


		}

		Vector3 cameraPos = camera->GetCameraPosition();
		constBufferData->cameraPos = DirectX::XMFLOAT4(cameraPos.x, cameraPos.y, cameraPos.z, 0);

#pragma region �s��v�Z
		DirectX::XMMATRIX matWorld = DirectX::XMMatrixIdentity();

		// FBX���f���̏ꍇ�A���b�V���̍s����|����
		if (pModelData->GetModelFormat() == ModelData::ModelFormat::MODEL_FORMAT_FBX)
		{
			matWorld *= meshGlobalTransforms[i];
		}

		Vector3 scale = modelConstDatas[objectName].scale.GetValue();
		matWorld *= DirectX::XMMatrixScaling
		(
			scale.x,
			scale.y,
			scale.z
		);

		Vector3 angle = modelConstDatas[objectName].angle.GetValue();
		matWorld *= DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(angle.z));
		matWorld *= DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(angle.x));
		matWorld *= DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(angle.y));

		constBufferData->normalMat = matWorld;

		Vector3 position = modelConstDatas[objectName].position.GetValue();
		matWorld *= DirectX::XMMatrixTranslation
		(
			position.x,
			position.y,
			position.z
		);

		constBufferData->worldMat = matWorld;
		constBufferData->mat = camera->GetViewAndProjectionMat();

#pragma endregion


#pragma region �����_�����O����
		//�㉺���E�O��
		std::array<float, 6>dirPos = pModelData->GetDirectionMaxPosition()[i];

		//���ɑO�����
		constBufferData->tex3DStart = dirPos[4];
		constBufferData->tex3DEnd = dirPos[5];
#pragma endregion


		constBuffer[i]->Unmap(0, nullptr);
	
#pragma endregion


	}

	//constBuffer[0]->Map(0, nullptr, (void**)&constBufferData);
	//constBuffer[0]->Unmap(0, nullptr);

	for (int i = 0; i < modelConstBuffer.size(); i++)
	{
		constBuffer[0]->Map(0, nullptr, (void**)&constBufferData);
		constBuffer[0]->Unmap(0, nullptr);

		std::string objectName = objectNames[i];

#pragma region �{�[���̃}�b�v


		const int BONE_NUM = pModelData->GetBoneNumber(objectNames[i]);
		if (BONE_NUM == 0)continue;


		// ���f���̃I�u�W�F�N�g���Ƃɐ�������悤�ɂ���
		SkinConstBufferData* skinConstData = nullptr;

		modelConstBuffer[i]->Map(0, nullptr, (void**)&skinConstData);

		if (pModelData->GetModelFormat() == ModelData::ModelFormat::MODEL_FORMAT_OBJ)
		{
			std::vector<Vector3>objBonePositions = pModelData->GetObjBonePosition();

			DirectX::XMMATRIX boneMat = DirectX::XMMatrixIdentity();
			DirectX::XMFLOAT3 boneScale;
			DirectX::XMFLOAT3 boneAngle;
			DirectX::XMFLOAT3 boneMoveVector;

			//�e�{�[���̍s���Z
			int parentBoneNum = 0;
			int bone = 0;

			DirectX::XMFLOAT3 bonePos = { 0,0,0 };


			for (UINT j = 0; j < BONE_NUM; j++)
			{
				boneMat = DirectX::XMMatrixIdentity();

				//�{�[�����璸�_�̋������ړ�
				bonePos = objBonePositions[j].ToXMFLOAT3();
				boneMat *= DirectX::XMMatrixTranslation(-bonePos.x, -bonePos.y, -bonePos.z);

				boneScale = boneDatas[j].scale;
				boneMat *= DirectX::XMMatrixScaling(boneScale.x, boneScale.y, boneScale.z);

				boneAngle = boneDatas[j].angle;
				boneMat *= DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(boneAngle.z));
				boneMat *= DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(boneAngle.x));
				boneMat *= DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(boneAngle.y));

				boneMoveVector = boneDatas[j].moveVector;

				//���f�����̂̃X�P�[���̏�Z�ɂ��A�{�[���̕��s�ړ��̒l�ɃX�P�[������Z����邽�߁A
				//�����đ�����}���Ă���
				//�{�[�����s��̏��ԂŊ|���邩�烂�f���̔{�����e�������Ⴄ
				Vector3 scale = modelConstDatas[objectName].scale.GetValue();
				boneMat *= DirectX::XMMatrixTranslation
				(
					boneMoveVector.x / scale.x,
					boneMoveVector.y / scale.y,
					boneMoveVector.z / scale.z
				);

				//��]��������߂�
				boneMat *= DirectX::XMMatrixTranslation(bonePos.x, bonePos.y, bonePos.z);

#pragma region �e�̏���


				DirectX::XMFLOAT3 boneAngleImpact = { 0.0f,0.0f,0.0f };
				DirectX::XMFLOAT3 boneScaleImpact = { 1.0f,1.0f,1.0f };
				DirectX::XMFLOAT3 boneMoveVectorImpact = { 0.0f,0.0f,0.0f };
				boneAngleImpact = parentBoneDatas[j].angleImpact;
				boneScaleImpact = parentBoneDatas[j].scaleImpact;
				boneMoveVectorImpact = parentBoneDatas[j].moveVectorImpact;

				//�e�̃{�[���ԍ����
				parentBoneNum = parentBoneDatas[j].parentBoneNum;

				//������g���ĉe���x�Ƃ����Z���Ă�
				//�q�@���@�e�̏��œ����
				std::vector<int>parentNums;
				parentNums.reserve(50);

				//������͐e�@���@�q
				std::vector<DirectX::XMMATRIX>parentMat;
				parentMat.reserve(50);


				//��ɂ��ׂĂ̐e���擾&�e���x�擾
				while (1)
				{
					//-1��������(�e���Z�b�g����ĂȂ�������)������
					if (parentBoneNum == -1)break;
					parentNums.push_back(parentBoneNum);

					//�e�̃{�[���ԍ����
					parentBoneNum = parentBoneDatas[parentBoneNum].parentBoneNum;

				}
				if (parentNums.size() != 0)
				{

					DirectX::XMMATRIX mulMat = DirectX::XMMatrixIdentity();

					//�e��
					const int maxParentSize = static_cast<int>(parentNums.size());

					DirectX::XMFLOAT3 pAngle = { 0,0,0 };
					DirectX::XMFLOAT3 pScale = { 1,1,1 };
					DirectX::XMFLOAT3 pMoveVector = { 0,0,0 };
					DirectX::XMFLOAT3 pPos = { 0,0,0 };

					DirectX::XMFLOAT3 pAngleImpact = { 1,1,1 };
					DirectX::XMFLOAT3 pScaleImpact = { 1,1,1 };
					DirectX::XMFLOAT3 pMoveVectorImpact = { 1,1,1 };

					mulMat = DirectX::XMMatrixIdentity();

					//�Ō�ɂ���e�̃{�[������ɉ񂷂̂ŁA�����
					pPos.x = objBonePositions[parentNums[maxParentSize - 1]].x;
					pPos.y = objBonePositions[parentNums[maxParentSize - 1]].y;
					pPos.z = objBonePositions[parentNums[maxParentSize - 1]].z;

					int impactIndex = j;
					for (auto& pNum : parentNums)
					{
						pAngle.x += boneDatas[pNum].angle.x;
						pAngle.y += boneDatas[pNum].angle.y;
						pAngle.z += boneDatas[pNum].angle.z;

						pScale.x *= boneDatas[pNum].scale.x;
						pScale.y *= boneDatas[pNum].scale.y;
						pScale.z *= boneDatas[pNum].scale.z;

						pMoveVector.x += boneDatas[pNum].moveVector.x;
						pMoveVector.y += boneDatas[pNum].moveVector.y;
						pMoveVector.z += boneDatas[pNum].moveVector.z;



						pAngleImpact.x *= parentBoneDatas[impactIndex].angleImpact.x;
						pAngleImpact.y *= parentBoneDatas[impactIndex].angleImpact.y;
						pAngleImpact.z *= parentBoneDatas[impactIndex].angleImpact.z;
						pScaleImpact.x *= parentBoneDatas[impactIndex].scaleImpact.x;
						pScaleImpact.y *= parentBoneDatas[impactIndex].scaleImpact.y;
						pScaleImpact.z *= parentBoneDatas[impactIndex].scaleImpact.z;
						pMoveVectorImpact.x *= parentBoneDatas[impactIndex].moveVectorImpact.x;
						pMoveVectorImpact.y *= parentBoneDatas[impactIndex].moveVectorImpact.y;
						pMoveVectorImpact.z *= parentBoneDatas[impactIndex].moveVectorImpact.z;

						impactIndex = pNum;
					}

					pAngle.x *= pAngleImpact.x;
					pAngle.y *= pAngleImpact.y;
					pAngle.z *= pAngleImpact.z;

					pScale.x *= pScaleImpact.x;
					pScale.y *= pScaleImpact.y;
					pScale.z *= pScaleImpact.z;

					pMoveVector.x *= pMoveVectorImpact.x;
					pMoveVector.y *= pMoveVectorImpact.y;
					pMoveVector.z *= pMoveVectorImpact.z;


					//�{�[�����璸�_�̋������ړ�
					mulMat *= DirectX::XMMatrixTranslation(-pPos.x, -pPos.y, -pPos.z);

					mulMat *= DirectX::XMMatrixScaling(pScale.x, pScale.y, pScale.z);

					mulMat *= DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(pAngle.z));
					mulMat *= DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(pAngle.x));
					mulMat *= DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(pAngle.y));


					//�e�͂܂Ƃ߂Ċ|���Ă���̂ŁA���f���Ǝ����̊g�k�����|����΂���
					//modelConstDatas[i][0].scale �� boneScale�|����K�v���邩�v�m�F
					mulMat *= DirectX::XMMatrixTranslation
					(
						pMoveVector.x / (scale.x * boneScale.x),
						pMoveVector.y / (scale.y * boneScale.y),
						pMoveVector.z / (scale.z * boneScale.z)
					);

					//��]��������߂�
					mulMat *= DirectX::XMMatrixTranslation(pPos.x, pPos.y, pPos.z);


					boneMat *= mulMat;
				}
#pragma endregion

				skinConstData->bones[j + 1] = boneMat;

			}


		}
		else if (pModelData->GetModelFormat() == ModelData::ModelFormat::MODEL_FORMAT_FBX)
		{
			pModelData->SetFbxAnimStack(fbxAnimationData.currentAnimationName);

			for (int j = 0; j < BONE_NUM; j++)
			{
				// �����Q�Ƃɂ����ق����ǂ�
				std::vector<ModelData::FbxBone> bones = pModelData->GetFbxBones(objectNames[i]);
				
				if (BONE_NUM >= BONE_MAX)
				{
					std::string outputStr = pModelData->GetModelPath() + "�̃{�[�������{�[���ő吔" + std::to_string(BONE_MAX) + "�𒴂��Ă��܂��B\n";
					OutputDebugStringA(outputStr.c_str());

					outputStr = "���f���̃{�[���� : " + std::to_string(BONE_NUM) + "\n";
					OutputDebugStringA(outputStr.c_str());
					break;
				}

				//bones[j].fbxCluster.

				//�ϊ�
				DirectX::XMMATRIX matCurrentPose;
				FbxAMatrix fbxCurrentPose =
					bones[j].fbxCluster->GetLink()->EvaluateGlobalTransform(fbxAnimationData.currentTime);
				FbxLoader::GetInstance()->ConvertMatrixFromFbx(&matCurrentPose, fbxCurrentPose);

				//��Z
				//skinConstData->bones[j] = bones[j].invInitialPose * matCurrentPose;

				//DirectX::XMMATRIX meshGlobalTransform = pModelData->GetMeshGlobalTransform(i);
				DirectX::XMMATRIX meshGlobalTransform = meshGlobalTransforms[i];
				DirectX::XMMATRIX invMeshGlobalTransform = DirectX::XMMatrixInverse(nullptr, meshGlobalTransform);

				skinConstData->bones[j] =
					meshGlobalTransform *
					bones[j].invInitialPose *
					matCurrentPose *
					invMeshGlobalTransform;


			}
		}

		modelConstBuffer[i]->Unmap(0, nullptr);
#pragma endregion

	}
}

void ModelObject::SetCmdList()
{
	DrawManager* drawManager = DrawManager::GetInstance();
	//cmdLists[0]->SetGraphicsRootSignature(rootSignature.Get());


	////cmdLists[0]->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
	//cmdLists[0]->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

//	if (drawManager->GetPreDrawType() != DrawType::MODEL)
	{
		cmdLists[0]->SetGraphicsRootSignature(rootSignature.Get());
		cmdLists[0]->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
//	if (drawManager->GetPreModelData() != pModelData) 
	{
		//���f�����L�o�b�t�@�Z�b�g
		if (modelConstBufferData.bufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL)
			cmdLists[0]->SetGraphicsRootConstantBufferView(MODEL_BUFFER_REGISTER, modelConstBuffer[0]->GetGPUVirtualAddress());

		//���[�U�[���f���o�b�t�@�Z�b�g
		if (userConstBufferData.bufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL)
			cmdLists[0]->SetGraphicsRootConstantBufferView(MODEL_BUFFER_REGISTER, userConstBuffer[0]->GetGPUVirtualAddress());
	}
	
	////���f�����L�o�b�t�@�Z�b�g
	//if (modelConstBufferData.bufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL)
	//	cmdLists[0]->SetGraphicsRootConstantBufferView(MODEL_BUFFER_REGISTER, modelConstBuffer[0]->GetGPUVirtualAddress());

	////���[�U�[���f���o�b�t�@�Z�b�g
	//if (userConstBufferData.bufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL)
	//	cmdLists[0]->SetGraphicsRootConstantBufferView(MODEL_BUFFER_REGISTER, userConstBuffer[0]->GetGPUVirtualAddress());

	const std::vector<VertexBufferSet>& vertexBufferSets = pModelData->GetVertexBufferSet();
	const std::vector<IndexBufferSet>& indexBufferSets = pModelData->GetIndexBufferSet();


	//���f���̃I�u�W�F�N�g���Ƃ��Z�b�g
	//���_�o�b�t�@�����[�v
	for (int i = 0; i < modelFileObjectNum; i++)
	{
		/*if(modelFileObjectNum >= 2)
		{
			i = modelFileObjectNum - 1;
		}*/

		std::string objectName = objectNames[i];


	//	if(drawManager->GetPreVertexBuffer() != vertexBufferSets[i].vertexBuffer.Get())
		cmdLists[0]->IASetVertexBuffers(0, 1, &vertexBufferSets[i].vertexBufferView);

		//if (drawManager->GetPreIndexBuffer() != indexBufferSets[i].indexBuffer.Get())
		cmdLists[0]->IASetIndexBuffer(&indexBufferSets[i].indexBufferView);
		
		//cmdLists[0]->SetPipelineState(materials[objectName]->GetPPipelineState()->GetPipelineState().Get());
	//	if (drawManager->GetMaterial() != materials[objectName]) 
		{
			cmdLists[0]->SetPipelineState(materials[objectName]->GetPPipelineState()->GetPipelineState().Get());
		}
		


#pragma region �e�N�X�`��
	//	if (drawManager->GetMaterial() != materials[objectName]) 
		{
			ID3D12DescriptorHeap* textureDescHeap = materials[objectName]->GetPTextureHeap();
			std::vector<ID3D12DescriptorHeap*> ppHeaps;
			ppHeaps.push_back(textureDescHeap);
			cmdLists[0]->SetDescriptorHeaps(1, &ppHeaps[0]);

			/*for (int i = 0; i < materials[objectName]->GetTextureNum(); i++) 
			{
				D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE
				(
					materials[objectName]->GetPTextureHeap()->GetGPUDescriptorHandleForHeapStart(),
					i,
					device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
				);
				cmdLists[0]->SetGraphicsRootDescriptorTable(TEXURE_ROOTPARAM_NUM, gpuDescHandle);
			}*/
			D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE
			(
				materials[objectName]->GetPTextureHeap()->GetGPUDescriptorHandleForHeapStart(),
				0,
				device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
			);
			cmdLists[0]->SetGraphicsRootDescriptorTable(TEXURE_ROOTPARAM_NUM, gpuDescHandle);
		}
#pragma endregion

#pragma region �萔


		//if(modelFileObjectNum >= 2)//�萔�o�b�t�@
		//	cmdLists[0]->SetGraphicsRootConstantBufferView(CONST_BUFFER_REGISTER, constBuffer[1]->GetGPUVirtualAddress());
		//else
		//�萔�o�b�t�@
		cmdLists[0]->SetGraphicsRootConstantBufferView(CONST_BUFFER_REGISTER, constBuffer[i]->GetGPUVirtualAddress());

		/*ModelConstBufferData* constBufferData;
		constBuffer[0]->Map(0, nullptr, (void**)&constBufferData);*/
		//if (drawManager->GetMaterial() != materials[objectName])
		{
			//�}�e���A���o�b�t�@
			cmdLists[0]->SetGraphicsRootConstantBufferView
			(MATERIAL_BUFFER_REGISTER, materials[objectName]->GetPConstBuffer(Material::MaterialConstBufferType::MATERIAL_DATA)->GetGPUVirtualAddress());

			//Color�}�e���A��
			cmdLists[0]->SetGraphicsRootConstantBufferView
			(COLOR_MATERIAL_BUFFER_REGISTER, materials[objectName]->GetPConstBuffer(Material::MaterialConstBufferType::COLOR)->GetGPUVirtualAddress());
		}
		//���f���o�b�t�@
		if (modelConstBufferData.bufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL_OBJECT)
			cmdLists[0]->SetGraphicsRootConstantBufferView(MODEL_BUFFER_REGISTER, modelConstBuffer[i]->GetGPUVirtualAddress());

		//���[�U�[�萔�o�b�t�@
		if (userConstBufferData.bufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL_OBJECT)
			cmdLists[0]->SetGraphicsRootConstantBufferView(MODEL_BUFFER_REGISTER, userConstBuffer[i]->GetGPUVirtualAddress());


#pragma endregion


		//�`��
		cmdLists[0]->DrawIndexedInstanced(static_cast<UINT>(pModelData->GetIndices()[i].size()), 1, 0, 0, 0);

	}
	drawManager->SetDrawData(*this);
}

void MelLib::ModelObject::Update()
{
	FbxAnimation();
}

void ModelObject::Draw(const std::string& rtName)
{
	RenderTarget::ChangeCurrentRenderTarget(RenderTarget::Get(rtName));

	//FbxAnimation();

	DrawCommonProcessing(rtName);
}

bool MelLib::ModelObject::MeshCat(const PlaneData& plane, ModelData*& pFront, ModelData*& pBack, const bool createCrossSection)
{
	// ���ʂ���]������Ƃ��AZXY����Ȃ���YXZ�ł�����ق�������?



	//�ً}(�����D��x���߂̌��݂̖��_)
	//1.���������l�p�`�ł��a����ɂ���Ă�5�p�`�ɂȂ�̂ŁA�ǂ����݂��ʂ̍Č`���͕K�v
	//�����́A�a��ꂽ�ӂ̎�����(���Α�����Ȃ���)�̒��_�ƏՓ˓_�ő��p�`�̎O�p�`�����s��
	//�a��ꂽ�ӂɊւ�镔�������s���΂悢

	//��邱��
	//1.���������ӂ�������Ȃ��A�S���_�ɑ΂��ĕ\��������s���B
	//(���Α��̒��_�����ׂď����Ȃ��Ƃ����Ȃ�����)

	//2.�f�ʂɎg�p����Փ˓_�Ɠ���ւ��钸�_���ǂ����̔��f�B
	//�g�����g��Ȃ��������킩��΂����B
	//�����́A�|�[�g�t�H���I�ɏ����Ă���
	//���f�ł�����A�z��̉��Ԗڂ��Ƃ��̐��l���擾���Ƃ��āA�f�ʂ̌`�����Ɏ擾���ăC���f�b�N�X�Ƃ��Ďg����悤�ɂ��Ƃ�
	//��������ւ����ɏ�����΂��āA�ォ��ǉ����Ă�����

	//3.������΂�����A���_�����ł͂Ȃ��C���f�b�N�X���ύX����B
	//���Α��̒��_���֌W���Ă�C���f�b�N�X�̃Z�b�g(�O�p�`���`������Y����3��)��������΂��B
	//���ʂ��܂����ł镔���͏�������_���B

	//���݂̖��_
	//1.�f�ʂ��`������̂Ɏg�����_�Ƃ�������Ȃ����_�����邪�A�f�ʂ��`������̂Ɏg�����_���ǂ����o�����B
	//�@�������̒��_�̂���g���܂킻���Ǝv�������A�f�ʂ̂悤�ɖʂ��`�����ĂȂ��\��������̂Ŗ����������B
	//(�l�p�`�͒f�ʂƓ����C���f�b�N�X�̖ʂ������炠�邩��ł���)
	//���������f�ʂ̂悤�ɖʂ��`�����ĂȂ��ꍇ�́A���̒��_��ǉ����Ȃ��Ƃ����Ȃ�
	//���������ؒf���ĂȂ��Ȃ钸�_�����ւ����ɏ���?
	//���ǒf�ʂ��`������̂Ɏg�����g��Ȃ����𔻒f���Ȃ��Ƃ����Ȃ��B
	//���ǎO�p�`���a��Ƃ��Ɠ����悤�Ɍ����炠��ʂ��`�����Ȃ����Ȃ��Ƃ����Ȃ�?

	//2,�a��������3�ȏ㕪�����ꍇ�͂ǂ����邩(w�̐^�񒆎a�������Ȃ�5�ɕ������)


	//�v����������
	//1.3�p�`���a�������ɖʂ������邩��A����ւ���΂ł���?
	//���ǎl�p�`�ɕς��Ȃ��Ƃ����Ȃ����疳���B
	//���p�`���O�p�`�̏W���̂ɂ����Ŗʂ��`�����Ȃ����΂ł���?

	//���̏�����Box��Board�ōs��

	//�Ƃ肠���������蔻�菈����������΂��āA
	//�C���f�b�N�X�̃Z�b�g�ƒf�ʂ̕`����s��(�f�ʗp�̃e�N�X�`���͊��蓖�ĂȂ�)


	//�X�P�[�������ł͂Ȃ��A��]�ƕ��s�ړ����l�����Ȃ��Ƃ����Ȃ�
	//���s�ړ��͖��Ȃ��B��]�́A���ʂ���]������΂���

	//���_�ɗ��\�̏�����������?
	//�V�F�[�_�[�ŕ�������?

	//�V�F�[�_�[�ŕ������āA�X�g���[���o�͂ŁA�C���f�b�N�X���󂯎���Ώ����̖��͉����B
	//�󂯎�ꂻ���ȋC�͂��邯�ǁB�������Ȃ��ƕ������Ă������Ȃ���


	//�Փ˓_�����߂�
	//���̈ʒu�����ɁA�V�F�[�_�[�œ_�𐶐��B
	//�X�g���[���ŕ����������_���o��
	//�o�͂������_�̕\������
	//2�̃o�b�t�@�ɕ�����

	//��������O�p�`���ǂ������ʂ��Ȃ��Ƃ����Ȃ����߁A
	//���ǒ��_�ɏ���ǉ����Ȃ��Ƃ����Ȃ�?
	//����Ƃ��A�V�F�[�_�[�ŋ��߂���?

	//�X�g���[���o�͂��ƃo�b�t�@�ɏ������ނ���A���_�f�[�^�擾�Ƒ��삪�ł��Ȃ�?����Ƃ��e�N�X�`���o�b�t�@�݂����ɓǂݎ���?

	//�W�I���g���V�F�[�_�[�ł�鏈����������Ŏ�������Ίy�ɕ����ł���?


	//��ɎO�p�`���Ƃɕ����ď��������ق������₷������
	//�^�v���ɁA�C���f�b�N�X���Q�l��Value3�Œ��_�A�C���f�b�N�X�̓Y����(�C���f�b�N�X�����������ɃA�N�Z�X����p)


	//�ŏ��̒i�K�ŁA�ׂ����߂��ق�������
	//�����̓_(���X�̎O�p�`�̓_)�ƏՓ˓_�ŋ��������߁A���̏������ɁA���������߂�

	//����ł�����������A�ӂ��ƂɏՓ˓_�i�[���āA
	//�O�p�`�̍��W�ǂ������A���̍��W���g���Ă�ӂ̏Փ˓_�A������̏Փ˓_�A�c��̍��W�Ƃ����������g���΂悢

	// 2022 5 9 
	//�@��O�o���message��
	// �d��x���x��	�R�[�h	����	�v���W�F�N�g	�t�@�C��	�s	�}�����
	// ���b�Z�[�W	LNT - arithmetic - overflow	�T�u���́A���L���^�Ɋ��蓖�Ă�O�ɃI�[�o�[�t���[����\��������܂��B	MyLibrary	C : \Users\ichik\Desktop\�v���W�F�N�g\Library�����O�V�K�v���W�F�N�g\MyLibrary\ModelObject.cpp	753
	// �̖���������Ύ��邩��

	if (catFrontModelData || catBackModelData)return false;

	//�O�p�`�����ɒ��_�����邩�Ȃ����̊m�F�������Ƃ��Ă�

	//�@��0��������ؒf�ł��Ȃ����߁Afalse
	if (plane.GetNormal() == 0.0f)return false;
	
	// ���ʏ��(��]�K���̂��߁A��蒼��)
	PlaneData rotPlane;
	rotPlane.SetPosition(plane.GetPosition());

	Vector3 angle = -modelConstDatas[objectNames[0]].angle.GetValue();
	DirectX::XMFLOAT3 xmAngle = DirectX::XMFLOAT3(angle.x, angle.y, angle.z);
	Vector3 normal = LibMath::RotateZXYVector3(plane.GetNormal(), xmAngle);
	rotPlane.SetNormal(normal);

	//���f���̒��_���O�p�`���Ƃɂ܂Ƃ߂�����
	struct ModelTri
	{
		//���_
		Value3<FbxVertex> vertData;

		//�\�����茋��
		Value3<char>vertFB;

		//�O�p�`�̕ӂ̏��
		Value3<Segment3DData>segmentData;

		//���ʂƂ̏Փ˔���
		Value3<bool>hitResult;

		//�Փ˓_�̒��_���
		Value3<FbxVertex>hitPosVert;

		//�C���f�b�N�X�̔z��̃C���f�b�N�X
		Value3<int> indicesIndex = 0;
	};


	std::vector<std::vector<FbxVertex>>vertPos = pModelData->GetVertices();
	std::vector<std::vector<USHORT>>indices = pModelData->GetIndices();

	std::vector<ModelTri>modelTri(indices[0].size() / 3);

	//�O�p�`���ƂɃf�[�^�i�[
	for (int i = 0, size = indices[0].size(); i < size; i += 3)
	{
		int triIndex = 0;
		if (i == 0)triIndex = 0;
		else triIndex = i / 3;

		//���_���
		modelTri[triIndex].vertData.v1 = vertPos[0][indices[0][i]];
		modelTri[triIndex].vertData.v2 = vertPos[0][indices[0][i + 1]];
		modelTri[triIndex].vertData.v3 = vertPos[0][indices[0][i + 2]];

		Vector3 scale = modelConstDatas[0].scale.GetValue();
		//�g�k�A���s�ړ��K��
		//���̂܂܂��ƃ��f���f�[�^�̒��_���{������������ԂɂȂ�̂ŁA����
		modelTri[triIndex].vertData.v1.pos.x *= scale.x;
		modelTri[triIndex].vertData.v1.pos.y *= scale.y;
		modelTri[triIndex].vertData.v1.pos.z *= scale.z;
		modelTri[triIndex].vertData.v2.pos.x *= scale.x;
		modelTri[triIndex].vertData.v2.pos.y *= scale.y;
		modelTri[triIndex].vertData.v2.pos.z *= scale.z;
		modelTri[triIndex].vertData.v3.pos.x *= scale.x;
		modelTri[triIndex].vertData.v3.pos.y *= scale.y;
		modelTri[triIndex].vertData.v3.pos.z *= scale.z;

		Vector3 position = modelConstDatas[0].position.GetValue();
		modelTri[triIndex].vertData.v1.pos.x += position.x;
		modelTri[triIndex].vertData.v1.pos.y += position.y;
		modelTri[triIndex].vertData.v1.pos.z += position.z;
		modelTri[triIndex].vertData.v2.pos.x += position.x;
		modelTri[triIndex].vertData.v2.pos.y += position.y;
		modelTri[triIndex].vertData.v2.pos.z += position.z;
		modelTri[triIndex].vertData.v3.pos.x += position.x;
		modelTri[triIndex].vertData.v3.pos.y += position.y;
		modelTri[triIndex].vertData.v3.pos.z += position.z;

		//�ӏ��
		modelTri[triIndex].segmentData.v1.SetPosition
		(Value2<Vector3>(modelTri[triIndex].vertData.v1.pos, modelTri[triIndex].vertData.v2.pos));
		modelTri[triIndex].segmentData.v2.SetPosition
		(Value2<Vector3>(modelTri[triIndex].vertData.v2.pos, modelTri[triIndex].vertData.v3.pos));
		modelTri[triIndex].segmentData.v3.SetPosition
		(Value2<Vector3>(modelTri[triIndex].vertData.v3.pos, modelTri[triIndex].vertData.v1.pos));

		//�C���f�b�N�X�̔z��̃C���f�b�N�X
		modelTri[triIndex].indicesIndex.v1 = i;
		modelTri[triIndex].indicesIndex.v2 = i + 1;
		modelTri[triIndex].indicesIndex.v3 = i + 2;

		//���\���茋��
		modelTri[triIndex].vertFB.v1 = LibMath::PointPlaneFrontBackCheck(modelTri[triIndex].vertData.v1.pos, rotPlane);
		modelTri[triIndex].vertFB.v2 = LibMath::PointPlaneFrontBackCheck(modelTri[triIndex].vertData.v2.pos, rotPlane);
		modelTri[triIndex].vertFB.v3 = LibMath::PointPlaneFrontBackCheck(modelTri[triIndex].vertData.v3.pos, rotPlane);

		//�ӂ̏Փˊm�F�A�Փ˓_�擾
		Segment3DCalcResult res;
		modelTri[triIndex].hitResult.v1
			= Collision::PlaneAndSegment3D(rotPlane, modelTri[triIndex].segmentData.v1, &res);
		modelTri[triIndex].hitPosVert.v1.pos = res.planeHitPos.ToXMFLOAT3();
		modelTri[triIndex].hitPosVert.v1.normal = modelTri[triIndex].vertData.v1.normal;
		//modelTri[triIndex].hitPosVert.v1.uv = modelTri[triIndex].vertData.v1.normal;

		modelTri[triIndex].hitResult.v2
			= Collision::PlaneAndSegment3D(rotPlane, modelTri[triIndex].segmentData.v2, &res);
		modelTri[triIndex].hitPosVert.v2.pos = res.planeHitPos.ToXMFLOAT3();
		modelTri[triIndex].hitPosVert.v2.normal = modelTri[triIndex].vertData.v2.normal;
		//modelTri[triIndex].hitPosVert.v2.uv = modelTri[triIndex].vertData.v2.normal;

		modelTri[triIndex].hitResult.v3
			= Collision::PlaneAndSegment3D(rotPlane, modelTri[triIndex].segmentData.v3, &res);
		modelTri[triIndex].hitPosVert.v3.pos = res.planeHitPos.ToXMFLOAT3();
		modelTri[triIndex].hitPosVert.v3.normal = modelTri[triIndex].vertData.v3.normal;
		//modelTri[triIndex].hitPosVert.v3.uv = modelTri[triIndex].vertData.v3.normal;
	}

	//�S���_���܂Ƃ߂�z��B�\�Ɨ��ŕ�����
	std::vector<FbxVertex>frontVertices;
	std::vector<FbxVertex>backVertices;
	std::vector<USHORT>frontIndices;
	std::vector<USHORT>backIndices;



	//�O�p�`�����Ă����āA�i�[���Ă���
	USHORT frontIndex = 0;
	USHORT backIndex = 0;

	for (const auto& tri : modelTri)
	{
		//�q�b�g���Ă�̂ɕ\���ɕ�����ĂȂ��Ƃ������Ƃ͒��_�ɂ������Ă邾���Ȃ̂ŁA�������Ȃ�
		if (tri.hitResult.v1 || tri.hitResult.v2 || tri.hitResult.v3)
		{

			if (tri.vertFB.v1 != -1 && tri.vertFB.v2 != -1 && tri.vertFB.v3 != -1
				|| tri.vertFB.v1 != 1 && tri.vertFB.v2 != 1 && tri.vertFB.v3 != 1)
			{
				if (tri.vertFB.v1 == 1 || tri.vertFB.v2 == 1 || tri.vertFB.v3 == 1)
				{
					frontVertices.push_back(tri.vertData.v1);
					frontVertices.push_back(tri.vertData.v2);
					frontVertices.push_back(tri.vertData.v3);

					frontIndices.push_back(frontIndex);
					frontIndices.push_back(frontIndex + 1);
					frontIndices.push_back(frontIndex + 2);

					frontIndex += 3;
				}
				else if (tri.vertFB.v1 == -1 || tri.vertFB.v2 == -1 || tri.vertFB.v3 == -1)
				{
					backVertices.push_back(tri.vertData.v1);
					backVertices.push_back(tri.vertData.v2);
					backVertices.push_back(tri.vertData.v3);

					backIndices.push_back(backIndex);
					backIndices.push_back(backIndex + 1);
					backIndices.push_back(backIndex + 2);

					backIndex += 3;
				}
				continue;
			}
		}


		//�ꎞ�I�Ɋi�[���邽�߂̔z��
		std::vector<FbxVertex>fVert;
		std::vector<FbxVertex>bVert;


		//�Е��ɕ΂��Ă���(�a���ĂȂ�������)�C���f�b�N�X��ʏ�ʂ�i�[���Ď���
		if (!tri.hitResult.v1 && !tri.hitResult.v2 && !tri.hitResult.v3)
		{
			if (tri.vertFB.v1 == 1)
			{
				frontVertices.push_back(tri.vertData.v1);
				frontVertices.push_back(tri.vertData.v2);
				frontVertices.push_back(tri.vertData.v3);

				frontIndices.push_back(frontIndex);
				frontIndices.push_back(frontIndex + 1);
				frontIndices.push_back(frontIndex + 2);

				frontIndex += 3;
			}
			else
			{
				backVertices.push_back(tri.vertData.v1);
				backVertices.push_back(tri.vertData.v2);
				backVertices.push_back(tri.vertData.v3);

				backIndices.push_back(backIndex);
				backIndices.push_back(backIndex + 1);
				backIndices.push_back(backIndex + 2);

				backIndex += 3;
			}
			continue;
		}

		//�΂��ĂȂ�������A�\�����ƂɊi�[

		//�������߂�


		//���_�̃C���f�b�N�X(frontInd�AbackInd�Ɋi�[����l)
		std::vector<USHORT>fVertInd;
		std::vector<USHORT>bVertInd;


		//�O�p�`�̓_���ǂ���ɕ΂��Ă邩�𒲂ׂ�
		if (tri.vertFB.v1 == 1)
		{
			fVert.push_back(tri.vertData.v1);
		}
		else if (tri.vertFB.v1 == -1)
		{
			bVert.push_back(tri.vertData.v1);
		}
		if (tri.vertFB.v2 == 1)
		{
			fVert.push_back(tri.vertData.v2);
		}
		else if (tri.vertFB.v2 == -1)
		{
			bVert.push_back(tri.vertData.v2);
		}
		if (tri.vertFB.v3 == 1)
		{
			fVert.push_back(tri.vertData.v3);
		}
		else if (tri.vertFB.v3 == -1)
		{
			bVert.push_back(tri.vertData.v3);

		}


		bool addVert = true;
		//�Е��̏ꍇ(���p�`�̖ʌ`������Ȃ��ꍇ)
		if (fVert.size() == 1)
		{
			frontVertices.push_back(fVert[0]);

			if (tri.hitResult.v1)
			{
				if (Vector3(fVert[0].pos) != Vector3(tri.hitPosVert.v1.pos))
				{
					frontVertices.push_back(tri.hitPosVert.v1);
					fVert.push_back(tri.hitPosVert.v1);
				}
			}
			if (tri.hitResult.v2)
			{
				for (int i = 0; i < fVert.size(); i++)
				{
					if (Vector3(fVert[i].pos) == Vector3(tri.hitPosVert.v2.pos))
					{
						addVert = false;
						break;
					}
				}

				if (addVert)
				{
					frontVertices.push_back(tri.hitPosVert.v2);
					fVert.push_back(tri.hitPosVert.v2);
				}
			}

			addVert = true;
			if (tri.hitResult.v3)
			{
				for (int i = 0; i < fVert.size(); i++)
				{
					if (Vector3(fVert[i].pos) == Vector3(tri.hitPosVert.v3.pos))
					{
						addVert = false;
						break;
					}
				}
				if (addVert)
				{
					frontVertices.push_back(tri.hitPosVert.v3);
					fVert.push_back(tri.hitPosVert.v3);
				}
			}

			Vector3 cross;
			cross = LibMath::CalcNormal(fVert[0].pos, fVert[1].pos, fVert[2].pos);
			if (Vector3(tri.vertData.v1.normal) == cross)
			{
				frontIndices.push_back(frontIndex);
				frontIndices.push_back(frontIndex + 1);
				frontIndices.push_back(frontIndex + 2);
			}
			else//�@�����t��������A���т��t�ɂ��Ċi�[
			{
				frontIndices.push_back(frontIndex + 2);
				frontIndices.push_back(frontIndex + 1);
				frontIndices.push_back(frontIndex);
			}

			frontIndex += 3;
		}
		else
		{
			frontVertices.push_back(fVert[0]);

			int fVertVNum = 0;
			//fVert[0]���܂܂�ĂĂ����������ʂɓ������Ă���Փ˓_���i�[
			if (tri.hitResult.v1)
			{
				if (tri.segmentData.v1.GetPosition().v1 == fVert[0].pos
					|| tri.segmentData.v1.GetPosition().v2 == fVert[0].pos)
				{
					//fVert0Seg = 
					fVertVNum = 1;
					frontVertices.push_back(tri.hitPosVert.v1);
				}
			}
			if (tri.hitResult.v2)
			{
				if (tri.segmentData.v2.GetPosition().v1 == fVert[0].pos
					|| tri.segmentData.v2.GetPosition().v2 == fVert[0].pos)
				{
					fVertVNum = 2;
					frontVertices.push_back(tri.hitPosVert.v2);
				}
			}
			if (tri.hitResult.v3)
			{
				if (tri.segmentData.v3.GetPosition().v1 == fVert[0].pos
					|| tri.segmentData.v3.GetPosition().v2 == fVert[0].pos)
				{
					fVertVNum = 3;
					frontVertices.push_back(tri.hitPosVert.v3);
				}
			}

			//������̏Փ˓_�Ǝc��̒��_���i�[
			if (tri.hitResult.v1 && fVertVNum != 1)
			{
				frontVertices.push_back(tri.hitPosVert.v1);
			}
			else if (tri.hitResult.v2 && fVertVNum != 2)
			{
				frontVertices.push_back(tri.hitPosVert.v2);
			}
			else if (tri.hitResult.v3 && fVertVNum != 3)
			{
				frontVertices.push_back(tri.hitPosVert.v3);
			}

			frontVertices.push_back(fVert[1]);



			//�O�p�`���`��

			//fVert���N���A���āA����ǉ������O�p�`��ǉ�
			fVert.clear();
			fVert.resize(4);
			for (int i = frontIndex; i < frontIndex + 4; i++)
			{
				fVert[i - frontIndex] = frontVertices[i];
			}

			//n���p�`�̎O�p�`�����𗘗p���ăC���f�b�N�X�����߂�
			//���_�����ԉ������_
			FbxVertex farVertex;

			//��ԉ������_�̍��W
			float farVertDis = 0.0f;

			//farVertex�̓Y����(�`����̍폜�p)
			int farVertIndex = 0;

			std::vector<int>skipVertIndices;

			int vertCount = fVert.size();
			//�\
			while (1)
			{
				farVertDis = 0.0f;
				//�O�p�`�����ɒ��_�����邩�Ȃ����̊m�F�������Ƃ��Ă�

				//��ԉ������_�����߂�
				for (int i = 0, size = fVert.size(); i < size; i++)
				{
					bool skip = false;
					for (const auto& ind : skipVertIndices)
					{
						if (i == ind)skip = true;
					}
					if (skip)continue;


					float dis = Vector3(fVert[i].pos).Length();
					if (farVertDis <= dis)
					{
						farVertex = fVert[i];
						farVertDis = dis;
						farVertIndex = i;
					}
				}

				//10/11 �������������Ȃ��Ƃ����Ȃ�(��������Ȃ��Ĕz��̃C���f�b�N�X���狁�߂�)
				//�ׂ̒��_�����߂�
				int farAddIndex = farVertIndex + 1;
				/*	if (farAddIndex >= fVert.size())farAddIndex = 0;
					for (const auto& ind : skipVertIndices)
					{
						if (ind == farAddIndex)
						{
							farAddIndex++;
							break;
						}
					}*/

				if (farAddIndex >= fVert.size())farAddIndex = 0;

				bool whileEnd = false;
				while (!whileEnd && skipVertIndices.size() != 0)
				{
					for (const auto& ind : skipVertIndices)
					{
						if (ind == farAddIndex || farAddIndex == farVertIndex)
						{
							farAddIndex++;
							if (farAddIndex >= fVert.size())farAddIndex = 0;
						}
						else
						{
							whileEnd = true;
						}
					}
				}





				int farSubIndex = farVertIndex - 1;
				/*	if (farSubIndex <= 0)farSubIndex = fVert.size() - 1;
					for (const auto& ind : skipVertIndices)
					{
						if (ind == farSubIndex)
						{
							farSubIndex--;
							break;
						}
					}*/
				if (farSubIndex < 0)farSubIndex = 3;
				whileEnd = false;
				while (!whileEnd && skipVertIndices.size() != 0)
				{
					for (const auto& ind : skipVertIndices)
					{
						if (ind == farSubIndex || farSubIndex == farVertIndex || farSubIndex == farAddIndex)
						{
							farSubIndex--;
							if (farSubIndex < 0)farSubIndex = 3;
						}
						else
						{
							whileEnd = true;
						}
					}
				}

				//�O�p�`���`��

				//���߂��O�ςƁA�ʂ̊O�ς���v������v�Z�I��
				//�O�p�`�̊O��
				Vector3 cross;

				//sub,far,add
				cross = LibMath::CalcNormal(fVert[farSubIndex].pos, farVertex.pos, fVert[farAddIndex].pos);
				if (Vector3(farVertex.normal) == cross)
				{
					frontIndices.push_back(frontIndex + farSubIndex);
					frontIndices.push_back(frontIndex + farVertIndex);
					frontIndices.push_back(frontIndex + farAddIndex);
				}
				else//�@�����t��������A�t�ɂ��Ċi�[ 2,1,far
				{
					frontIndices.push_back(frontIndex + farAddIndex);
					frontIndices.push_back(frontIndex + farVertIndex);
					frontIndices.push_back(frontIndex + farSubIndex);
				}

				//���_�����ԉ����_���폜
				//fVert.erase(fVert.begin() + farVertIndex);
				vertCount--;
				skipVertIndices.push_back(farVertIndex);

				//�`���s�ɂȂ�����I��
				//if (fVert.size() == 2)break;
				if (vertCount == 2)break;

			}
			frontIndex += 4;

		}


		if (bVert.size() == 1)
		{
			backVertices.push_back(bVert[0]);

			bool addVert = true;
			if (tri.hitResult.v1)
			{
				if (Vector3(bVert[0].pos) != Vector3(tri.hitPosVert.v2.pos))
				{
					backVertices.push_back(tri.hitPosVert.v1);
					bVert.push_back(tri.hitPosVert.v1);
				}
			}
			if (tri.hitResult.v2)
			{
				for (int i = 0; i < bVert.size(); i++)
				{
					//���ꂾ�ƁA�Փ˓_�����̎O�p�`�Ŋ܂܂ꂽ��A��΂ɓ���Ȃ��Ȃ��Ē��_����Ȃ��ăG���[�o��
					//backVertices����Ȃ���bVert�ł�����?
					if (Vector3(bVert[i].pos) == Vector3(tri.hitPosVert.v2.pos))
					{
						addVert = false;
						break;
					}
				}
				if (addVert)
				{
					backVertices.push_back(tri.hitPosVert.v2);
					bVert.push_back(tri.hitPosVert.v2);
				}
			}

			addVert = true;
			if (tri.hitResult.v3)
			{
				for (int i = 0; i < bVert.size(); i++)
				{
					if (Vector3(bVert[i].pos) == Vector3(tri.hitPosVert.v3.pos))
					{
						addVert = false;
						break;
					}
				}
				if (addVert)
				{
					backVertices.push_back(tri.hitPosVert.v3);
					bVert.push_back(tri.hitPosVert.v3);
				}
			}

			Vector3 cross;
			cross = LibMath::CalcNormal(bVert[0].pos, bVert[1].pos, bVert[2].pos);
			if (Vector3(tri.vertData.v1.normal) == cross)
			{
				backIndices.push_back(backIndex);
				backIndices.push_back(backIndex + 1);
				backIndices.push_back(backIndex + 2);
			}
			else//�@�����t��������A���т��t�ɂ��Ċi�[
			{
				backIndices.push_back(backIndex + 2);
				backIndices.push_back(backIndex + 1);
				backIndices.push_back(backIndex);
			}

			backIndex += 3;
		}
		else
		{
			backVertices.push_back(bVert[0]);

			int bVertVNum = 0;
			//bVert[0]���܂܂�ĂĂ����������ʂɓ������Ă���Փ˓_���i�[
			if (tri.hitResult.v1)
			{
				if (tri.segmentData.v1.GetPosition().v1 == bVert[0].pos
					|| tri.segmentData.v1.GetPosition().v2 == bVert[0].pos)
				{
					//bVert0Seg = 
					bVertVNum = 1;
					backVertices.push_back(tri.hitPosVert.v1);
				}
			}
			if (tri.hitResult.v2)
			{
				if (tri.segmentData.v2.GetPosition().v1 == bVert[0].pos
					|| tri.segmentData.v2.GetPosition().v2 == bVert[0].pos)
				{
					bVertVNum = 2;
					backVertices.push_back(tri.hitPosVert.v2);
				}
			}
			if (tri.hitResult.v3)
			{
				if (tri.segmentData.v3.GetPosition().v1 == bVert[0].pos
					|| tri.segmentData.v3.GetPosition().v2 == bVert[0].pos)
				{
					bVertVNum = 3;
					backVertices.push_back(tri.hitPosVert.v3);
				}
			}

			//������̏Փ˓_�Ǝc��̒��_���i�[
			if (tri.hitResult.v1 && bVertVNum != 1)
			{
				backVertices.push_back(tri.hitPosVert.v1);
			}
			else if (tri.hitResult.v2 && bVertVNum != 2)
			{
				backVertices.push_back(tri.hitPosVert.v2);
			}
			else if (tri.hitResult.v3 && bVertVNum != 3)
			{
				backVertices.push_back(tri.hitPosVert.v3);
			}

			backVertices.push_back(bVert[1]);



			//�O�p�`���`��

			//bVert���N���A���āA����ǉ������O�p�`��ǉ�
			bVert.clear();
			bVert.resize(4);
			for (int i = backIndex; i < backIndex + 4; i++)
			{
				bVert[i - backIndex] = backVertices[i];
			}

			//n���p�`�̎O�p�`�����𗘗p���ăC���f�b�N�X�����߂�
			//���_�����ԉ������_
			FbxVertex farVertex;

			//��ԉ������_�̍��W
			float farVertDis = 0.0f;

			//farVertex�̓Y����(�`����̍폜�p)
			int farVertIndex = 0;

			std::vector<int>skipVertIndices;
			int vertCount = bVert.size();
			//��
			while (1)
			{
				farVertDis = 0.0f;
				//�O�p�`�����ɒ��_�����邩�Ȃ����̊m�F�������Ƃ��Ă�

				//��ԉ������_�����߂�
				for (int i = 0, size = bVert.size(); i < size; i++)
				{
					bool skip = false;
					for (const auto& ind : skipVertIndices)
					{
						if (i == ind)skip = true;
					}
					if (skip)continue;

					float dis = Vector3(bVert[i].pos).Length();
					if (farVertDis <= dis)
					{
						farVertex = bVert[i];
						farVertDis = dis;
						farVertIndex = i;

					}
				}

				//10/11 �������������Ȃ��Ƃ����Ȃ�(��������Ȃ��Ĕz��̃C���f�b�N�X���狁�߂�)
				//�ׂ̒��_�����߂�
				int farAddIndex = farVertIndex + 1;
				if (farAddIndex >= bVert.size())farAddIndex = 0;

				/*for (const auto& ind : skipVertIndices)
				{
					if (ind == farAddIndex)
					{
						farAddIndex++;

						if (farAddIndex == farVertIndex)farAddIndex++;
					}
				}*/

				bool whileEnd = false;
				while (!whileEnd && skipVertIndices.size() != 0)
				{
					for (const auto& ind : skipVertIndices)
					{
						if (ind == farAddIndex || farAddIndex == farVertIndex)
						{
							farAddIndex++;
							if (farAddIndex >= bVert.size())farAddIndex = 0;
						}
						else
						{
							whileEnd = true;
						}
					}
				}



				int farSubIndex = farVertIndex - 1;
				if (farSubIndex < 0)farSubIndex = 3;
				/*for (const auto& ind : skipVertIndices)
				{
					if (ind == farSubIndex)
					{
						farSubIndex--;

						if (farSubIndex == farVertIndex || farSubIndex == farAddIndex)farSubIndex--;
					}
				}*/
				whileEnd = false;
				while (!whileEnd && skipVertIndices.size() != 0)
				{
					for (const auto& ind : skipVertIndices)
					{
						if (ind == farSubIndex || farSubIndex == farVertIndex || farSubIndex == farAddIndex)
						{
							farSubIndex--;
							if (farSubIndex < 0)farSubIndex = 3;
						}
						else
						{
							whileEnd = true;
						}
					}
				}


				//�O�p�`���`��

				//���߂��O�ςƁA�ʂ̊O�ς���v������v�Z�I��
				//�O�p�`�̊O��
				Vector3 cross;

				//sub,far,add
				cross = LibMath::CalcNormal(bVert[farSubIndex].pos, farVertex.pos, bVert[farAddIndex].pos);
				if (Vector3(farVertex.normal) == cross)
				{
					backIndices.push_back(backIndex + farSubIndex);
					backIndices.push_back(backIndex + farVertIndex);
					backIndices.push_back(backIndex + farAddIndex);
				}
				else//�@�����t��������A�t�ɂ��Ċi�[ 2,1,far
				{
					backIndices.push_back(backIndex + farAddIndex);
					backIndices.push_back(backIndex + farVertIndex);
					backIndices.push_back(backIndex + farSubIndex);
				}

				//���_�����ԉ����_���폜
				//bVert.erase(bVert.begin() + farVertIndex);
				vertCount--;
				skipVertIndices.push_back(farVertIndex);


				//�`���s�ɂȂ�����I��
				//if (bVert.size() == 2)break;
				if (vertCount == 2)break;

			}
			backIndex += 4;

		}

	}

	//���S�ɂǂ������ɕ΂��Ă�����؂�ĂȂ��̂ŁAfalse
	if (frontVertices.size() == 0 || backVertices.size() == 0)return false;

	//�����ŁA�f�ʂ̏����Z�b�g���Ă���

	if (!pFront && !catFrontModelData)
	{
		//���_�ƃC���f�b�N�X�����Ƀo�b�t�@���쐬&�X�V(Map)
		//�o�b�t�@�쐬��胂�f���f�[�^���쐬���銴���ɂ���?
		std::vector<std::vector<FbxVertex>> vert(1, frontVertices);
		std::vector<std::vector<USHORT>> ind(1, frontIndices);
		catFrontModelData = std::make_unique<ModelData>();
		catFrontModelData->Create(vert, ind);
		pFront = catFrontModelData.get();
	}
	if (!pBack && !catBackModelData)
	{
		std::vector<std::vector<FbxVertex>> vert(1, backVertices);
		std::vector<std::vector<USHORT>> ind(1, backIndices);
		catBackModelData = std::make_unique<ModelData>();
		catBackModelData->Create(vert, ind);
		pBack = catBackModelData.get();
	}

	return true;



}

void ModelObject::SetPosition(const Vector3& position, const std::string& name)
{
	if (name == "")
	{
		for (auto& data : modelConstDatas)
		{
			data.second.position = position;
		}
	}
	else
	{
		modelConstDatas[name].position = position;
	}
}

void ModelObject::SetScale(const Vector3& scale, const std::string& name)
{
	Vector3 modelScale;
	if (name == "")
	{
		for (auto& data : modelConstDatas)
		{
			data.second.scale = scale;

			modelScale = data.second.scale.GetValue();
			if (scale.x == 0.0f)data.second.scale.GetRefValue().x = 0.0001f;
			if (scale.y == 0.0f)data.second.scale.GetRefValue().y = 0.0001f;
			if (scale.z == 0.0f)data.second.scale.GetRefValue().z = 0.0001f;
		}
	}
	else
	{
		modelConstDatas[name].scale = scale.ToXMFLOAT3();
		modelScale = modelConstDatas[name].scale.GetValue();

		if (modelScale.x == 0.0f)modelConstDatas[name].scale.GetRefValue().x = 0.0001f;
		if (modelScale.y == 0.0f)modelConstDatas[name].scale.GetRefValue().y = 0.0001f;
		if (modelScale.z == 0.0f)modelConstDatas[name].scale.GetRefValue().z = 0.0001f;
	}


}

void ModelObject::SetAngle(const Vector3& angle, const std::string& name)
{
	if (name == "")
	{
		for (auto& data : modelConstDatas)
		{
			data.second.angle = angle;
		}
	}
	else
	{
		modelConstDatas[name].angle = angle;
	}


}

void MelLib::ModelObject::SetAddUV(const MelLib::Vector2& uv, const std::string& name)
{
	if (name == "")
	{
		for (auto& data : modelConstDatas)
		{
			data.second.addUV = uv.ToXMFLOAT2();
		}
	}
	else
	{
		modelConstDatas[name].addUV = uv.ToXMFLOAT2();
	}
}


void MelLib::ModelObject::SetAddColor(const Color& color, const std::string& name)
{

	Value4<float> colorValue4(color.ToFloat());
	DirectX::XMFLOAT4 colorXMFLOAT4(colorValue4.v1, colorValue4.v2, colorValue4.v3, colorValue4.v4);

	if (name == "")
	{
		for (auto& data : modelConstDatas)
		{
			data.second.addColor = colorXMFLOAT4;
		}
	}
	else
	{
		modelConstDatas[name].addColor = colorXMFLOAT4;
	}
}

void MelLib::ModelObject::SetSubColor(const Color& color, const std::string& name)
{
	Value4<float> colorValue4(color.ToFloat());
	DirectX::XMFLOAT4 colorXMFLOAT4(colorValue4.v1, colorValue4.v2, colorValue4.v3, colorValue4.v4);

	if (name == "")
	{
		for (auto& data : modelConstDatas)
		{
			data.second.subColor = colorXMFLOAT4;
		}
	}
	else
	{
		modelConstDatas[name].subColor = colorXMFLOAT4;
	}
}

void MelLib::ModelObject::SetMulColor(const Color& color, const std::string& name)
{
	Value4<float> colorValue4(color.ToFloat());
	DirectX::XMFLOAT4 colorXMFLOAT4(colorValue4.v1, colorValue4.v2, colorValue4.v3, colorValue4.v4);

	if (name == "")
	{
		for (auto& data : modelConstDatas)
		{
			data.second.mulColor = colorXMFLOAT4;
		}
	}
	else
	{
		modelConstDatas[name].mulColor = colorXMFLOAT4;
	}
}


MelLib::ModelObject::ModelObject(ModelObject& obj, const std::string& name)
{
	CopyModelObject(obj, name,CopyModelObjectContent::ALL);
}
MelLib::ModelObject::ModelObject(ModelObject& obj)
{
	CopyModelObject(obj, obj.objectName, CopyModelObjectContent::NUMBER_FLAG);
}
//
//ModelObject& MelLib::ModelObject::operator=(ModelObject& obj)
//{
//	Create(obj.pModelData, obj.objectName, nullptr);
//	modelConstDatas = obj.modelConstDatas;
//	materials = obj.materials;
//
//	if (obj.catFrontModelData)
//	{
//		catFrontModelData = std::make_unique<ModelData>();
//		*catFrontModelData = *obj.catFrontModelData;
//	}
//	if (obj.catBackModelData)
//	{
//		catBackModelData = std::make_unique<ModelData>();
//		*catBackModelData = *obj.catBackModelData;
//	}
//	return *this;
//}

bool MelLib::ModelObject::CopyModelObject(const ModelObject& obj, const std::string& name, CopyModelObjectContent copyContent)
{
	if (name == obj.objectName)return false;

	if (copyContent == CopyModelObjectContent::ALL)
	{
		Create(obj.pModelData, name, nullptr);
		materials = obj.materials;
		if (obj.catFrontModelData)
		{
			catFrontModelData = std::make_unique<ModelData>();
			*catFrontModelData = *obj.catFrontModelData;
		}
		if (obj.catBackModelData)
		{
			catBackModelData = std::make_unique<ModelData>();
			*catBackModelData = *obj.catBackModelData;
		}
	}

	modelConstDatas = obj.modelConstDatas;

	return true;
}

bool ModelObject::Initialize(ID3D12Device* dev, const std::vector<ID3D12GraphicsCommandList*>& cmdList)
{
	device = dev;
	cmdLists = cmdList;


#pragma region �f�B�X�N���v�^�����W_���[�g�p�����[�^�[
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, Material::GetTextureNumMax(), 0);
	/*CD3DX12_DESCRIPTOR_RANGE descRangeSRV2;
	descRangeSRV2.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, Material::GetTextureNumMax(), 1);*/

	CD3DX12_ROOT_PARAMETER rootparam[6] = {};

	//�s���F�Ȃ�
	rootparam[CONST_BUFFER_REGISTER].InitAsConstantBufferView(CONST_BUFFER_REGISTER, 0, D3D12_SHADER_VISIBILITY_ALL);
	//���[�U�[�萔
	rootparam[USER_BUFFER_REGISTER].InitAsConstantBufferView(USER_BUFFER_REGISTER, 0, D3D12_SHADER_VISIBILITY_ALL);
	//�}�e���A��
	rootparam[MATERIAL_BUFFER_REGISTER].InitAsConstantBufferView(MATERIAL_BUFFER_REGISTER, 0, D3D12_SHADER_VISIBILITY_PIXEL);
	//PBR�}�e���A��
	rootparam[COLOR_MATERIAL_BUFFER_REGISTER].InitAsConstantBufferView(COLOR_MATERIAL_BUFFER_REGISTER, 0, D3D12_SHADER_VISIBILITY_PIXEL);
	//���f������
	rootparam[MODEL_BUFFER_REGISTER].InitAsConstantBufferView(MODEL_BUFFER_REGISTER, 0, D3D12_SHADER_VISIBILITY_ALL);

	//�e�N�X�`��
	rootparam[TEXURE_ROOTPARAM_NUM].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_PIXEL);
	//rootparam[6].InitAsDescriptorTable(1, &descRangeSRV2, D3D12_SHADER_VISIBILITY_PIXEL);
#pragma endregion

#pragma region ���[�g�V�O�l�`��


	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	rootSignatureDesc.pParameters = rootparam;
	rootSignatureDesc.NumParameters = _countof(rootparam);


	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	samplerDesc.MinLOD = 0.0f;
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	ComPtr<ID3DBlob>rootSigBlob;
	ComPtr<ID3DBlob>errorBlob;

	auto result = D3D12SerializeRootSignature(
		&rootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);


	if (FAILED(result))
	{
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";

		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	result = device->CreateRootSignature(
		0,
		rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));

#pragma endregion

	PipelineState::SetModelRootSignature(rootSignature.Get());

}

void MelLib::ModelObject::SetPar(const float par, const std::string& name)
{
	if (name == "")
	{
		for (auto& data : modelConstDatas)
		{
			data.second.par = par / 100.0f;
		}

	}
	else
	{
		modelConstDatas[name].par = par / 100.0f;
	}

}

void MelLib::ModelObject::SetMaterial(Material* mtl, const std::string& name)
{
	if (!mtl)
	{
#ifdef _DEBUG
		OutputDebugString(L"�}�e���A���̃Z�b�g�Ɏ��s���܂����B�}�e���A����nullptr�ł��B\n");
#endif // _DEBUG
		return;
	}
	if (name == "")
	{
		for (auto& material : materials)
		{
			material.second = mtl;
		}

	}
	else
	{
		materials[name] = mtl;
	}



}

Material* MelLib::ModelObject::GetPMaterial(const std::string& name)
{
	if (name == "")
	{
		return materials[objectNames[0]];
	}
	return materials[name];
}

Vector3 MelLib::ModelObject::GetPosition(const std::string& name) const
{
	if (name == "")
	{
		return modelConstDatas.at(objectNames[0]).position.GetValue();
	}
	return modelConstDatas.at(name).position.GetValue();
}
Vector3 MelLib::ModelObject::GetAngle(const std::string& name) const
{
	if (name == "")
	{
		return modelConstDatas.at(objectNames[0]).angle.GetValue();
	}
	return modelConstDatas.at(name).angle.GetValue();
}

Vector3 MelLib::ModelObject::GetScale(const std::string& name) const
{
	if (name == "")
	{
		return modelConstDatas.at(objectNames[0]).scale.GetValue();
	}
	return modelConstDatas.at(name).scale.GetValue();
}

MelLib::Vector2 MelLib::ModelObject::GetAddUV(const std::string& name) const
{
	if (name == "")
	{
		return modelConstDatas.at(objectNames[0]).addUV;
	}
	return modelConstDatas.at(name).addUV;
}



bool ModelObject::Create(ModelData* pModelData, const std::string& objectName, ConstBufferData* userConstBufferData, const std::string& name)
{


	if (!pModelData)
	{
#ifdef _DEBUG

		OutputDebugStringA(name.data());
		OutputDebugStringW(L"�̐����Ɏ��s���܂����B\n");
#endif // _DEBUG
		return false;
	}

	pModelObjects.emplace(name, std::make_unique<ModelObject>(pModelData, objectName, userConstBufferData));


	return true;
}

void ModelObject::Delete(const std::string& name)
{
	pModelObjects.erase(name);
}


void ModelObject::SetCurrentFream(const UINT fream)
{
	FbxTime setTime = fbxAnimationData.animationTimes.startTime * fream;

	if (setTime > fbxAnimationData.animationTimes.endTime) {
		setTime = fbxAnimationData.animationTimes.endTime;
	}

	fbxAnimationData.currentTime = setTime;
}



void ModelObject::FbxAnimation()
{
	// ������Draw�ŌĂ΂Ȃ��悤�ɂ���

	if (!isAnimation || pModelData->GetModelFormat() != ModelData::ModelFormat::MODEL_FORMAT_FBX)return;


	//�^�C����i�߂�
	if(animationReverse)fbxAnimationData.currentTime += fbxAnimationData.animationTimes.frameTime * -fbxAnimationData.timeMag;
	else fbxAnimationData.currentTime += fbxAnimationData.animationTimes.frameTime * fbxAnimationData.timeMag;

	animationEnd = false;


	/*if (fbxAnimationData.currentTime > fbxAnimationData.animationTimes.endTime) 
	{

		if (animationEndStop) 
		{
			fbxAnimationData.currentTime = fbxAnimationData.animationTimes.endTime;
			return;
		}

		fbxAnimationData.currentTime = fbxAnimationData.animationTimes.startTime;

	}
	else
		if (fbxAnimationData.currentTime < fbxAnimationData.animationTimes.startTime) {

			if (animationEndStop) {
				fbxAnimationData.currentTime = fbxAnimationData.animationTimes.startTime;
				return;
			}

			fbxAnimationData.currentTime = fbxAnimationData.animationTimes.endTime;

		}*/


	if (fbxAnimationData.currentTime >= fbxAnimationData.animationTimes.endTime
		&& !animationReverse
		|| fbxAnimationData.currentTime <= fbxAnimationData.animationTimes.startTime
		&& animationReverse)
	{
		animationEnd = true;
	}

	if(animationReverse)
	{
		if (fbxAnimationData.currentTime < fbxAnimationData.animationTimes.startTime)
		{

			if (animationEndStop)
			{
				fbxAnimationData.currentTime = fbxAnimationData.animationTimes.startTime;

			}
			else 
			{
				fbxAnimationData.currentTime = fbxAnimationData.animationTimes.endTime;
			}
		}
		else if (fbxAnimationData.currentTime > fbxAnimationData.animationTimes.endTime)
		{
			// �r���ōĐ��t�ɂ��Ă��߂����߂̏���
			fbxAnimationData.currentTime = fbxAnimationData.animationTimes.endTime;
		}
	}
	else
	{
		if (fbxAnimationData.currentTime > fbxAnimationData.animationTimes.endTime)
		{

			if (animationEndStop)
			{
				fbxAnimationData.currentTime = fbxAnimationData.animationTimes.endTime;
				
			}
			else
			{
				fbxAnimationData.currentTime = fbxAnimationData.animationTimes.startTime;
			}
		}
		else if (fbxAnimationData.currentTime < fbxAnimationData.animationTimes.startTime)
		{
			// �r���ōĐ��t�ɂ��Ă��߂����߂̏���
			fbxAnimationData.currentTime = fbxAnimationData.animationTimes.startTime;
		}
	}


}


bool ModelObject::Create(ModelData* pModelData, const std::string& objectName, ConstBufferData* userConstBufferData)
{
#pragma region ���O

	this->objectName = objectName;

	if (createCount[objectName] >= 1) 
	{
		this->objectName += "_" + std::to_string(createCount[objectName]);
	}

	createCount[objectName]++;


#pragma endregion

	/*guiPosition.SetData(0, objectName,"ModelPosition",-10000,10000);
	guiAngle.SetData(0, objectName,"ModelAngle",-10000,10000);
	guiScale.SetData(1, objectName,"ModelScale",-10000,10000);*/
	

	if (!pModelData)
	{
#ifdef _DEBUG
		OutputDebugStringW(L"pModelData��nullptr�ł��B�����Ɏ��s���܂���\n");
#endif // _DEBUG
		return false;
	}

	// �I�u�W�F�N�g���擾
	objectNames = pModelData->GetObjectNames();

#pragma region �f�[�^�Z�b�g
	if (userConstBufferData)this->userConstBufferData = *userConstBufferData;

	//obj�̃{�[����MoveVector�̊��鏈���̂��߂ɁA�I�u�W�F�N�g���ƂɃo�b�t�@�쐬
	if (pModelData->GetModelFormat() == ModelData::ModelFormat::MODEL_FORMAT_OBJ)
	{
		modelConstBufferData.bufferType = ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL_OBJECT;
	}
	else
	{
		modelConstBufferData.bufferType = ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL_OBJECT;
	}
	modelConstBufferData.bufferDataSize = sizeof(SkinConstBufferData);

	modelFileObjectNum = pModelData->GetModelFileObjectNumber();
	this->pModelData = pModelData;

#pragma endregion

	CreateConstBuffer();

#pragma region GUI


	for (const auto& objName : objectNames)
	{
		modelConstDatas.try_emplace(objName);

		modelConstDatas[objName].position.SetData(0, this->objectName, objName +"_Position", -1000, 1000);
		modelConstDatas[objName].angle.SetData(0, this->objectName, objName +"_Angle", -359, 359);
		modelConstDatas[objName].scale.SetData(1, this->objectName, objName +"_Scale", -10, 10);
	}
#pragma endregion

#pragma region �}�e���A��


	//�}�e���A���擾
	std::vector<ADSAMaterial*>modelDataMtl = pModelData->GetPMaterial();

	size_t size = modelDataMtl.size();

	for (int i = 0; i < size; i++)
	{
		materials[objectNames[i]] = modelDataMtl[i];
	}

#pragma endregion

#pragma region �A�j���[�V�����֌W


	boneDatas.resize(pModelData->GetBoneNumber());
	parentBoneDatas.resize(pModelData->GetBoneNumber());
	SkinConstBufferData* skinConstData = nullptr;

	for (auto& buffer : modelConstBuffer)
	{
		buffer->Map(0, nullptr, (void**)&skinConstData);
		for (int i = 0; i < BONE_MAX; i++)
		{
			skinConstData->bones[i] = DirectX::XMMatrixIdentity();
		}
		buffer->Unmap(0, nullptr);
	}

	if (pModelData->GetBoneNumber() != 0 && pModelData->GetModelFormat() == MelLib::ModelData::ModelFormat::MODEL_FORMAT_FBX) {
		fbxAnimationData.animationTimes.frameTime.SetTime(0, 0, 0, 1, 0, FbxTime::EMode::eFrames60);

		// 0�Ԗڂ̃A�j���[�V�����̎��Ԃ��Z�b�g
		pModelData->GetAnimationTimeData(0, fbxAnimationData.animationTimes.startTime, fbxAnimationData.animationTimes.endTime);
	}
#pragma endregion



	return true;
}



std::vector<std::vector<Vector3>> MelLib::ModelObject::GetVerticesData(const bool scaleImpact, const bool angleImpact, const bool transformImpact)
{
	std::vector<std::vector<Vector3>>verticesPos = pModelData->GetVerticesPosition();

	for (int i = 0, size = objectNames.size(); i < size; i++)
	{
		std::string objectName = objectNames[i];
		for (auto& pos : verticesPos[i])
		{
			Matrix mat = Matrix::GetTranslationMatrix(pos);
			if (scaleImpact) mat *= Matrix::GetScalingMatrix(modelConstDatas[objectName].scale.GetValue());
			if (angleImpact)mat *= Matrix::GetRotateZXYMatrix(modelConstDatas[objectName].angle.GetValue());
			if (transformImpact)mat *= Matrix::GetTranslationMatrix(modelConstDatas[objectName].position.GetValue());
			pos = Vector3(mat[3][0], mat[3][1], mat[3][2]);
		}
	}
	return verticesPos;

}

void MelLib::ModelObject::GetModelTriangleData(std::vector<std::vector<TriangleData>>& vec) const
{

	std::vector<std::vector<Vector3>>vertPos = pModelData->GetVerticesPosition();
	std::vector<std::vector<USHORT>>vertIndex = pModelData->GetIndices();

	vec.resize(vertIndex.size());


	for (int i = 0; i < objectNames.size(); i++)
	{
		std::string objectName = objectNames[i];
		//vec[i].resize(vertPos[i].size() / 3);
		vec[i].resize(vertIndex[i].size() / 3);

		for (int j = 0, loopCount = 0; j < vertIndex[i].size(); j += 3, loopCount++)
		{
			Value3<Vector3>pos(vertPos[i][vertIndex[i][j]], vertPos[i][vertIndex[i][j + 1]], vertPos[i][vertIndex[i][j + 2]]);

			vec[i][loopCount].SetPosition(pos);
			vec[i][loopCount].SetScale(Vector3(modelConstDatas.at(objectName).scale.GetValue()));
			vec[i][loopCount].SetAngle(modelConstDatas.at(objectName).angle.GetValue());
			vec[i][loopCount].SetTranslationPosition(modelConstDatas.at(objectName).position.GetValue());
		}
	}

}

Vector3 MelLib::ModelObject::CalcAnimationPosition
(
	const Vector3& pos, const float weigth, const std::string& boneName, const std::string& meshName
	, const MelLib::Vector3& startPos , const MelLib::Vector3& startAngle, const MelLib::Vector3& startScale
)const
{

	// ��U���b�V���̈ړ��ʕ��������Č��_��ŉ񂳂Ȃ�����o�O��?
	// �����b�V���̈ړ��Ƃ��K�����ĂȂ�����֌W�Ȃ�
	// ���f���̊g�k�Ƃ������{�̏�ԂŌv�Z���Ȃ���������������

	// ���f���̈ړ��ʂ�g�k��0�̂Ƃ��Ƀ��[���h�s��|����̂��x�X�g?
	// ���ꂩ�A��̍��W�����Ă��炤�����Ȃ�

	// �e�̃m�[�h�̍s����|���Ȃ��ƃ_��
	//�����e���x100%�ő��v?

	pModelData->SetFbxAnimStack(fbxAnimationData.currentAnimationName);

	DirectX::XMMATRIX posMat = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);

	// �t�s����|���邱�Ƃɂ��A�g�k�Ȃǂ������f���ɂ��̂܂܍��W�����킹�ăZ�b�g���Ă�����ɓ�������
	DirectX::XMMATRIX startWorldMat = DirectX::XMMatrixIdentity();
	startWorldMat *= DirectX::XMMatrixScaling
	(
		startScale.x,
		startScale.y,
		startScale.z
	);
	startWorldMat *= DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(startAngle.z));
	startWorldMat *= DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(startAngle.x));
	startWorldMat *= DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(startAngle.y));

	startWorldMat *= DirectX::XMMatrixTranslation
	(
		startPos.x,
		startPos.y,
		startPos.z
	);
	posMat *= DirectX::XMMatrixInverse(nullptr, startWorldMat);
	/*Vector3 nodeT = pModelData->GetNode(meshName).translation;
	posMat *= DirectX::XMMatrixTranslation(nodeT.x, nodeT.y, nodeT.z);*/



	// ��Z
	DirectX::XMMATRIX meshGlobalTransform = pModelData->GetMeshGlobalTransform(meshName);
	DirectX::XMMATRIX invMeshGlobalTransform = DirectX::XMMatrixInverse(nullptr, meshGlobalTransform);


	ModelData::FbxBone bone;
	pModelData->GetFbxBone(meshName, boneName, bone);

	// �擾�ƕϊ�
	DirectX::XMMATRIX matCurrentPose;

	FbxAMatrix fbxCurrentPose =
		bone.fbxCluster->GetLink()->EvaluateGlobalTransform(fbxAnimationData.currentTime);
	FbxLoader::GetInstance()->ConvertMatrixFromFbx(&matCurrentPose, fbxCurrentPose);


	DirectX::XMMATRIX boneMat = DirectX::XMMatrixIdentity();
	boneMat *=
		//meshGlobalTransform *
		bone.invInitialPose *
		matCurrentPose //*
		//invMeshGlobalTransform
		;

	boneMat *= weigth; 
	posMat *= boneMat;

	// �e�{�[���̍s�����Z
	/*while (1)
	{
		if (bone.parentBone)
		{
			bone = pModelData->GetFbxBone(bone.parentBone->boneName);
		}
		else
		{
			break;
		}

		fbxCurrentPose =
			bone.fbxCluster->GetLink()->EvaluateGlobalTransform(fbxAnimationData.currentTime);
		FbxLoader::GetInstance()->ConvertMatrixFromFbx(&matCurrentPose, fbxCurrentPose);

		boneMat = DirectX::XMMatrixIdentity();
		boneMat *=
			bone.invInitialPose *
			matCurrentPose;

		posMat *= boneMat;
	}*/

	posMat *= startWorldMat;


	// ��l������ꍇ�A�Ō�ɂ��̂܂܂̒l�g�������[���h�s��|�����Ⴂ���Ȃ�
	// ��l�Ƃ̍��������Ȃ��Ƃ����Ȃ�
	DirectX::XMMATRIX worldMat = DirectX::XMMatrixIdentity();
	Vector3 scale = modelConstDatas.at(meshName).scale.GetValue();
	worldMat *= DirectX::XMMatrixScaling
	(
		scale.x / startScale.x,
		scale.y / startScale.y,
		scale.z / startScale.z
	);

	Vector3 angle = modelConstDatas.at(meshName).angle.GetValue();
	worldMat *= DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(angle.z - startAngle.z));
	worldMat *= DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(angle.x - startAngle.x));
	worldMat *= DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(angle.y - startAngle.y));

	Vector3 position = modelConstDatas.at(meshName).position.GetValue();
	worldMat *= DirectX::XMMatrixTranslation
	(
		position.x - startPos.x,
		position.y - startPos.y,
		position.z - startPos.z
	);
	posMat *= worldMat;

	return MelLib::Vector3(posMat.r[3].m128_f32[0], posMat.r[3].m128_f32[1], posMat.r[3].m128_f32[2]);

}

void MelLib::ModelObject::SetAnimationReversePlayBack(const bool flag)
{
	animationReverse = flag;

	if (animationReverse) 
	{
		if (fbxAnimationData.currentTime <= fbxAnimationData.animationTimes.startTime)animationEnd = true;
		else animationEnd = false;
	}
	else 
	{
		if (fbxAnimationData.currentTime <= fbxAnimationData.animationTimes.endTime)animationEnd = true;
		else animationEnd = false;
	}
}

void MelLib::ModelObject::SetAnimation(const std::string& name)
{
	if (fbxAnimationData.currentAnimationName == name)return;
	//fbxAnimationData.currentTime = fbxAnimationData.animationTimes.startTime;
	fbxAnimationData.currentAnimationName = name;
	pModelData->GetAnimationTimeData(name, fbxAnimationData.animationTimes.startTime, fbxAnimationData.animationTimes.endTime);


	//pModelData->SetFbxAnimStack(name);

	// �I���t���O�Z�b�g
	if (fbxAnimationData.animationTimes.endTime == fbxAnimationData.currentTime)animationEnd = true;
	else animationEnd = false;
}

