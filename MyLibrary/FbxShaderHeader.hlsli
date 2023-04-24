

struct VSOutput
{
	/*float4 worldPos:POSITION;
	float4 svpos : SV_POSITION;*/

	float4 worldPos : POSITION;
	float4 svpos : SV_POSITION;
	float3 normal:NORMAL;
	float2 uv : TEXCOORD;
};

struct GSOutput
{
	float4 worldPos:POSITION;
	float4 svpos : SV_POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
};


//��{�I�ȏ��
static const int DIR_LIGTH_MAX = 20;
cbuffer cbuff0 : register(b0)
{
	float4 lightColor[DIR_LIGTH_MAX];
	float4 light[DIR_LIGTH_MAX];
	float4 cameraPos;
	matrix mat;
	matrix normalMat;
	matrix worldMat;
	float4 mulColor;
	float4 addColor;
	float4 subColor;
	float2 addUV;
	float par;
	float tex3DStart;
	float tex3DEnd;
	float ex;
};




//�F
cbuffer cbuff4 : register(b4)
{
	float4 baseColor;
}

static const int MAX_BONE_NUM = 64;
//�X�L�j���O
cbuffer cbuff3 : register(b3)
{
	//matrix matSkinning[128];
	matrix matSkinning[MAX_BONE_NUM];
}

