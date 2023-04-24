#pragma once
#include "GameObject2D.h"
#include "Particle2D.h"

// �p�[�e�B�N���N���X�����A�p�[�e�B�N���쐬���͂�����p�����邱��
// �G�~�b�^�[�������Ɏ�ނƃp�[�e�B�N�������w�肷��悤�ɂ���?
// 2D��3D�ŕ�����?

namespace MelLib 
{
    class Emitter2D :public MelLib::GameObject2D
    {
    private:
        std::vector<Particle2D>particles;

    public:
        Emitter2D(const Particle2D* const pParticle, unsigned int particleNum);
        ~Emitter2D(){}

        void Update()override;
        void Draw()override;
        
    };

    //class Particle :
    //    public MelLib::GameObject
    //{

    //};

}