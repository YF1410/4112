#pragma once

namespace MelLib
{
	enum class FORMAT_TYPE
	{
		UNSIGNED_INT,//unsigned int
		SIGNED_INT,//int
		FLOAT,//float
	};

	//パイプライン作成時に
	struct InputLayoutData
	{

		//セマンティクス名
		std::string semantics;

		//送る値の数
		UINT number = 4;

		//送る値の型
		FORMAT_TYPE formatType = FORMAT_TYPE::FLOAT;


	};


	enum class DrawMode
	{
		SOLID,//通常通りモデルを表示します
		WIREFRAME,//ワイヤーフレームで表示します
	};

	enum class CullMode
	{
		NONE,//カリングしません
		FRONT,//表面をカリングします
		BACK//裏面をカリングします
	};

	enum class BlendMode
	{
		NONE,
		ADD,
		SUB,
	};


	//パイプラインの設定をまとめた構造体
	struct DrawOption
	{
		DrawMode drawMode = DrawMode::SOLID;
		CullMode cullMode = CullMode::NONE;
		BlendMode blendMode = BlendMode::ADD;

		bool depthTest = true;
		bool alphaWrite = true;

		void SetModelDefData()
		{
			drawMode = DrawMode::SOLID;
			cullMode = CullMode::NONE;
			blendMode = BlendMode::ADD;
			depthTest = true;
			alphaWrite = true;
		}
	};

	//シェーダーの情報をまとめた構造体
	struct ShaderData
	{
		const wchar_t* shaderPath = nullptr;
		const char* entryPoint = nullptr;
		const char* ver = nullptr;
	};

	struct ShaderDataSet
	{
		//頂点シェーダー情報
		ShaderData vShaderData = { L"../MyLibrary/FbxVertexShader.hlsl","main","vs_5_0" };

		//ハルシェーダー情報
		ShaderData hShaderData = { L"NULL","","" };

		//ドメインシェーダー情報
		ShaderData dShaderData = { L"NULL","","" };

		//ジオメトリシェーダー情報
		ShaderData gShaderData = { L"LIB","main","gs_5_0" };

		//ピクセルシェーダー情報
		ShaderData pShaderData = { L"../MyLibrary/FbxPixelShader.hlsl","main","ps_5_0" };
	};
}
