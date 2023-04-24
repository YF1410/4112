#pragma once
#include<limits>
#include<vector>

//�|�C���^��z��ɂ܂Ƃ߂��Ȃ�(�u�Ԃ̔�����Ƃ�Ȃ�)����A�N���X�e���v���[�g��߂�
//�R���X�g���N�^��֐��ȂǂŃp�����[�^����C�ɃZ�b�g�ł���悤�ɂ���?

namespace MelLib
{
	//�J�E���^�[�N���X
	class Counter
	{
	private:
		static std::vector<Counter*>pCounter;

		int count = 0;
		int preCount = -1;
		int resetMaxNum = INT_MAX;
		int resetMinNum = INT_MIN;
		int maxCount = INT_MAX;
		int minCount = INT_MIN;

		//���̃t���[���Ŏ����I�Ƀ��Z�b�g���邩�ǂ���
		bool countReset = false;

		void Update();

	public:

		Counter();
		Counter(const int num);
		~Counter();

		void operator++() { count++; }
		void operator--() { count--; }

		void IncrementCount() { count++; }
		void DecrementCount() { count--; }
		void AddCount(const int num) { count += num; }
		void SubCount(const int num) { count -= num; }
		void ResetMaxCount() { count = resetMaxNum; }
		void ResetMinCount() { count = resetMinNum; }
		void ResetCountZero() { count = 0; }

#pragma region �Z�b�g
		void SetCount(const int num) { count = num; }
		void SetResetMaxNum(const int num) { resetMaxNum = num; }
		void SetResetMinNum(const int num) { resetMaxNum = num; }

		/// <summary>
		///	Library�N���X��LoopStartProcess�֐��Ăяo�����ɃJ�E���g���ő�܂��͍ŏ��J�E���g���ɒB���Ă����ꍇ�A
		/// �����Ń��Z�b�g���邩�ǂ����̃t���O���Z�b�g���܂��B
		/// </summary>
		/// <param name="flag"></param>
		void SetResetFlag(const bool flag) { countReset = flag; }
#pragma endregion


#pragma region �Q�b�g

		int GetCount() { return count; }
		bool GetCountMax() { return count >= maxCount; }
		bool GetCountMin() { return count <= minCount; }
#pragma endregion

		static void AllUpdate();
	};

	//�J�E���^�[�N���X(float�^)
	class CounterF
	{
	private:
		float count = 0;
		float preCount = -1;
		float resetNum = 0;
		float maxCount = FLT_MAX;
		float minCount = FLT_MIN;


	public:
		CounterF() {}
		CounterF(const float num) :count(num), preCount(count - 1) {}


		void operator++() { count++; }
		void operator--() { count--; }

		void IncrementCount() { count++; }
		void DecrementCount() { count--; }
		void AddCount(const float num) { count += num; }
		void SubCount(const float num) { count -= num; }
		void ResetCount() { count = resetNum; }
		void ResetCountZero() { count = 0; }

#pragma region �Z�b�g
		void SetCount(const float num) { count = num; }
		void SetResetNum(const float num) { resetNum = num; }
#pragma endregion


#pragma region �Q�b�g

		float GetCount() { return count; }

#pragma endregion

	};
}