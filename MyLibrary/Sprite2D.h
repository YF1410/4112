#pragma once
#include "Sprite2DBase.h"

#include"RenderTarget.h"

//�g�k�������_������?�s�N�Z���P�ʂ̊g�k���₷��������
namespace MelLib
{
	//�X�v���C�g�N���X
	class Sprite2D :
		public Sprite2DBase
	{
	public:


	private:

		static std::unordered_map<std::string, std::unique_ptr<Sprite2D>> pSprite2D;


	public:
		Sprite2D();
		Sprite2D(const Color& color);
		Sprite2D(Texture* pTexture);
		~Sprite2D();

		void Create();
		void Create(const Color& color);
		void Create(Texture* pTexture);

		/// <summary>
		/// �������܂��B
		/// </summary>
		/// <param name="color"></param>
		/// <param name="name"></param>
		/// <returns></returns>
		//static bool Create(const Color& color, const std::string& name);
		/// <summary>
		/// �������܂��BpTexture��nullptr�̏ꍇ�͉����\������܂���BTexture���Z�b�g���邩�ADrawType��Color�ɂ���Color���Z�b�g����ƕ\������܂��B
		/// </summary>
		/// <param name="pTexture"></param>
		/// <param name="name"></param>
		/// <returns></returns>
		//static bool Create(Texture* pTexture, const std::string& name);
		
		static void Delete(const std::string& name);
		static Sprite2D* Get(const std::string& name) { return pSprite2D[name].get(); }

		void Draw(const std::string& rtName = RenderTarget::GetMainRenderTargetNama())override;

		void SetColor(const Color& color);
		void SetTexture(Texture* pTexture);
	};

}