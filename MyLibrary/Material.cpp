#include "Material.h"
#include"CreateBuffer.h"
#include<d3dx12.h>
#include"ErrorProcess.h"

ID3D12Device* MelLib::Material::device;
ComPtr<ID3D12Resource>MelLib::Material::textureNoneTextureBuffer;

void MelLib::Material::MapColorBuffer(const Color& color)
{
	DirectX::XMFLOAT4* col;
	colorBuffer->Map(0, nullptr, (void**)&col);
	*col = DirectX::XMFLOAT4((float)color.r / 255.0f, (float)color.g / 255.0f, (float)color.b / 255.0f, (float)color.a / 255.0f);
	colorBuffer->Unmap(0, nullptr);

}

void MelLib::Material::SetOrLoadTextureProcess()
{



}

void MelLib::Material::CreateInitialize(const size_t& mtlByte, const unsigned int textureNum)
{

	//�萔�o�b�t�@�쐬
	CreateBuffer::GetInstance()->CreateConstBuffer
	(
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		mtlByte,
		&materialBuffer
	);

	//�萔�o�b�t�@�쐬
	CreateBuffer::GetInstance()->CreateConstBuffer
	(
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		sizeof(DirectX::XMFLOAT4),
		&colorBuffer
	);
	MapColorBuffer(color);


	//�f�B�X�N���v�^�q�[�v�쐬
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.NumDescriptors = 1 + textureNum;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NodeMask = 0;

	auto result = device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&textureHeap));

	//�f�t�H���g�̓e�N�X�`���Z�b�g���Ȃ��̂ŁA�e�N�X�`�����Z�b�g���̃o�b�t�@�̃r���[���쐬
	CreateBuffer::GetInstance()->CreateShaderResourceView
	(
		CD3DX12_CPU_DESCRIPTOR_HANDLE
		(
			textureHeap->GetCPUDescriptorHandleForHeapStart(),
			TEXTURE_HANDLE_NUM,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		),
		textureNoneTextureBuffer.Get()
	);

	this->textureNumMax = textureNum;

}

void MelLib::Material::MapMaterialData(void** pData)
{
	materialBuffer->Map(0, nullptr, pData);
}

void MelLib::Material::UnmapMaterialData()
{
	materialBuffer->Unmap(0, nullptr);

}

void MelLib::Material::Initialize(ID3D12Device* dev)
{
	device = dev;

	CreateBuffer::GetInstance()->CreateOneColorTextureBuffer
	(
		Color(0, 0, 0, 0),
		&textureNoneTextureBuffer
	);

}

ID3D12Resource* MelLib::Material::GetPConstBuffer(const MaterialConstBufferType type) const
{
	switch (type)
	{
	case MaterialConstBufferType::COLOR:
		return colorBuffer.Get();
		break;

	case MaterialConstBufferType::MATERIAL_DATA:
		return materialBuffer.Get();
	}
}

void MelLib::Material::SetColor(const Color& color)
{
	this->color = color;
	MapColorBuffer(color);
}

bool MelLib::Material::SetTexture(Texture* pTex, const std::string& name)
{
	//pTexture = pTex; 


	//pTextures.emplace(pTex->GetTextureName(),pTex);

	bool useTexture = pTextures.find(name) != pTextures.end();
	if (!useTexture && pTextures.size() == textureNumMax)
	{
		ErrorProcess::GetInstance()->StartErroeProcess
		(L"�}�e���A���Ƀe�N�X�`�����Z�b�g�ł��܂���B�e�N�X�`�����̏���𒴂��Ă��܂��B", false);
		return false;
	}




	if (pTex)
	{
		pTextures[name] = pTex;
		CreateBuffer::GetInstance()->CreateShaderResourceView
		(
			CD3DX12_CPU_DESCRIPTOR_HANDLE
			(
				textureHeap->GetCPUDescriptorHandleForHeapStart(),
				pTextures.size() - 1,
				device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
			),
			pTex->GetTextureBuffer()
		);

		MapColorBuffer(Color(0, 0, 0, 0));
	}
	else
	{
		CreateBuffer::GetInstance()->CreateShaderResourceView
		(
			CD3DX12_CPU_DESCRIPTOR_HANDLE
			(
				textureHeap->GetCPUDescriptorHandleForHeapStart(),
				0,
				device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
			),
			textureNoneTextureBuffer.Get()
		);

		MapColorBuffer(color);
	}


	return true;
}
//
//void MelLib::Material::SetTexture(const std::vector<Texture*>& pTex)
//{
//	for (const auto& texture : pTex) 
//	{
//		SetTexture(pTex);
//	}
//}

//void MelLib::Material::SetNormalMapTexture(Texture* pTex)
//{
//	
//	pNormalMapTexture = pTex; 
//	if (!pTex)
//	{
//		
//		return;
//	}
//	CreateBuffer::GetInstance()->CreateShaderResourceView
//	(
//		CD3DX12_CPU_DESCRIPTOR_HANDLE
//		(
//			textureHeap->GetCPUDescriptorHandleForHeapStart(),
//			NORMAL_MAP_HANDLE_NUM,
//			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
//		),
//		pTex->GetTextureBuffer()
//	);
//}
////
//void MelLib::Material::SetTexture3D(Texture3D* pTex)
//{
//
//	pTexture3D = pTex;
//	if (!pTex)
//	{
//		return;
//	}
//	
//
//	CreateBuffer::GetInstance()->CreateShaderResourceView
//	(
//		CD3DX12_CPU_DESCRIPTOR_HANDLE
//		(
//			textureHeap->GetCPUDescriptorHandleForHeapStart(),
//			TEXTURE_3D_HANDLE_NUM,
//			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
//		),
//		pTex->GetTextureBuffer()
//	);
//}

MelLib::Texture* MelLib::Material::GetPTexture(const std::string& name)
{
	if (name == "" || pTextures.size() == 1)
	{
		for (auto& tex : pTextures)return tex.second;
	}
	return pTextures.at(name);

}

#pragma region ADSA

MelLib::ADSAMaterial::ADSAMaterial(ADSAMaterial& mtl)
{
	Create(mtl.drawData, mtl.textureNumMax);

	// ������texture�Z�b�g��������
}

MelLib::ADSAMaterial& MelLib::ADSAMaterial::operator=(ADSAMaterial& mtl)
{
	Create(mtl.drawData, mtl.textureNumMax);
	return *this;
}

void MelLib::ADSAMaterial::Create(const DrawOption& drawData, const unsigned int textureNum)
{
	//�p�C�v���C���쐬
	ShaderDataSet set =
	{
		{ L"../MyLibrary/FbxVertexShader.hlsl","main","vs_5_0" },
		{ L"NULL","","" },
		{ L"NULL","","" },
		{ L"LIB","main","gs_5_0" },
		{ L"../MyLibrary/FbxPixelShader.hlsl","main","ps_5_0" }
	};
	Create(drawData, textureNum, set);
}

void MelLib::ADSAMaterial::Create(const DrawOption& drawData, const unsigned int textureNum, const ShaderDataSet& shader)
{
	CreateInitialize(sizeof(ADSAMaterialData), textureNum);
	Map();


	pipelineState = std::make_unique<PipelineState>();
	pipelineState->CreatePipeline
	(
		drawData,
		shader,
		PipelineStateType::MODEL,
		nullptr,
		1
	);

	this->drawData = drawData;
}

void MelLib::ADSAMaterial::Map()
{
	ADSAMaterialData* mtl = nullptr;
	MapMaterialData((void**)&mtl);
	*mtl = materialData;
	UnmapMaterialData();

}

void MelLib::ADSAMaterial::SetMaterialData(const ADSAMaterialData& data)
{
	materialData = data;
	Map();
}

#pragma endregion


#pragma region PBR


void MelLib::PBRMaterial::Map()
{
	PBRMaterialData* mtl = nullptr;
	MapMaterialData((void**)&mtl);
	*mtl = materialData;
	UnmapMaterialData();
}

void MelLib::PBRMaterial::Create(const DrawOption& drawData, const unsigned int textureNum)
{
	CreateInitialize(sizeof(PBRMaterial), textureNum);
	Map();

	//����PBR�̃V�F�[�_�[����ăZ�b�g����悤�ɂ���
	//�p�C�v���C���쐬
	ShaderDataSet set =
	{
		{ L"../MyLibrary/FbxVertexShader.hlsl","main","vs_5_0" },
		{ L"NULL","","" },
		{ L"NULL","","" },
		{ L"../MyLibrary/FbxGeometryShader.hlsl","main","gs_5_0" },
		{ L"../MyLibrary/PBRPixelShader.hlsl","main","ps_5_0" }
	};

	pipelineState->CreatePipeline
	(
		drawData,
		set,
		PipelineStateType::MODEL,
		nullptr,
		1
	);
}

void MelLib::PBRMaterial::SetMaterialData(const PBRMaterialData& data)
{
	materialData = data;
	Map();
}

MelLib::PBRMaterial::PBRMaterial(PBRMaterial& mtl)
{
	Create(mtl.drawData, mtl.textureNumMax);
}

MelLib::PBRMaterial MelLib::PBRMaterial::operator=(PBRMaterial& mtl)
{
	return PBRMaterial(mtl);
}

#pragma endregion