#include "Sprite2DBase.h"

using namespace MelLib;
DirectX::XMMATRIX Sprite2DBase::cameraMatrix;
PipelineState Sprite2DBase::defaultPipeline;


//Sprite2DBase::Sprite2DBase(const Color& color)
//{
//	Create(color);
//}
//
//Sprite2DBase::Sprite2DBase(Texture* pTexture)
//{
//	Create(pTexture);
//}


bool Sprite2DBase::Initialize(const int winWidth, const int winHeight)
{
	cameraMatrix = DirectX::XMMatrixOrthographicOffCenterLH
	(
		0.0f,
		winWidth,
		winHeight,
		0.0f,
		0.0f,
		1.0f
	);

	DrawOption data;
	data.alphaWrite = true;
	data.blendMode = BlendMode::ADD;
	data.cullMode = CullMode::NONE;
	data.depthTest = false;
	data.drawMode = DrawMode::SOLID;
	
	ShaderDataSet set =
	{
		{ L"../MyLibrary/SpriteVertexShader.hlsl","VSmain","vs_5_0" },
		{ L"NULL","","" },
		{ L"NULL","","" },
		{ L"NULL","","" },
		{ L"../MyLibrary/SpritePixelShader.hlsl","PSmain","ps_5_0" }
	};
	
	auto result = defaultPipeline.CreatePipeline
	(
		data,
		set,
		PipelineStateType::SPRITE,
		nullptr,
		1
	);
	if (!result)
	{
		OutputDebugString(L"Sprite2DBase�̏������Ɏ��s���܂����B�f�t�H���g�p�C�v���C���𐶐��ł��܂���ł���\n");
		return false;
	}
	return true;
}

//
//void Sprite2DBase::Create(const Color& color)
//{
//
//	CreateBuffer();
//	InitializeVertices();
//	SetOneColorSpriteColor(color);
//	
//	pipeline = defaultPipeline.GetPipelineState();
//}
//
//void Sprite2DBase::Create(Texture* pTexture)
//{
//	this->pTexture = pTexture;
//	//�e�N�X�`������������`��͈͕ύX
//	if (pTexture) drawRightDownPosition = pTexture->GetTextureSize();
//	CreateBuffer();
//	InitializeVertices();
//	
//	pipeline = defaultPipeline.GetPipelineState();
//}

void Sprite2DBase::SpriteInitialize()
{
	CreateBuffer();
	InitializeVertices();

}
//
//void Sprite2DBase::Draw(const std::string& rtName)
//{
//	SpriteVertex* vertex;
//	MapVertexBuffer((void**)&vertex);
//
//#pragma region ���_���W
//	Vector2 spriteSize = drawRightDownPosition - drawLeftUpPosition;
//
//	vertices[0].pos = { -spriteSize.x / 2 , spriteSize.y / 2, 0 };
//	vertices[1].pos = { -spriteSize.x / 2 ,-spriteSize.y / 2,0 };
//	vertices[2].pos = { spriteSize.x / 2  ,   spriteSize.y / 2 ,0 };
//	vertices[3].pos = { spriteSize.x / 2  , -spriteSize.y / 2,0 };
//
//#pragma endregion
//
//#pragma region UV���W
//
//	Vector2 textureSize = 1;
//	if (pTexture)  textureSize = pTexture->GetTextureSize();
//	Vector2 uvLeftUp = { 1.0f / textureSize.x * drawLeftUpPosition.x ,1.0f / textureSize.y * drawLeftUpPosition.y };
//	Vector2 uvRightDown = { 1.0f / textureSize.x * drawRightDownPosition.x ,1.0f / textureSize.y * drawRightDownPosition.y };
//
//	vertices[0].uv = { uvLeftUp.x ,uvRightDown.y };
//	vertices[1].uv = { uvLeftUp.x,uvLeftUp.y };
//	vertices[2].uv = { uvRightDown.x ,uvRightDown.y };
//	vertices[3].uv = { uvRightDown.x ,uvLeftUp.y };
//#pragma endregion
//
//
//	auto vertexNum = vertices.size();
//	for (int i = 0; i < vertexNum; i++)
//		vertex[i] = vertices[i];
//
//	UnmapVertexBuffer();
//
//	ConstDataMat();
//	
//	MatrixMap(pTexture);
//	SetCmdList(pTexture);
//}
//

void Sprite2DBase::MatrixMap(Texture* texture)
{

	SpriteConstBufferData* constBufferData;
	constBuffer->Map(0, nullptr, (void**)&constBufferData);

	DirectX::XMMATRIX matWorld = DirectX::XMMatrixIdentity();
	
	// �g�k
	matWorld *= DirectX::XMMatrixScaling
	(
		constData.scale.x,
		constData.scale.y,
		1
	);

	// ��]�O�Ɉړ�
	Vector2 textureSize = 1.0f;
	if (texture)textureSize = texture->GetTextureSize();
	
	// rotationPoint = 0�̎��ɍ����]�ɂȂ�悤�ɓ�����
	if (texture) 
	{
		matWorld *= DirectX::XMMatrixTranslation(-rotationPoint.x + textureSize.x / 2, -rotationPoint.y + textureSize.y / 2, 0.0f);
	}
	else
	{
		matWorld *= DirectX::XMMatrixTranslation(-rotationPoint.x + constData.scale.x / 2, -rotationPoint.y + constData.scale.y / 2, 0.0f);
	}

	// ��]
	matWorld *= DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(constData.angle.z));
	/*matWorld *= DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(constData.angle.x));
	matWorld *= DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(constData.angle.y));*/

	float width = textureSize.x;
	float height = textureSize.y;
	width /= 2;
	height /= 2; 

	// ���W�̐��l�̕����ɍ��オ����悤�ɃZ�b�g���AscalingPoint�����炵�ċ^���I�Ȋg�k�ʒu�����炷�������s��
	matWorld *= DirectX::XMMatrixTranslation
	(
		constData.position.x + (width * constData.scale.x) + (vertices[2].pos.x - width) - (scalingPoint.x * (constData.scale.x - 1)),
		constData.position.y + (height * constData.scale.y) + (vertices[0].pos.y - height) - (scalingPoint.y * (constData.scale.y - 1)),
		0.0f
	);


	//���S��g�k
	/*matWorld *= DirectX::XMMatrixTranslation
	(
		constData.position.x + (vertices[2].pos.x - width) + width,
		constData.position.y + (vertices[0].pos.y - height) + height,
		0.0f
	);*/

	// �߂�
	if (texture)
	{
		matWorld *= DirectX::XMMatrixTranslation(rotationPoint.x - textureSize.x / 2, rotationPoint.y - textureSize.y / 2, 0.0f);
	}
	else
	{
		matWorld *= DirectX::XMMatrixTranslation(rotationPoint.x - constData.scale.x / 2, rotationPoint.y - constData.scale.y / 2, 0.0f);
	}


	constBufferData->mat = matWorld * cameraMatrix;


	constBuffer->Unmap(0, nullptr);
}

