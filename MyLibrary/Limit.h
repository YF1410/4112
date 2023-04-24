#pragma once

namespace MelLib
{

	//���l���N�����v�������Ԃ��邷��N���X
	//�C�[�W���O�Ƃ܂Ƃ߂�?�C�[�W���O��Ԃ���
	class Limit
	{
	public:

		/// <summary>
		/// num��numMin������������numMin�ɁAnumMax���z������umMax�ɂ��܂��B
		/// </summary>
		/// <typeparam name="T">���l�̌^</typeparam>
		/// <param name="num">���l</param>
		/// <param name="numMin">�Œ�l</param>
		/// <param name="numMax">�ō��l</param>
		/// <returns></returns>
		template<class T>
		static T Clamp(T num, const T numMin, const T numMax)
		{
			if (num > numMax)num = numMax;
			else if (num < numMin)num = numMin;
			return num;
		}

		static float Smoothstep(float num, const float numMin, const float numMax)
		{
			num = Clamp((num - numMin) / (numMax - numMin), 0.0f, 1.0f);
			return num * num * (3 - 2 * num);
		}
	};

}