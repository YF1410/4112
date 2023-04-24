#pragma once
#include"Sprite3D.h"
#include"FrameTimer.h"

namespace MelLib
{
	// �X�v���C�g3D�𗘗p�����p�[�e�B�N�������Ƃ��Ɍp��������N���X
	class Particle3D
	{
	protected:
		Sprite3D particleSprite;

		Vector3 startPosition;
		bool isDead = false;

	protected:
		/// <summary>
		/// �p�[�e�B�N����������(�����ʒu�ɖ߂�)���ǂ����̃t���O��true�ɂ��܂��B
		/// </summary>
		void TrueIsDeadFlag() { isDead = true; }

	public:
		Particle3D(const Color& color);
		Particle3D(Texture* pTexture);

		/// <summary>
		/// �G�~�b�^�[�ɃZ�b�g����p�̃R���X�g���N�^
		/// </summary>
		/// <param name="particle"></param>
		Particle3D(const Particle3D& particle);

		/// <summary>
		/// �G�~�b�^�[�ɃZ�b�g����p�̃R���X�g���N�^
		/// </summary>
		/// <param name="particle"></param>
		Particle3D(const Particle3D& particle, const Vector3& pos);
		virtual ~Particle3D() {}
		
		virtual void Initialize();
		virtual void Update();
		void Draw();


		/// <summary>
		/// �����������Ƃ��ɖ߂��֐�
		/// </summary>
		void ResetParametor() 
		{
			particleSprite.SetPosition(startPosition); 
			isDead = false;
		}

		bool GetIsDead() { return isDead; }
		
	};
}
