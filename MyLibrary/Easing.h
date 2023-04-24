#pragma once
#include"Vector.h"

namespace MelLib
{
	/*class Easing
	{
	private:
		Easing() {}
		~Easing() {}

		static Vector3 CalcEasing(const Vector3& startPos, const Vector3& endPos, const float t);

	public:
		static Vector3 EaseIn
		(
			const Vector3& startPos,
			const Vector3& endPos,
			const float time
		);

		static Vector3 EaseOut
		(
			const Vector3& startPos,
			const Vector3& endPos,
			const float time
		);

		static Vector3 EaseInOut
		(
			const Vector3& startPos,
			const Vector3& endPos,
			const float time
		);



	};*/

	//�C�[�W���O�̏��AT�����������N���X
	template<class V>
	class Easing
	{
	private:
		V value = 0.0f;
		V start = 0.0f;
		V end = 1.0f;

		//�p�[�Z���g
		float par = 0.0f;
		//�֐��Ăяo�����̉��Z�l
		float addPar = 0.0f;

		V CalcEasing (const V startPos, const V endPos, const float t)const
		{
			return startPos * (1.0f - t) + endPos * t;
		}

		void Clamp()
		{
			if (par >= 100.0f)par = 100.0f;
			else if (par <= 0.0f)par = 0.0f;
		}
	public:
		Easing() {}
		Easing(V start, V end) :start(start), end(end) {}
		Easing(V start, V end, const float addPar) :start(start), end(end), addPar(addPar) {}
		Easing(V start, V end, const float addPar, const float par) :start(start), end(end), par(par), addPar(addPar) {}

		V Lerp(V v)
		{
			float max1Par = (v / 100.0f);
			value = CalcEasing(start, end, max1Par);
			return value;
		}

		/// <summary>
		/// �p�[�Z���g�̉��Z���ɍs���Ă���⊮���܂��B
		/// </summary>
		/// <returns></returns>
		V PreLerp()
		{
			par += addPar;
			float max1Par = (par / 100.0f);
			value = CalcEasing(start, end, max1Par);
			return value;
		}
		V Lerp()
		{
			float max1Par = (par / 100.0f);
			value = CalcEasing(start, end, max1Par);
			par += addPar;
			return value;
		}



		V EaseIn()
		{
			par += addPar;
			float max1Par = (par / 100.0f);
			float t = (max1Par * max1Par);
			value = CalcEasing(start, end, t);
			return value;
		}

		V EaseOut()
		{
			par += addPar;
			float max1Par = (par / 100.0f);
			float t = max1Par * (2 - max1Par);
			value = CalcEasing(start, end, t);
			return value;
		}

		V EaseInOut()
		{
			par += addPar;
			float max1Par = (par / 100.0f);
			float t = max1Par * max1Par * (3 - 2 * max1Par);
			value = CalcEasing(start, end, t);
			return value;
		}

		/// <summary>
		/// �͈͂̊J�n���Z�b�g���܂��B
		/// </summary>
		/// <param name="start"></param>
		void SetStart(const V start) { this->start = start; }

		/// <summary>
		/// �͈͂̏I�����Z�b�g���܂��B
		/// </summary>
		/// <param name="end"></param>
		void SetEnd(const V end) { this->end = end; }

		/// <summary>
		/// start����end�͈̔͂ǂ̕ӂ���%�Őݒ肵�܂��B
		/// </summary>
		/// <param name="par"></param>
		void SetPar(const float par) { this->par = par; }

		/// <summary>
		/// EaseIn�Ȃǂ�value�ɑ������֐����Ăяo�������ɉ��Z����l��ݒ肵�܂��B
		/// </summary>
		/// <param name="par"></param>
		void SetAddPar(const float par) { addPar = par; }

		V GetValue()const { return value; }
		V GetStart()const { return start; }
		V GetEnd() const { return end; }
		float GetPar()const { return par; }
		float GetAddPar()const { return addPar; }

		// ��Ԃ�������1�t���[���̕ω��ʂ�Ԃ��܂�
		V GetFrameLarpValue()const
		{
			float oneFramePar = (addPar / 100.0f);
			V moveStartPos = CalcEasing(start, end, 0);
			V moveEndPos = CalcEasing(start, end, oneFramePar);
			return moveEndPos - moveStartPos;
		}
	};
}
