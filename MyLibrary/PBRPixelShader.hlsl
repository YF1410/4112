#include"FbxShaderHeader.hlsli"
#include"PBRHeader.hlsli"

static const float PI = 3.141592654f;
static float3 N;
Texture2D<float4> tex:register(t0);
SamplerState smp:register(s0);



float SchlickFresnel(float f0, float f90, float cosine)
{
	float m = saturate(1 - cosine);
	float m2 = m * m;
	float m5 = m2 * m2 * m;
	return lerp(f0, f90, m5);
}

float3 SchlickFresnel3(float3 f0, float3 f90, float cosine)
{
	float m = saturate(1 - cosine);
	float m2 = m * m;
	float m5 = m2 * m2 * m;
	return lerp(f0, f90, m5);
}


//UE4��GGX���z
float DistributionGGX(float alpha, float NdotH)
{
	float alpha2 = alpha * alpha;
	float t = NdotH * NdotH * (alpha2 - 1.0f) + 1.0f;

	return alpha2 / (PI * t * t);

}

//�f�B�Y�j�[�̃t���l���v�Z
float3 DisneyFresnel(float LdotH,float3 color)
{
	//�P�x
	float luminance = 0.3f * color.r + 0.6f * color.g + 0.1f * color.b;
	
	//�F����
	float3 tintColor = color / luminance;

	//������̋��ʔ��ːF
	float3 nonMetalColor = specular * 0.08f * tintColor;

	//metalness�ɂ����(�����̏ꍇ��color�ɂȂ�)
	float3 specularColor = lerp(nonMetalColor, color, metalness);

	return SchlickFresnel3(specularColor, float3(1, 1, 1), LdotH);
}

//G��
float GeometricSmith(float cosine)
{
	float k = (roughness + 1.0f);
	k = k * k / 8.0f;
	return cosine / (cosine * (1.0f - k) + k);
}

//���ʔ��˂̌v�Z
float3 CookTorranceSpecular(float NdotL,float NdotV,float NdotH,float LdotH, float3 color)
{
	//�S�̂̌v�Z��
	//return = D(h)F(v,h)G(l,v,h) / 4(n�El)(n�Ev)

	//D��(���z)(Distribution)
	float Ds = DistributionGGX(roughness * roughness, NdotH);

	//F��(�t���l��)
	float3 Fs = DisneyFresnel(LdotH ,color);

	//G��(�􉽌���)
	float Gs = GeometricSmith(NdotL) * GeometricSmith(NdotV);

	//m��(����)
	float m = 4.0f * NdotL * NdotV;

	return Ds * Fs * Gs / m;
}


//�o�������˗����z�֐�
float3 BRDF(float3 L,float3 V ,float3 color)
{
	//�@���ƃ��C�g�̓���
	float NdotL = dot(N, L);
	//�@���ƃJ���������̓���
	float NdotV = dot(N, V);

	//�ǂ��炩��90�x�ȏゾ������^����
	if (NdotL < 0 || NdotV < 0) 
	{
		return float3(0, 0, 0); 
	}

	//�n�[�t�x�N�g��
	float3 H = normalize(L + V);
	float NdotH = dot(N, H);
	float LdotH = dot(L, H);
	
	//�g�U���˗�
	float diffuseReflectance = 1.0f / PI;

	float energyBias = 0.5f * roughness;
	//���ˊp��90�x�̏ꍇ�̊g�U���˗�
	float Fd90 = energyBias + 2.0f * LdotH * LdotH * roughness;
	//�����Ă������̊g�U���˗�
	float FL = SchlickFresnel(1.0f, Fd90, NdotL);
	//�o�Ă����Ƃ��̊g�U���˗�
	float FV = SchlickFresnel(1.0f, Fd90, NdotV);
	float energyFactor = lerp(1.0f, 1.0f / 1.51f, roughness);
	//�����ďo�Ă����܂ł̊g�U���˗�
	float Fd = FL * FV * energyFactor;

	//�g�U���ˍ�
	float3 diffuseColor = diffuseReflectance * Fd * baseColor * (1 - metalness);


	//���ʔ��ˍ�
	float3 specularColor = CookTorranceSpecular(NdotL, NdotV, NdotH, LdotH,color);

	return diffuseColor + specularColor;
	
}




float4 main(GSOutput input) : SV_TARGET
{
	float4 color = tex.Sample(smp, input.uv) + baseColor;


	//�ȉ�PBR�̏���
	N = input.normal;
	float3 finalRGB = float3(0, 0, 0);
	float3 eyedir = normalize(cameraPos.xyz - input.worldPos.xyz);

	for (int i = 0; i < DIR_LIGTH_MAX; i++)
	{
		finalRGB += BRDF(-light[i].xyz, eyedir,color.rgb) * lightColor[i];
	}

	return float4(finalRGB, color.a);
}
