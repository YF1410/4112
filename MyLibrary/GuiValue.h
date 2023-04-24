#pragma once
#include<string>
#include<unordered_map>
#include<vector>

#include"Vector.h"

// struct�ł�����?
// �ݒ肵���l�������o����悤�ɁA�t�@�C���̓E�B���h�E���Ƃł���?
// �e�X�g�v���C���͕ϐ��̒l���ς���Ă��e���󂯂Ȃ��悤��
// �e�X�g�v���C���͕ϐ��̒l�ς����邯�ǁA�G�f�B�b�g�ɖ߂�����SET_VALUE�ɖ߂��悤�ɂ���
// �ǂݍ��񂾎��Ƀ\�[�X�R�[�h�ƒl����v���Ȃ�������ǂݍ��񂾃t�@�C���̒l��D��(Unity�Ɠ����ɂ���)

// �R���X�g���N�^�Ńf�[�^����������ǂݍ��ނ悤�ɂ���

namespace MelLib 
{

	class GuiOption
	{
	private:
		std::string inOutPath;

		GuiOption(){}
		~GuiOption() {}
	public:
		GuiOption(GuiOption& o) = delete;
		GuiOption& operator=(GuiOption& o) = delete;
		static GuiOption* GetInstance();

		/// <summary>
		/// Gui�Őݒ肵���l�̏��̓��o�͐��ݒ肵�܂��B
		/// </summary>
		/// <param name="path"></param>
		void SetGuiDataPath(const std::string& path) { inOutPath = path; }
		std::string GetGuiDataPath() { return inOutPath; }

	};

	// Unity��SerializeField�݂����Ȃ��
	class GuiInt
	{
	private:
		int value = 0;
		int valueMin = 0;
		int valueMax = 0;
		int setValue = 0;
		std::string windowName;
		std::string lavel;
		
	
		bool typingInput = false;
	public:
		GuiInt() {}
		/// <summary>
		/// 
		/// </summary>
		/// <param name="value"></param>
		/// <param name="windowName"></param>
		/// <param name="lavel"></param>
		/// <param name="minNumber"></param>
		/// <param name="maxNumber"></param>GuiInt(int value, const std::string& windowName, const std::string& lavel,int minNumber,int maxNumber);
		GuiInt(const int value, const std::string& windowName, const std::string& lavel, int minNumber, int maxNumber);
		~GuiInt();

		void SetData(int value, const std::string& windowName, const std::string& lavel, int minNumber, int maxNumber);
		
		void operator=(const int num) { this->value = num; }
		GuiInt& operator=(const GuiInt& value)
		{
			*this = value;
			return *this;
		}

		GuiInt& operator+=(const GuiInt& value)
		{
			this->value += value.GetValue();
			return *this;
		}

		void operator++() { value++; }
		void operator--() { value--; }

		
		int GetValue()const { return value; }
		int GetMaxValue() const { return valueMax; }
		int GetMinValue()const { return valueMin; }
		int& GetRefValue() { return value; }

		bool GetTypingInputValueFlag()const { return typingInput; }

		/// <summary>
		/// �ǂݍ��܂ꂽ�l���Z�b�g���܂��B�ǂݍ��񂾒l���Z�b�g�������ꍇ��
		/// ���C�u�����̏������������n�܂�O�ɃR���X�g���N�^���Ăяo����Ă��܂��A
		/// �f�[�^���Z�b�g�ł��Ȃ������ꍇ�ɂ��g�����������B
		/// </summary>
		void SetLoadData();

		void SetValue(int value) { this->value = value; }

		void SetTypingInputFlag(const bool flag) { typingInput = flag; }

	};

	class GuiFloat
	{
	private:
		float value = 0.0f;
		float valueMin = 0.0f;
		float valueMax = 0.0f;
		float setValue = 0.0f;
		std::string windowName;
		std::string lavel;

		bool typingInput = false;
	public:
		GuiFloat(){}
		/// <summary>
		/// 
		/// </summary>
		/// <param name="value"></param>
		/// <param name="windowName"></param>
		/// <param name="lavel"></param>
		/// <param name="minNumber"></param>
		/// <param name="maxNumber"></param>
		GuiFloat(float value, const std::string& windowName, const std::string& lavel, float minNumber, float maxNumber);
		~GuiFloat();

		void SetData(float value, const std::string& windowName, const std::string& lavel, float minNumber, float maxNumber);

		GuiFloat& operator=(const float num) 
		{
			this->value = num; 
			return *this; 
		}

		GuiFloat& operator+=(const GuiFloat& value)
		{
			this->value += value.GetValue();
			return *this;
		}

		void operator++() { value++; }
		void operator--() { value--; }

		float GetValue()const { return value; }
		float GetMaxValue() const { return valueMax; }
		float GetMinValue()const { return valueMin; }

		float& GetRefValue() { return value; }
		bool GetTypingInputValueFlag()const { return typingInput; }
		/// <summary>
		/// �ǂݍ��܂ꂽ�l���Z�b�g���܂��B�ǂݍ��񂾒l���Z�b�g�������ꍇ��
		/// ���C�u�����̏������������n�܂�O�ɃR���X�g���N�^���Ăяo����Ă��܂��A
		/// �f�[�^���Z�b�g�ł��Ȃ������ꍇ�ɂ��g�����������B
		/// </summary>
		void SetLoadData();
		void SetValue(float value) { this->value = value; }
		void SetTypingInputFlag(const bool flag) { typingInput = flag; }
	};

	

	class GuiVector3
	{
	private:
		MelLib::Vector3 value = 0.0f;
		float valueMin = 0.0f;
		float valueMax = 0.0f;
		MelLib::Vector3 setValue = 0.0f;
		std::string windowName;
		std::string lavel;

		bool typingInput = false;
	public:
		GuiVector3(){}
		/// <summary>
		/// 
		/// </summary>
		/// <param name="value"></param>
		/// <param name="windowName"></param>
		/// <param name="lavel"></param>
		/// <param name="minNumber"></param>
		/// <param name="maxNumber"></param>
		GuiVector3(const MelLib::Vector3& value, const std::string& windowName, const std::string& lavel, float minNumber, float maxNumber);
		~GuiVector3();

		void SetData(const MelLib::Vector3& value, const std::string& windowName, const std::string& lavel, float minNumber, float maxNumber);

		GuiVector3& operator=(const GuiVector3& value)
		{
			this->value = value.GetValue();
			return *this;
		}

		GuiVector3& operator=(const Vector3& value)
		{
			this->value = value;
			return *this;
		}

	
		GuiVector3& operator+=(const GuiVector3& value)
		{
			this->value += value.GetValue();
			return *this;
		}

		

	/*	void operator++() { value++; }
		void operator--() { value--; }*/

		MelLib::Vector3 GetValue()const { return value; }
		MelLib::Vector3& GetRefValue() { return value; }
		float GetMaxValue() const { return valueMax; }
		float GetMinValue()const { return valueMin; }
		bool GetTypingInputValueFlag()const { return typingInput; }
		
		/// <summary>
		/// �ǂݍ��܂ꂽ�l���Z�b�g���܂��B�ǂݍ��񂾒l���Z�b�g�������ꍇ��
		/// ���C�u�����̏������������n�܂�O�ɃR���X�g���N�^���Ăяo����Ă��܂��A
		/// �f�[�^���Z�b�g�ł��Ȃ������ꍇ�ɂ��g�����������B
		/// </summary>
		void SetLoadData();

		void SetValue(const Vector3& value) { this->value = value; }
		void SetTypingInputFlag(const bool flag) { typingInput = flag; }
	};

	/*Vector3 operator+(const GuiVector3& value, const GuiVector3& value2) { return value + value2; }
	Vector3 operator-(const GuiVector3& value, const GuiVector3& value2) { return value - value2; }
	Vector3 operator*(const GuiVector3& value, const GuiVector3& value2) { return value * value2; }
	Vector3 operator/(const GuiVector3& value, const GuiVector3& value2) { return value / value2; }
	Vector3 operator+(const Vector3& value, const GuiVector3& value2) { return value + value2; }
	Vector3 operator-(const Vector3& value, const GuiVector3& value2) { return value - value2; }
	Vector3 operator*(const Vector3& value, const GuiVector3& value2) { return value * value2; }
	Vector3 operator/(const Vector3& value, const GuiVector3& value2) { return value / value2; }*/

	class GuiBool
	{
	private:
		bool value = false;
		bool setValue = false;
		std::string windowName;
		std::string lavel;

	public:
		GuiBool(){}
		/// <summary>
		/// 
		/// </summary>
		/// <param name="value"></param>
		/// <param name="windowName"></param>
		/// <param name="lavel"></param>
		/// <param name="minNumber"></param>
		/// <param name="maxNumber"></param>
		GuiBool(bool value, const std::string& windowName, const std::string& lavel);
		~GuiBool();

		void SetData(bool value, const std::string& windowName, const std::string& lavel);

		GuiBool& operator=(const GuiBool& value)
		{
			this->value = value.GetValue();
			return *this;
		}

		GuiBool& operator=(const bool& value)
		{
			this->value = value;
			return *this;
		}

		

		bool GetValue()const { return value; }
		bool& GetRefValue() { return value; }

		/// <summary>
		/// �ǂݍ��܂ꂽ�l���Z�b�g���܂��B�ǂݍ��񂾒l���Z�b�g�������ꍇ��
		/// ���C�u�����̏������������n�܂�O�ɃR���X�g���N�^���Ăяo����Ă��܂��A
		/// �f�[�^���Z�b�g�ł��Ȃ������ꍇ�ɂ��g�����������B
		/// </summary>
		void SetLoadData();
		void SetValue(bool value) { this->value = value; }
	};
}