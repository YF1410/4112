#pragma once

#include<vector>

//���Z�q�I�[�o�[���[�h����?
//�}�l�[�W���[�ł�Update�ƃX�g�b�v�t���O�֐���߂āA
//�֐��Ăяo���Ă�ԃX�g�b�v���Ȃ�(Update����)���Ďd�l�ɂ���?


namespace MelLib
{
	//�t���[���^�C�}�[(�v���O�����ˑ��^�C�}�[)
	class FrameTimer
	{
	private:
		//�^�C�}�[�̃|�C���^�̔z��(Update�p)
		static std::vector<FrameTimer*>pTimers;

		//���݂̎���
		int time = 0;
		//�O�t���[���̎���
		int preTime = 0;
		//�ő�v�����ԂɒB�����Ƃ��Ɍ��݂̎��Ԃ��㏑�����鎞��
		int resetMaxTime = 0;
		//�ŏ��v�����ԂɒB�����Ƃ��Ɍ��݂̎��Ԃ��㏑�����鎞��
		int resetMinTime = 0;
		//�ő�v������
		int maxTime = INT_MAX;
		//�ŏ��v������
		int minTime = INT_MIN;
		//�v�����邩�ǂ���
		bool isStop = true;
		//�ő�v�����ԂɒB�����u�Ԃ��ǂ���
		bool timeMaxMoment = false;
		//�ŏ��v�����ԂɒB�����u�Ԃ��ǂ���
		bool timeMinMoment = false;

		//���Ԃ����Z���邩���Z���邩
		bool isDecrement = false;
		//���Z�b�g���邩�ǂ���
		bool resetFlag = false;

	private:
		void Update();
	public:

		FrameTimer();

		~FrameTimer();

	

		static void AllUpdate();

		/// <summary>
		/// ���݂̎��Ԃ��ő�v�����ԂɒB�����Ƃ��̎��Ԃɂ��܂��B
		/// </summary>
		void ResetMaxTime() { time = resetMaxTime; }

		/// <summary>
		/// ���݂̎��Ԃ��ŏ��v�����ԂɒB�����Ƃ��̎��Ԃɂ��܂��B
		/// </summary>
		void ResetMinTime() { time = resetMinTime; }

		/// <summary>
		/// ���Ԃ�0�Ń��Z�b�g���܂��B
		/// </summary>
		void ResetTimeZero() { time = 0; }

#pragma region �Q�b�g


		/// <summary>
		/// �^�C�}�[����~���Ă��邩��Ԃ��܂��B
		/// </summary>
		/// <returns></returns>
		bool GetStopFlag()const { return isStop; }

		/// <summary>
		///	���݂̎��Ԃ�Ԃ��܂��B
		/// </summary>
		/// <returns></returns>
		int GetNowTime() const { return time; }

		/// <summary>
		/// �ő�v�����Ԃ��擾���܂��B
		/// </summary>
		/// <returns></returns>
		int GetMaxTime() const { return maxTime; }

		/// <summary>
		/// �ŏ��v�����Ԃ��擾���܂��B
		/// </summary>
		/// <returns></returns>
		int GetMinTime() const { return minTime; }

		/// <summary>
		/// �ő�v�����ԂɒB�����Ƃ��Ɍ��݂̎��Ԃ��㏑�����鎞�Ԃ��擾���܂��B
		/// </summary>
		/// <returns></returns>
		int GetResetMaxTime()const { return resetMaxTime; }

		/// <summary>
		/// �ŏ��v�����ԂɒB�����Ƃ��Ɍ��݂̎��Ԃ��㏑�����鎞�Ԃ��擾���܂��B
		/// </summary>
		/// <returns></returns>
		int GetResetMinTime()const { return resetMinTime; }

		/// <summary>
		/// �ő�v�����Ԃƌ��݂̎��Ԃ������ɂȂ����u�Ԃ�true��Ԃ��܂��B
		/// </summary>
		/// <returns></returns>
		bool GetSameAsMaxFlag()const { return timeMaxMoment; }

		/// <summary>
		/// �ŏ��v�����Ԃƌ��݂̎��Ԃ������ɂȂ����u�Ԃ�true��Ԃ��܂��B
		/// </summary>
		/// <returns></returns>
		bool GetSameAsMinFlag() const { return timeMinMoment; }

		bool GetMaxJustFlag()const { return time == maxTime; }
		bool GetMinJustFlag()const { return time == minTime; }

		bool GetMaxOverFlag()const { return time >= maxTime; }
		bool GetMinOverFlag()const { return time <= minTime; }

		/// <summary>
		/// ���Ԃ��Z�b�g�������l�̔{���������ꍇ�Atrue��Ԃ��܂��B
		/// </summary>
		/// <param name="num">���l</param>
		/// <returns></returns>
		bool GetMultipleTimeFlag(const int num)const { return time % num == 0; }

		/// <summary>
		/// �Z�b�g�������Ԃ��ƂɃt���O��true��false�ɐ؂�ւ��ĕԂ��܂��Bfalse����X�^�[�g���܂��B
		/// </summary>
		/// <param name="num">���l</param>
		/// <returns></returns>
		int GetSetByNumberFlag(const int num) const { return time % (num * 2) >= num; }
#pragma endregion

#pragma region �Z�b�g

		/// <summary>
		/// �^�C�}�[���~���邩�ǂ����̃t���O���Z�b�g���܂��A
		/// </summary>
		/// <param name="flag"></param>
		void SetStopFlag(const bool flag) { isStop = flag; }


		/// <summary>
		/// �ő�܂��͍ŏ��v�����ԂɒB�����Ƃ��ɁA���Ԃ������I�Ƀ��Z�b�g���邩�ǂ����̃t���O���Z�b�g���܂��B
		/// </summary>
		/// <param name="flag"></param>
		void SetResetFlag(const bool flag) { resetFlag = flag; }

		/// <summary>
		/// ���Ԃ����Z���Ă��������Z���Ă��������w�肵�܂��B
		/// </summary>
		/// <param name="flag"></param>
		void SetDecrementFlag(const bool flag) { isDecrement = flag; }

		/// <summary>
		/// �ő�v�����ԂɒB�����Ƃ��Ɍ��݂̎��Ԃ����Z�b�g���邽�߂̎��Ԃ�ݒ肵�܂��B
		/// </summary>
		/// <param name="num"></param>
		void SetResetMaxTime(const int num) { resetMaxTime = num; }

		/// <summary>
		/// �ŏ��v�����ԂɒB�����Ƃ��Ɍ��݂̎��Ԃ����Z�b�g���邽�߂̎��Ԃ�ݒ肵�܂��B
		/// </summary>
		/// <param name="num"></param>
		void SetResetMinTime(const int num) { resetMinTime = num; }

		/// <summary>
		/// �ő�v�����Ԃ�ݒ肵�܂��B
		/// </summary>
		/// <param name="num"></param>
		void SetMaxTime(const int num) { maxTime = num; }

		/// <summary>
		/// �ŏ��v�����Ԃ�ݒ肵�܂��B
		/// </summary>
		/// <param name="num"></param>
		void SetMinTime(const int num) { minTime = num; }

		/// <summary>
		/// �^�C�}�[�̌��݂̎��Ԃ�ݒ肵�܂��B
		/// </summary>
		/// <param name="num"></param>
		void SetNowTime(const int num) { time = num; }
#pragma endregion

	};

}