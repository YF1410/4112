#pragma once
#include"Vector.h"
#include<vector>

namespace MelLib
{

	//�����N���X
	class Random
	{
	private:
		Random() {}
		~Random() {}

		using UINT = unsigned int;
	public:

		static void Initialize();

		/// <summary>
		/// �V�[�h�l���Z�b�g���܂��B
		/// </summary>
		/// <param name="seed"></param>
		static void SetSeed(const UINT seed);

		/// <summary>
		/// �V�[�h�l�Ɏ��Ԃ��g�p����悤�ɂ��܂��B
		/// </summary>
		static void SetSeedTime();

		/// <summary>
		///	�������擾���܂��B�擾�ł���l��rand�֐��Ɠ����ł��B
		/// </summary>
		/// <param name="num"></param>
		/// <returns></returns>
		static int GetRandomNumber(const int num);

		/// <summary>
		/// �w�肵���͈͓��̐��̂ǂꂩ���擾���܂��B
		/// </summary>
		/// <param name="start"></param>
		/// <param name="end"></param>
		/// <returns></returns>
		static int GetRandomNumberRangeSelect(const int start, const int end);

		/// <summary>
		/// nums�̒�����ǂꂩ1�������_���ŕԂ��܂��B
		/// </summary>
		/// <param name="nums"></param>
		/// <returns></returns>
		static int GetRandomNumberSetNumber(const std::vector<int>& nums);

		/// <summary>
		/// �����̗������擾���܂��B
		/// </summary>
		/// <param name="num"></param>
		/// <param name="digits">����</param>
		/// <returns></returns>
		static float GetRandomFloatNumber(const float num, const int digits);

		/// <summary>
		/// �w�肵���͈͓��̐��̂ǂꂩ���擾���܂��B
		/// </summary>
		/// <param name="num"></param>
		/// <param name="digits">����</param>
		/// <returns></returns>
		static float GetRandomFloatNumberRangeSelect(const float start, const float end, const int digits);

		/// <summary>
	/// nums�̒�����ǂꂩ1�������_���ŕԂ��܂��B
	/// </summary>
	/// <param name="nums"></param>
	/// <returns></returns>
		static float GetRandomNumberSetFloatNumber(const std::vector<float>& nums);

		/// <summary>
		/// Vector3�̗������擾���܂��B
		/// </summary>
		/// <returns></returns>
		static Vector3 GetRandomVector3Number(const float num, const int digits);

		/// <summary>
		/// �w�肵���͈͓��̐���Vector3�Ɋi�[���܂��B
		/// </summary>
		/// <param name="start"></param>
		/// <param name="end"></param>
		/// <param name="digits"></param>
		/// <returns></returns>
		static Vector3 GetRandomVector3NumberRangeSelect(const float start, const float end, const int digits);

	};

}
