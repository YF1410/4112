#include"PostEffectTestShaderHeader.hlsli"

VSOutput VSmain(float4 pos : POSITION, float2 uv : TEXCOORD)
{
	VSOutput output;
	output.svpos = mul(mat, pos);//�s��ƍ��W����Z
	output.uv = uv;
	return output;
}