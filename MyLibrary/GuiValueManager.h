#pragma once
#include<string>
#include<map>
#include<unordered_map>
#include<vector>
#include<typeinfo>
#include"GameObject.h"

#include"GuiValue.h"


// valueDatas���e�f�[�^��map�ǂ��炩�����ł���
// valueDatas�Ɗe�f�[�^�̒��g�͕ʕ������������

// valueData����Ȃ�
// �e�f�[�^����l���o���ď������߂΂���

namespace MelLib
{
	// .guid(GUI Data) �d�l
	// �t�@�C�����̓E�B���h�E��
	// 
	// ����(���x��������)
	// ���x��(HP�Ȃǂ̃p�����[�^�[��)
	// ��؂�(char�^��-1�B������ł͂Ȃ�)
	// ���O�����p����(2023_04_19��U�p�~)
	// �l�̌^
	// �l�f�[�^
	// �I�����ʒl(-1���܂������B-2������)


	class GuiValueManager
	{
	private:

		GuiValueManager();
		~GuiValueManager() {}

		// createWindowNames�ɓn���ꂽ���O���Ȃ������ꍇ�ǉ�
		void AddCreateWindowName(const std::string& windowName);


		/// <summary>
		/// 
		/// </summary>
		/// <param name="windowName">�E�B���h�E��</param>
		/// <param name="lavel">���x��</param>
		/// <param name="data">char*�ɕϊ������f�[�^�̃|�C���^�̎Q��</param>
		/// <param name="dataSize">�f�[�^�T�C�Y</param>
		void Save(const std::string& windowName, const std::string& lavel, const char*& data, const type_info& type,size_t dataSize);
		void Export();

		void Load();
		void AllSetLoadData();
	private:

		static const std::string DATA_FORMAT;
		static const std::unordered_map<std::string, char>DATA_FORMAT_STR;



		// std::map < Window��, std::map<���x����, �f�[�^(.guid�̒��g�S��)>>
		// �f�[�^
		std::unordered_map < std::string, std::unordered_map<std::string, std::string>>valueDatas;
		// �ǉ���������
		std::unordered_map < std::string, std::vector<std::string>>addOrders;
		//[�I�u�W�F�N�g��][���x����]
		std::unordered_map<std::string, std::unordered_map<std::string, GuiInt*>>intValues;
		std::unordered_map<std::string, std::unordered_map<std::string, GuiFloat*>>floatValues;
		std::unordered_map<std::string, std::unordered_map<std::string, GuiVector3*>>vector3Values;
		std::unordered_map<std::string, std::unordered_map<std::string, GuiBool*>>boolValues;

		// ���������E�B���h�E�ƕ`��t���O���m�F���邽�߂̃t���O
		std::unordered_map < std::string, bool >drawWindowFlag;
		std::unordered_map < std::string, bool >typingInputFlag;

		int* castIntData(const std::string& data);
		float* castFloatData(const std::string& data);
		Vector3* castVector3Data(const std::string& data);
		bool* castBoolData(const std::string& data);



	public:
		GuiValueManager(GuiValueManager& m) = delete;
		GuiValueManager& operator=(GuiValueManager& m) = delete;
		static GuiValueManager* GetInstance();

		void AddGuiValue(GuiInt* pGuiValue, const std::string& windowName,const std::string& lavel);
		void AddGuiValue(GuiFloat* pGuiValue, const std::string& windowName,const std::string& lavel);
		void AddGuiValue(GuiVector3* pGuiValue, const std::string& windowName,const std::string& lavel);
		void AddGuiValue(GuiBool* pGuiValue, const std::string& windowName,const std::string& lavel);

		void EraseGuiValue(const type_info& type,const std::string& windowName, const std::string& lavel);
		void DeleteWindow(const std::string& windowName);

		void Initialize();
		void Update();

		/// <summary>
		/// Gui�̒lobject����object2�ɃR�s�[���܂��B
		/// </summary>
		/// <param name="object"></param>
		//void CopyGuiValue(const GameObject& object,const GameObject& object2);
		
		/// <summary>
		/// �E�B���h�E1�̒l���E�B���h�E2�̃��x������v����l�ɃR�s�[���܂��B
		/// </summary>
		/// <param name="windowName1"></param>
		/// <param name="windowName2"></param>
		void CopyGuiValue(const std::string& windowName1, const std::string& windowName2);

		/// <summary>
		/// �E�B���h�E����ύX���܂��B
		/// </summary>
		/// <param name="windowName"></param>
		void ChangeWindowName(const std::string& windowName,const std::string& newWindowName);

		/// <summary>
		/// �ۑ�����Ă���GUI�̃f�[�^��ǂݍ��݂܂��B
		/// </summary>
		/// <param name="windowName"></param>
		void LoadGUIFileData(const std::string& windowName);

		void SetTypingInputFlag(const std::string& windowName,bool flag);
		void ChangeTypingInputFlag(const std::string& windowName);
		bool GetTypingInputFlag(const std::string& windowName)const;

		bool GetGuiData(GuiInt* pGuiValue, int& refInt, const std::string& windowName, const std::string& lavel);
		bool GetGuiData(GuiFloat* pGuiValue, float& refFloat,const std::string& windowName, const std::string& lavel);
		bool GetGuiData(GuiBool* pGuiValue, bool& refFlag, const std::string& windowName, const std::string& lavel);
		bool GetGuiData(GuiVector3* pGuiValue, Vector3& refVectior3,const std::string& windowName, const std::string& lavel);

		void SetDrawWindowFlag(const std::string& windowName, bool flag) { drawWindowFlag[windowName] = flag; }
		void ChangeDrawWindowFlag(const std::string& windowName) { drawWindowFlag[windowName] = !drawWindowFlag[windowName]; }

		

	};

}