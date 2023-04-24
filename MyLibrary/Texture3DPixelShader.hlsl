#include"FbxShaderHeader.hlsli"
#include"ADSAHeader.hlsli"

Texture2D<float4> tex:register(t0);
SamplerState smp:register(s0);



float4 main(GSOutput input) : SV_TARGET
{
	//float w = smoothstep()

	float4 texColor = tex.Sample(smp, input.uv) + baseColor;
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

	texColor += addColor - subColor;
	texColor *= mulColor;

	//�ׂ��h��e�N�X�`���g���ƒP�F�}�e���A���p�ӂ��邲�ƂɃe�N�X�`���o�b�t�@�p�ӂ��Ȃ��Ƃ����Ȃ����炱������(/2����)�B
	//�ׂ��h�e�N�X�`���g���ĂȂ����ǁA1�I�[�o�[���邵�Asaturate��菈�������\�����邩��A/2
	float alphaSum = (shaderColor.a + texColor.a) / 2;

	return float4(shaderColor.rgb * texColor.rgb, alphaSum);




	//�ȉ�PBR�̏���
	/*N = input.normal;
	float3 finalRGB = float3(0, 0, 0);
	float3 eyedir = normalize(cameraPos.xyz - input.worldPos.xyz);

	for (int i = 0; i < 3; i++)
	{
		finalRGB += BRDF(-light.xyz, eyedir) * lightColor;
	}
	return float4(finalRGB, 1);*/


}