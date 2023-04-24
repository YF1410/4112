#include"ObjShaderHeader.hlsli"

VSOutput VSmain
(
	float4 pos : POSITION, 
	float2 uv : TEXCOORD, 
	float3 normal : NORMAL,
	unsigned int bone : BONENUM
)
{
	matrix boneMat = boneMatArray[bone];

	VSOutput output;


	//�{�[���̍s��̏�Z
	output.worldPos = mul(boneMat, pos);
	output.worldPos = mul(worldMat, output.worldPos);

	//�{�[���̍s��̏�Z
	output.svpos = mul(boneMat, pos);
	output.svpos = mul(mat, output.svpos);

	//�@���ɂ��{�[���̍s��|���Ȃ��Ƃ����Ȃ��̂ł�
	//���K�����邩��normalMat����Ȃ��̂ł�
	//normalMat�̓r���[�v���W�F�N�V�����|���ĂȂ�����
	//mat�͊|���Ă邩��Ⴄ?
	output.normal = normalize(mul(boneMat, normal));
	output.normal = normalize(mul(normalMat, output.normal));
	output.uv = uv;

	return output;

}
