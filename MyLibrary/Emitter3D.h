#pragma once
#include"GameObject.h"
#include"Particle3D.h"


// �����p�x�Ƃ��������ݒ�ł���悤�ɂ���
// �����̓p�[�e�B�N���N���X�ł�������
// �������ł������w��ł���悤�ɂ��Ă���������


namespace MelLib
{
    class Emitter3D :public GameObject
	{
    private:
        //std::vector<std::shared_ptr<Particle3D>>particles;
        std::vector<Particle3D>particles;

        // Update�ADraw���s�����ǂ����̃t���O
        std::vector<bool>particleUpdateDrawFlag;
        
        // ���o�Ԋu
        FrameTimer releaseTimer;
        // ���o��~�t���O
        bool isStop = false;

        
    private:

        void CheckChangeUpdateDrawFlag();

        /// <summary>
        /// �p�[�e�B�N���X�V����
        /// </summary>
        void ParticleUpdate();

    public:
       // Emitter3D(const std::vector<std::shared_ptr<Particle3D>>& pParticle, const Vector3& pos, unsigned int releaseTime);
        Emitter3D(const Particle3D& pParticle, unsigned int particleNum, unsigned int releaseTime, const Vector3& pos,const std::string& name);
        ~Emitter3D() {}

        void Update()override;
        void Draw()override;

        void SetIsStop(const bool flag) 
        {
            isStop = flag; 
            releaseTimer.SetStopFlag(flag);
        }
        
       /* void SetPosition(const Vector3& pos) { SetPosition(pos); }

        Vector3 GetPosition()const { return GetPosition(); }*/
	};
}

