#include"FbxShaderHeader.hlsli"
#include"ADSAHeader.hlsli"

//�ꉞ�e�N�X�`��1���ł�Texture3D�ŕ`��ł��邪�A
//�X�v���C�g�ƃo�b�t�@�̐����֐�����������X�v���C�g�����ǂ��ɂ����Ȃ��Ƃ����Ȃ�

Texture3D<float4> tex:register(t0);
Texture3D<float4> tex2:register(t1);
SamplerState smp:register(s0);

float4 GetTexColor(GSOutput input,Texture3D<float4> t)
{
	//���[���h�s��K��
	float start = mul(worldMat, tex3DStart);
	float end = mul(worldMat, tex3DEnd);
	//smoothstep�Ɣ͈͂ŁAw�����߂�
	float w = smoothstep(start, end, input.worldPos.z);

	return t.Sample(smp, float3(input.uv, w)) + baseColor;

}



float4 GetShaderColor(GSOutput input) 
{
	float4 shaderColor = m_ambient;
	const float shininess = 4.0f;//����x
	const float3 eyeDir = normalize(cameraPos - input.worldPos.xyz);
	for (int i = 0; i < DIR_LIGTH_MAX; i++)
	{

		float3 lightDir = light[i];

		float3 dotlightnormal = dot(-lightDir, input.normal);
		float3 reflect = normalize(lightDir + 2 * dotlightnormal * input.normal);

		float3 diffuse = saturate(dotlightnormal * m_diffuse);
		float3 specular = pow(saturate(dot(reflect, eyeDir)), shininess) * m_specular;

		shaderColor.rgb += diffuse + specular * lightColor[i];
	}

	shaderColor.a = m_alpha;

	return shaderColor;
}

float4 BlendAddSubMulColor(float4 texColor,float4 shaderColor) 
{
	float alphaSum = (shaderColor.a + texColor.a) / 2;

	float3 changeColor = texColor.rgb;
	changeColor += addColor.rgb - subColor.rgb;
	changeColor *= mulColor.rgb;

	float changeAlpha = alphaSum;
	changeAlpha += addColor.a - subColor.a;
	changeAlpha *= mulColor.a;

	float4 sumColor = float4(shaderColor * changeColor, changeAlpha);

	return sumColor;
}

float4 main(GSOutput input) : SV_TARGET
{
	float4 texColor = GetTexColor(input,tex);
	float4 shaderColor = GetShaderColor(input);
	float4 sumColor = BlendAddSubMulColor(texColor, shaderColor);

	return sumColor;

	////���[���h�s��K��
	//float start = mul(worldMat,tex3DStart);
	//float end = mul(worldMat, tex3DEnd);
	////smoothstep�Ɣ͈͂ŁAw�����߂�
	//float w = smoothstep(start, end,input.worldPos.z);

	//float4 texColor = tex2.Sample(smp, float3(input.uv, w)) + baseColor;
	//float4 shaderColor = m_ambient;

	//const float shininess = 4.0f;//����x
	//const float3 eyeDir = normalize(cameraPos - input.worldPos.xyz);
	//for (int i = 0; i < DIR_LIGTH_MAX; i++)
	//{

	//	float3 lightDir = light[i];

	//	float3 dotlightnormal = dot(-lightDir, input.normal);
	//	float3 reflect = normalize(lightDir + 2 * dotlightnormal * input.normal);

	//	float3 diffuse = saturate(dotlightnormal * m_diffuse);
	//	float3 specular = pow(saturate(dot(reflect, eyeDir)), shininess) * m_specular;
	//
	//	shaderColor.rgb += diffuse + specular * lightColor[i];
	//}
	//
	//shaderColor.a = m_alpha;

	///*texColor += addColor - subColor;
	//texColor *= mulColor;*/

	////�ׂ��h��e�N�X�`���g���ƒP�F�}�e���A���p�ӂ��邲�ƂɃe�N�X�`���o�b�t�@�p�ӂ��Ȃ��Ƃ����Ȃ����炱������(/2����)�B
	////�ׂ��h�e�N�X�`���g���ĂȂ����ǁA1�I�[�o�[���邵�Asaturate��菈�������\�����邩��A/2
	//// ���̏����Y�ꂿ���������v�m�F(2022/4/11)
	//float alphaSum = (shaderColor.a + texColor.a) / 2;

	//float3 changeColor = texColor.rgb;
	//changeColor += addColor.rgb - subColor.rgb;
	//changeColor *= mulColor.rgb;

	//float changeAlpha = alphaSum;
	//changeAlpha += addColor.a - subColor.a;
	//changeAlpha *= mulColor.a;

	//float4 sumColor = float4(shaderColor * changeColor, changeAlpha);
	//

	///*float4 sumColor = float4(shaderColor.rgb * texColor.rgb, alphaSum);
	//sumColor += addColor - subColor;
	//sumColor *= mulColor;*/

	//return sumColor;
}