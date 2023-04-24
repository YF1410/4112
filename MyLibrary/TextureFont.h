#pragma once
#include"Texture.h"
#include"Values.h"

namespace MelLib
{
	//SpriteFont�Ŏg�p����e�N�X�`���N���X
	class TextureFont
	{
	private:
		static std::unordered_map < std::string, std::unique_ptr<TextureFont >> pTextureFonts;

		Texture* pTexture = nullptr;

		//�c���̐�
		Value2<UINT>fonsLineNum;

	public:
		TextureFont() {}
		~TextureFont() {}

		Texture* GetPTexture() { return pTexture; }
		Value2<UINT>GetFontLineNumber() { return fonsLineNum; }


		static bool Load(const std::string& path, const Value2<UINT>lineNum, const std::string& name);
		static TextureFont* Get(const std::string& name) { return pTextureFonts[name].get(); }
		static void Delete(const std::string& name);

	};

}
