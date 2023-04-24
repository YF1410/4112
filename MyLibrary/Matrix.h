#pragma once
#include<array>
#include"Vector.h"
#include<DirectXMath.h>

namespace MelLib
{

	struct Matrix
	{
		Matrix();
		Matrix(const DirectX::XMMATRIX& m);

		std::array<std::array<float, 4>, 4> matrix =
		{
			{
				{0,0,0,0},
				{0,0,0,0},
				{0,0,0,0},
				{0,0,0,0}
			}
		};
		
		

		Matrix operator*(const Matrix& mat)const;
		void operator*=(const Matrix& mat);

		std::array<float, 4>& operator[](const unsigned int num);
		std::array<float, 4>operator[](const unsigned int num)const;

		/// <summary>
		/// XMMATRIX�ɕϊ����܂��B
		/// </summary>
		/// <returns></returns>
		DirectX::XMMATRIX ToXMMATRIX();

		/// <summary>
		/// ��s����擾���܂��B
		/// </summary>
		/// <returns></returns>
		static Matrix GetZeroMatrix();

		/// <summary>
		/// �P�ʍs����擾���܂��B
		/// </summary>
		/// <returns></returns>
		static Matrix GetIdentityMatrix();

		/// <summary>
		/// X����ŉ�]������s����擾���܂��B
		/// </summary>
		/// <param name="angle">�p�x</param>
		/// <returns></returns>
		static Matrix GetRotateXMatrix(const float angle);

		/// <summary>
		/// Y����ŉ�]������s����擾���܂��B
		/// </summary>
		/// <param name="angle">�p�x</param>
		/// <returns></returns>
		static Matrix GetRotateYMatrix(const float angle);

		/// <summary>
		/// Z����ŉ�]������s����擾���܂��B
		/// </summary>
		/// <param name="angle">�p�x</param>
		/// <returns></returns>
		static Matrix GetRotateZMatrix(const float angle);

		/// <summary>
		/// ��]�s���ZXY�̏��ԂŊ|���Ď擾���܂��B
		/// </summary>
		/// <param name="angle">�p�x</param>
		/// <returns>�v�Z����</returns>
		static Matrix GetRotateZXYMatrix(const Vector3& angle);

		/// <summary>
		/// �g�k����s����擾���܂��B
		/// </summary>
		/// <param name="vector">�g�k�x</param>
		/// <returns></returns>
		static Matrix GetScalingMatrix(const Vector3& vector);

		/// <summary>
		/// ���s�ړ�����s����擾���܂��B
		/// </summary>
		/// <param name="vector">�ړ���</param>
		/// <returns></returns>
		static Matrix GetTranslationMatrix(const Vector3& vector);
	};
}



