#include"FbxShaderHeader.hlsli"
#include"ADSAHeader.hlsli"

//一応テクスチャ1枚でもTexture3Dで描画できるが、
//スプライトとバッファの生成関数同じだからスプライト側をどうにかしないといけない

Texture3D<float4> tex:register(t0);
Texture3D<float4> tex2:register(t1);
SamplerState smp:register(s0);

float4 GetTexColor(GSOutput input,Texture3D<float4> t)
{
	//ワールド行列適応
	float start = mul(worldMat, tex3DStart);
	float end = mul(worldMat, tex3DEnd);
	//smoothstepと範囲で、wを求める
	float w = smoothstep(start, end, input.worldPos.z);

	return t.Sample(smp, float3(input.uv, w)) + baseColor;

}



float4 GetShaderColor(GSOutput input) 
{
	float4 shaderColor = m_ambient;
	const float shininess = 4.0f;//光沢度
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

	////ワールド行列適応
	//float start = mul(worldMat,tex3DStart);
	//float end = mul(worldMat, tex3DEnd);
	////smoothstepと範囲で、wを求める
	//float w = smoothstep(start, end,input.worldPos.z);

	//float4 texColor = tex2.Sample(smp, float3(input.uv, w)) + baseColor;
	//float4 shaderColor = m_ambient;

	//const float shininess = 4.0f;//光沢度
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

	////べた塗りテクスチャ使うと単色マテリアル用意するごとにテクスチャバッファ用意しないといけないからこうする(/2する)。
	////べた塗テクスチャ使ってないけど、1オーバーするし、saturateより処理速い可能性あるから、/2
	//// この処理忘れちゃったから要確認(2022/4/11)
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