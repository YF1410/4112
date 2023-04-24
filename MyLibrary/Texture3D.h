#pragma once
#include<d3d12.h>
#include<DirectXTex.h>
#include<wrl.h>
#include<vector>
#include"Texture.h"

namespace MelLib 
{
	//�X�v���C�g�̃A�j���[�V�����Ƃ��ɂ��g������

	//�}�e���A���ƕ��s���Ď��������ق�������?

	//�������̏������e�N�X�`��
	class Texture3D :public Texture
	{
	private:
		template<class T>
		using ComPtr = Microsoft::WRL::ComPtr<T>;

		static std::unordered_map<std::string, std::unique_ptr<Texture3D>>pTextures;
		
	
	public:

		static bool Load(const std::vector<std::string>& path, const std::string& name);
		static void Delete(const std::string& name);
		static Texture3D* Get(const std::string& name) { return pTextures[name].get(); }

		bool Load3DTexture(const std::vector<std::string>& texturePath);
	};
}

