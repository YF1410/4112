#pragma once
#include<DirectXMath.h>
#include<vector>
#include"Quaternion.h"
#include"LibraryEnum.h"

//#include"GuiValue.h"

namespace MelLib
{
	struct Vector3;
	struct Quaternion;


	struct Vector2
	{
		float x, y;

		Vector2();
		Vector2(const float num);
		Vector2(const float x, const float y);
		Vector2(const DirectX::XMFLOAT2& f);
		Vector2(const DirectX::XMVECTOR& v);

		//XMFLOAT�̓L���X�g��������ǂ��ɂ�����
		//static�̃L���X�g(static Vector3 ToVector3(XMFLOAT);)���?���ł�Quaternion�Ƃ���static�̃L���X�g�����?

		Vector3 ToVector3()const;
		Quaternion ToQuaternion()const;
		DirectX::XMFLOAT2 ToXMFLOAT2()const	{return { x,y };}

		static Vector2 ToVector2(const Vector3& v);
		static Vector2 ToVector2(const Quaternion& q);
		static Vector2 ToVector2(const DirectX::XMFLOAT2& f);

#pragma region ���Z�q

		void operator+= (const Vector2& vector);
		void operator-= (const Vector2& vector);
		void operator*= (const Vector2& vector);
		void operator/= (const Vector2& vector);

		bool operator==(const Vector2& vector);
		bool operator!=(const Vector2& vector);


		void operator++();
		void operator--();

		Vector2 operator-();
#pragma endregion

#pragma region �֐�
		//static�̕��͕��ʂ̃����o�֐��Ăяo����return����΂�������
		//�v�Z���ʎ����ăo�O�Ȃ�������7
		//static�̕�������������Ȃ�?Vector2().Abs()�ď����邵


		float Length()const;
		static float Length(const Vector2& vector);

		Vector2 Normalize()const;
		static Vector2 Normalize(const Vector2& vector);

		float Dot(const Vector2& vector)const;
		static float Dot(const Vector2& vector1, const Vector2& vector2);

		float Cross(const Vector2& vector)const;
		static float Cross(const Vector2& vector1, const Vector2& vector2);

		Vector2 Abs()const;
		static Vector2 Abs(const Vector2& vector);



#pragma endregion


	};
#pragma region ���Z�q

	Vector2 operator+(const Vector2& vector1, const Vector2& vector2);
	Vector2 operator-(const Vector2& vector1, const Vector2& vector2);
	Vector2 operator*(const Vector2& vector1, const Vector2& vector2);
	Vector2 operator/(const Vector2& vector1, const Vector2& vector2);
	Vector2 operator*(const Vector2& vector, const float f);
	Vector2 operator+(const Vector2& vector, const float f);
	Vector2 operator-(const Vector2& vector, const float f);
	Vector2 operator/(const Vector2& vector, const float f);
	Vector2 operator+(const float f, const Vector2& vector);
	Vector2 operator-(const float f, const Vector2& vector);
	Vector2 operator*(const float f, const Vector2& vector);
	Vector2 operator/(const float f, const Vector2& vector);
#pragma endregion


#pragma region �֐�

	Vector2 Vector2Normalize(const Vector2& vector);

	float Vector2Dot(const Vector2& vector1, const Vector2& vector2);

	float Vector2Cross(const Vector2& vector1, const Vector2& vector2);

#pragma endregion



	struct Vector3
	{
		float x, y, z;

		Vector3();
		Vector3(const float num);
		Vector3(const float x, const float y, const float z);
		Vector3(const DirectX::XMFLOAT3& f);
		Vector3(const DirectX::XMVECTOR& v);
		//Vector3(const GuiVector3& value);

		Vector2 ToVector2() const;
		Quaternion ToQuaternion()const;

		static Vector3 ToVector3(const Vector2& v);
		static Vector3 ToVector3(const Quaternion& q);
		static Vector3 ToVector3(const DirectX::XMFLOAT3& f);

		DirectX::XMFLOAT3 ToXMFLOAT3() const { return { x,y ,z }; }


#pragma region ���Z�q

		void operator+= (const Vector3& vector);
		void operator-= (const Vector3& vector);
		void operator*= (const Vector3& vector);
		void operator/= (const Vector3& vector);


		bool operator==(const Vector3& vector) const;
		bool operator!=(const Vector3& vector)const;

		float& operator[](const unsigned int num);

		void operator++();
		void operator--();

		Vector3 operator-();
#pragma endregion

#pragma region �֐�
		float Length()const;
		static float Length(const Vector3& vector);

		Vector3 Normalize()const;
		static Vector3 Normalize(const Vector3& vector);

		float Dot(const Vector3& vector)const;
		static float Dot(const Vector3& vector1, const Vector3& vector2);

		Vector3 Cross(const Vector3& vector)const;
		static Vector3 Cross(const Vector3& vector1, const Vector3& vector2);

		Vector3 Abs()const;
		static Vector3 Abs(const Vector3& vector);
		
		/// <summary>
		/// standardVec�ɋ߂����A�܂��͉������Ƀ\�[�g�����܂��B
		/// </summary>
		/// <param name="vectors">�\�[�g����Vector3�̔z��</param>
		/// <param name="standardVec">�\�[�g�̊�ƂȂ�l</param>
		/// <param name="type ">�\�[�g�^�C�v</param>
		/// <returns></returns>
		static std::vector<Vector3>Sort(const std::vector<Vector3>& vectors,const Vector3& standardVec, const SortType type);



#pragma endregion
	};
#pragma region ���Z�q
	Vector3 operator+(const Vector3& vector1, const Vector3& vector2);
	Vector3 operator-(const Vector3& vector1, const Vector3& vector2);
	Vector3 operator*(const Vector3& vector1, const Vector3& vector2);
	Vector3 operator/(const Vector3& vector1, const Vector3& vector2);
	Vector3 operator+(const Vector3& vector, const float f);
	Vector3 operator-(const Vector3& vector, const float f);
	Vector3 operator*(const Vector3& vector, const float f);
	Vector3 operator/(const Vector3& vector, const float f);
	Vector3 operator+(const float f, const Vector3& vector);
	Vector3 operator-(const float f, const Vector3& vector);
	Vector3 operator*(const float f, const Vector3& vector);
	Vector3 operator/(const float f, const Vector3& vector);
#pragma endregion


#pragma region �֐�

	Vector3 Vector3Normalize(const Vector3& vector);

	Vector3 Vector3Cross(const Vector3& vector1, const Vector3& vector2);

	float Vector3Dot(const Vector3& vector1, const Vector3& vector2);
#pragma endregion

}