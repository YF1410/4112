#pragma once

namespace MelLib
{
	enum class FORMAT_TYPE
	{
		UNSIGNED_INT,//unsigned int
		SIGNED_INT,//int
		FLOAT,//float
	};

	//�p�C�v���C���쐬����
	struct InputLayoutData
	{

		//�Z�}���e�B�N�X��
		std::string semantics;

		//����l�̐�
		UINT number = 4;

		//����l�̌^
		FORMAT_TYPE formatType = FORMAT_TYPE::FLOAT;


	};


	enum class DrawMode
	{
		SOLID,//�ʏ�ʂ胂�f����\�����܂�
		WIREFRAME,//���C���[�t���[���ŕ\�����܂�
	};

	enum class CullMode
	{
		NONE,//�J�����O���܂���
		FRONT,//�\�ʂ��J�����O���܂�
		BACK//���ʂ��J�����O���܂�
	};

	enum class BlendMode
	{
		NONE,
		ADD,
		SUB,
	};


	//�p�C�v���C���̐ݒ���܂Ƃ߂��\����
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

	//�V�F�[�_�[�̏����܂Ƃ߂��\����
	struct ShaderData
	{
		const wchar_t* shaderPath = nullptr;
		const char* entryPoint = nullptr;
		const char* ver = nullptr;
	};

	struct ShaderDataSet
	{
		//���_�V�F�[�_�[���
		ShaderData vShaderData = { L"../MyLibrary/FbxVertexShader.hlsl","main","vs_5_0" };

		//�n���V�F�[�_�[���
		ShaderData hShaderData = { L"NULL","","" };

		//�h���C���V�F�[�_�[���
		ShaderData dShaderData = { L"NULL","","" };

		//�W�I���g���V�F�[�_�[���
		ShaderData gShaderData = { L"LIB","main","gs_5_0" };

		//�s�N�Z���V�F�[�_�[���
		ShaderData pShaderData = { L"../MyLibrary/FbxPixelShader.hlsl","main","ps_5_0" };
	};
}
