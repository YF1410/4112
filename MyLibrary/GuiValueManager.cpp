#include "GuiValueManager.h"
#include"ImguiManager.h"
#include"Random.h"
#include"StringSupport.h"
#include"Input.h"
#include<fstream>
#include<filesystem>

const std::string MelLib::GuiValueManager::DATA_FORMAT = ".guid";
const std::unordered_map<std::string, char>MelLib::GuiValueManager::DATA_FORMAT_STR =
{
	{"int",'i'},
	{"bool",'b'},
	{"Vector3",'3'},
	{"float",'f'},
};

MelLib::GuiValueManager::GuiValueManager()
{
	// �����œǂݍ��ނƊJ���̎��s����
	//Load();
}

void MelLib::GuiValueManager::AddCreateWindowName(const std::string& windowName)
{
	for (const auto& d : drawWindowFlag)
	{
		if (d.first == windowName)return;
	}
	drawWindowFlag.emplace(windowName, true);
	typingInputFlag.emplace(windowName, false);
}


// 2022_10_03
// �Ȃ����ǂݍ��݂������ɑ���������������
// �v�m�F

void MelLib::GuiValueManager::Save(const std::string& windowName, const std::string& lavel, const char*& data, const type_info& type, const size_t dataSize)
{
	// �폜���ꂽGui�̃p�����[�^�͏����o���Ȃ��悤�ɂ���

	// �V�[�N���Ĉꕔ��������������ɂ́A��Ƀt�@�C�����J���Ă��������Ȃ�
	// �J�������Ə����邩��

	// ��U�E�B���h�E���S�������o���ł���������

	// -1�͋�؂�A�I�[��-2
	// ��{�I��0�`127�Ɏ��܂镶�������g��Ȃ����낤����100�ȓ��̗������Z���Ă�-1-2�ɂȂ�Ȃ��Ǝv������-1-2����؂�ɂ��Ă�


	// �����̕����ŉ�����肪�N�����Ė������[�v��������Ă�

	// �����o������
	// ���������Z�������x��
	// ���x���̏I�[����
	// ����΍��p����
	// �ϐ��̌^
	// ���l�f�[�^
	// ��؂�(-1)�܂��͏I�[(-2)

	std::string param;

	// ����΍��p����
	char ran = static_cast<char>(Random::GetRandomNumber(100) + 1);

	std::string addLavel = lavel;
	//for (auto& c : addLavel) c += ran;
	param += addLavel;
	// �I�[�������Z
	param += -1;

	// �����p�������Z
	param += ran;

	if (type == typeid(int))param += DATA_FORMAT_STR.at("int");
	else if (type == typeid(float))param += DATA_FORMAT_STR.at("float");
	else if (type == typeid(bool))param += DATA_FORMAT_STR.at("bool");
	else if (type == typeid(MelLib::Vector3))param += DATA_FORMAT_STR.at("Vector3");

	for (size_t i = 0; i < dataSize; i++)
	{
		param += data[i];
	}

	// ��������
	valueDatas[windowName][lavel] = param;

}

void MelLib::GuiValueManager::Export()
{
	// �l��ۑ�
	for (const auto& window : intValues) 
	{
		const std::string WINDOW_NAME = window.first;
		for (const auto& value : window.second) 
		{
			GuiInt& guiInt = *value.second;
			int num = guiInt.GetValue();
			
			const char* data = reinterpret_cast<char*>(&num);
			Save(WINDOW_NAME, value.first, data, typeid(num), sizeof(num));
		}
	}
	for (const auto& window : floatValues)
	{
		const std::string WINDOW_NAME = window.first;
		for (const auto& value : window.second)
		{
			GuiFloat& guiFloat = *value.second;
			float num = guiFloat.GetValue();

			const char* data = reinterpret_cast<char*>(&num);
			Save(WINDOW_NAME, value.first, data, typeid(num), sizeof(num));
		}
	}
	for (const auto& window : vector3Values)
	{
		const std::string WINDOW_NAME = window.first;
		for (const auto& value : window.second)
		{
			GuiVector3& guiVector3 = *value.second;
			Vector3 num = guiVector3.GetValue();

			const char* data = reinterpret_cast<char*>(&num);
			Save(WINDOW_NAME, value.first, data, typeid(num), sizeof(num));
		}
	}	
	for (const auto& window : boolValues)
	{
		const std::string WINDOW_NAME = window.first;
		for (const auto& value : window.second)
		{
			GuiBool& guiBool = *value.second;
			bool num = guiBool.GetValue();

			const char* data = reinterpret_cast<char*>(&num);
			Save(WINDOW_NAME, value.first, data, typeid(num), sizeof(num));
		}
	}


	for (const auto& datas : valueDatas) 
	{
		const std::string WINDOW_NAME = datas.first;
		const std::string EXPORT_PATH = GuiOption::GetInstance()->GetGuiDataPath() + WINDOW_NAME + DATA_FORMAT;

		// �����o��
		std::ofstream file(EXPORT_PATH);

		int loopNum = 0;
		for (const auto& d : datas.second)
		{
			file.write(d.second.c_str(), d.second.size());


			if (loopNum != datas.second.size() - 1)
			{
				char kugiri = -1;
				file.write(&kugiri, 1);
			}
			else
			{
				char kugiri = -2;
				file.write(&kugiri, 1);
			}
			loopNum++;
		}

		file.close();
	}
}

void MelLib::GuiValueManager::Load()
{

	std::string importPath = GuiOption::GetInstance()->GetGuiDataPath();

	if (importPath.size() == 0)importPath = std::filesystem::current_path().string();

	for (const auto& p : std::filesystem::directory_iterator(importPath))
	{
		std::string filePath = p.path().string();
		if (filePath.find(DATA_FORMAT) != std::string::npos)
		{
			std::string fileName = StringSupport::ExtractionFileName(filePath);

			fileName.erase(fileName.begin() + (fileName.size() - DATA_FORMAT.size()), fileName.end());

			std::ifstream file(filePath);

			while (1)
			{

				// ��؂�܂Ń��[�v���Ċi�[
				std::string lavel;
				char c = 0;
				while (1)
				{
					file.read(&c, 1);
					if (c == -1)break;
					lavel += c;
				}

				std::string param = lavel;
				param += -1;

				//// �ݒ肵���������擾
				//char randNum = 0;
				//file.read(&randNum, 1);
				//// ���Z���Ă����Ƃ������O�ɖ߂�
				//for (auto& c : lavel)c -= randNum;
				//param += randNum;


				// �^�̎擾
				char formatChar = 0;
				file.read(&formatChar, 1);
				param += formatChar;

				if (formatChar == DATA_FORMAT_STR.at("int")
					|| formatChar == DATA_FORMAT_STR.at("float"))
				{
					char value[4];
					file.read(value, 4);

					for (int i = 0; i < 4; i++)param += value[i];
				}
				else if (formatChar == DATA_FORMAT_STR.at("bool"))
				{
					char value;
					file.read(&value, 1);
					param += value;
				}
				else if (formatChar == DATA_FORMAT_STR.at("Vector3"))
				{
					char value[sizeof(Vector3)];
					file.read(value, sizeof(Vector3));
					for (int i = 0; i < sizeof(Vector3); i++)param += value[i];
				}

				// �����o���̎��̂��߂ɓǂݍ��񂾂��̂��i�[���ĕۑ����Ă���
				valueDatas[fileName][lavel] = param;

				// ��؂��-1���Ȃ������甲����
				char kugiri = 0;
				file.read(&kugiri, 1);
				if (kugiri == -2)break;
			}

			file.close();
		}
	}

}

void MelLib::GuiValueManager::AllSetLoadData()
{
	//int
	for (auto& data : intValues) 
	{
		for (auto& value : data.second) 
		{
			value.second->SetLoadData();
		}
	}

	for (auto& data : floatValues)
	{
		for (auto& value : data.second)
		{
			value.second->SetLoadData();
		}
	}

	for (auto& data : vector3Values)
	{
		for (auto& value : data.second)
		{
			value.second->SetLoadData();
		}
	}

	for (auto& data : boolValues)
	{
		for (auto& value : data.second)
		{
			value.second->SetLoadData();
		}
	}
}


MelLib::GuiValueManager* MelLib::GuiValueManager::GetInstance()
{
	static GuiValueManager m;
	return &m;
}

void MelLib::GuiValueManager::AddGuiValue(GuiInt* pGuiValue, const std::string& windowName, const std::string& lavel)
{
	intValues[windowName].try_emplace(lavel, pGuiValue);
	AddCreateWindowName(windowName);

	addOrders[windowName].push_back(lavel);
}

void MelLib::GuiValueManager::AddGuiValue(GuiFloat* pGuiValue, const std::string& windowName, const std::string& lavel)
{
	floatValues[windowName].try_emplace(lavel, pGuiValue);
	AddCreateWindowName(windowName);
	addOrders[windowName].push_back(lavel);
}

void MelLib::GuiValueManager::AddGuiValue(GuiVector3* pGuiValue, const std::string& windowName, const std::string& lavel)
{
	vector3Values[windowName].try_emplace(lavel, pGuiValue);
	AddCreateWindowName(windowName);
	addOrders[windowName].push_back(lavel);
}

void MelLib::GuiValueManager::AddGuiValue(GuiBool* pGuiValue, const std::string& windowName, const std::string& lavel)
{
	boolValues[windowName].try_emplace(lavel, pGuiValue);
	AddCreateWindowName(windowName);
	addOrders[windowName].push_back(lavel);
}

void MelLib::GuiValueManager::EraseGuiValue(const type_info& type, const std::string& windowName, const std::string& lavel)
{
	if (type == typeid(int))
	{
		intValues[windowName].erase(lavel);
		if (intValues[windowName].size() == 0)intValues.erase(windowName);
	}
	else if (type == typeid(float))
	{
		floatValues[windowName].erase(lavel);
		if (floatValues[windowName].size() == 0)floatValues.erase(windowName);
	}
	else if (type == typeid(MelLib::Vector3))
	{
		vector3Values[windowName].erase(lavel);
		if (vector3Values[windowName].size() == 0)vector3Values.erase(windowName);
	}
	else if (type == typeid(bool))
	{
		boolValues[windowName].erase(lavel);
		if (boolValues[windowName].size() == 0)boolValues.erase(windowName);
	}
}

void MelLib::GuiValueManager::DeleteWindow(const std::string& windowName)
{
	intValues.erase(windowName);
	floatValues.erase(windowName);
	vector3Values.erase(windowName);
	boolValues.erase(windowName);

	drawWindowFlag.erase(windowName);
	typingInputFlag.erase(windowName);
	addOrders.erase(windowName);
}

void MelLib::GuiValueManager::Initialize()
{
	Load();
}

void MelLib::GuiValueManager::Update()
{
	// �����[�X���ɕ`�悵�Ȃ��ݒ肾������return
	if (!ImguiManager::GetInstance()->GetReleaseDrawFrag())return;

	// �z�񌩂āAWindow�����ꏏ��������1�̃E�B���h�E�ɂ܂Ƃ߂�
	for (const auto& d : drawWindowFlag)
	{
		const std::string WINDOW_NAME = d.first;

		// �`��t���O��false�������玟��
		if (!drawWindowFlag[WINDOW_NAME])continue;

		ImguiManager::GetInstance()->BeginDrawWindow(WINDOW_NAME);

		// �؂�ւ�������ǂ���
		bool changeFlag = false;

		// lavel�ꗗ
		// ���̔z�񏇂ɃE�B���h�E�ɒǉ����Ă���
		std::vector<std::string>lavels(addOrders[WINDOW_NAME].size());

		bool lavelCheckEnd = false;


		for (const auto& lavel : addOrders[WINDOW_NAME])
		{
			lavelCheckEnd = false;

			// �f�[�^�̔z�񂩂�T���o���A��������ǉ��ƕۑ�����

			// int
			const std::unordered_map<std::string, GuiInt*>& refInts = intValues[WINDOW_NAME];
			for (auto& value : refInts)
			{
				const std::string& LAVEL = value.first;

				// ���x�����������������珈��
				if (lavel == LAVEL)
				{
					lavelCheckEnd = true;

					GuiInt& guiInt = *value.second;
					int num = guiInt.GetValue();
					//ImGuiInputTextFlags_CharsDecimal
					if (guiInt.GetTypingInputValueFlag())
					{
						changeFlag = ImguiManager::GetInstance()->DrawInputIntBox(LAVEL, num, guiInt.GetMinValue(), guiInt.GetMaxValue());
					}
					else changeFlag = ImguiManager::GetInstance()->DrawSliderInt(LAVEL, num, guiInt.GetMinValue(), guiInt.GetMaxValue());
					guiInt = num;
				/*	if (changeFlag)
					{
						const char* data = reinterpret_cast<char*>(&num);
						Save(WINDOW_NAME, LAVEL, data, typeid(num), sizeof(num), changeFlag);
						guiInt = num;
					}*/


				}

			}

			if (lavelCheckEnd)continue;

			// float 
			const std::unordered_map<std::string, GuiFloat*>& refFloats = floatValues[WINDOW_NAME];
			for (auto& value : refFloats)
			{
				const std::string& LAVEL = value.first;

				// ���x�����������������珈��
				if (lavel == LAVEL)
				{
					lavelCheckEnd = true;

					GuiFloat& guiFloat = *value.second;
					float num = guiFloat.GetValue();
					
					
					if (guiFloat.GetTypingInputValueFlag()) changeFlag = ImguiManager::GetInstance()->DrawInputFloatBox(LAVEL, num, guiFloat.GetMinValue(), guiFloat.GetMaxValue());
					else changeFlag = changeFlag = ImguiManager::GetInstance()->DrawSliderFloat(LAVEL, num, guiFloat.GetMinValue(), guiFloat.GetMaxValue());
					
					guiFloat = num;
					/*if (changeFlag)
					{
						const char* data = reinterpret_cast<char*>(&num);
						Save(WINDOW_NAME, LAVEL, data, typeid(num), sizeof(num), changeFlag);
						guiFloat = num;
					}*/
				}

			}


			if (lavelCheckEnd)continue;

			// Vector3 
			const std::unordered_map<std::string, GuiVector3*>& refVector3s = vector3Values[WINDOW_NAME];
			for (auto& value : refVector3s)
			{
				const std::string& LAVEL = value.first;

				// ���x�����������������珈��
				if (lavel == LAVEL)
				{
					lavelCheckEnd = true;

					GuiVector3& guiVector3 = *value.second;
					Vector3 num = guiVector3.GetValue();
					

					if (guiVector3.GetTypingInputValueFlag()) changeFlag = ImguiManager::GetInstance()->DrawInputVector3Box(LAVEL, num);
					else changeFlag = ImguiManager::GetInstance()->DrawSliderVector3(LAVEL, num, guiVector3.GetMinValue(), guiVector3.GetMaxValue());
					guiVector3 = num;

					/*if (changeFlag)
					{
						const char* data = reinterpret_cast<char*>(&num);
						Save(WINDOW_NAME, LAVEL, data, typeid(num), sizeof(num), changeFlag);
						guiVector3 = num;
					}*/
				}

			}

			if (lavelCheckEnd)continue;

			// bool 
			const std::unordered_map<std::string, GuiBool*>& refBools = boolValues[WINDOW_NAME];
			for (auto& value : refBools)
			{
				const std::string& LAVEL = value.first;

				// ���x�����������������珈��
				if (lavel == LAVEL)
				{
					lavelCheckEnd = true;

					GuiBool& guiBool = *value.second;
					bool flag = guiBool.GetValue();
					changeFlag = ImguiManager::GetInstance()->DrawCheckBox(LAVEL, flag);
					guiBool = flag;
					/*if (changeFlag)
					{
						const char* data = reinterpret_cast<char*>(&flag);
						Save(WINDOW_NAME, LAVEL, data, typeid(flag), sizeof(flag), changeFlag);
						guiBool = flag;
					}*/
				}

			}
		}

		ImguiManager::GetInstance()->EndDrawWindow();
	}


	bool pushCtrl = Input::KeyState(DIK_LCONTROL) || Input::KeyState(DIK_RCONTROL);
	// �ۑ�
	if (pushCtrl && Input::KeyTrigger(DIK_S))Export();

	//// �Ǎ�
	//if (pushCtrl && Input::KeyTrigger(DIK_L))
	//{
	//	Load();
	//	AllSetLoadData();
	//}
#pragma region ��


	//// int
	//const std::unordered_map<std::string, GuiInt*>& refInts = intValues[windowName];
	//for (auto& value : refInts) 
	//{
	//	const std::string& LAVEL = value.first;
	//	GuiInt& guiInt = *value.second;
	//	int num = guiInt.GetValue();
	//	changeFlag = ImguiManager::GetInstance()->DrawSliderInt(LAVEL, num, guiInt.GetMinValue(), guiInt.GetMaxValue());
	//	
	//	if (changeFlag) 
	//	{
	//		const char* data = reinterpret_cast<char*>(&num);
	//		Save(windowName, LAVEL, data, typeid(num),sizeof(num), changeFlag);
	//		guiInt = num;
	//	}
	//}

	//// float 
	//const std::unordered_map<std::string, GuiFloat*>& refFloats = floatValues[windowName];
	//for (auto& value : refFloats)
	//{
	//	const std::string& LAVEL = value.first;
	//	GuiFloat& guiFloat = *value.second;
	//	float num = guiFloat.GetValue();
	//	changeFlag = ImguiManager::GetInstance()->DrawSliderFloat(LAVEL, num, guiFloat.GetMinValue(), guiFloat.GetMaxValue());
	//	if (changeFlag)
	//	{
	//		const char* data = reinterpret_cast<char*>(&num);
	//		Save(windowName, LAVEL, data, typeid(num), sizeof(num), changeFlag);
	//		guiFloat = num;
	//	}
	//}

	//// Vector3
	//const std::unordered_map<std::string, GuiVector3*>& refVector3 = vector3Values[windowName];
	//for (auto& value : refVector3)
	//{
	//	const std::string& LAVEL = value.first;
	//	GuiVector3& guiVector3 = *value.second;
	//	MelLib::Vector3 num = guiVector3.GetValue();
	//	changeFlag = ImguiManager::GetInstance()->DrawSliderVector3(LAVEL, num, guiVector3.GetMinValue(), guiVector3.GetMaxValue());
	//	if (changeFlag)
	//	{
	//		const char* data = reinterpret_cast<char*>(&num);
	//		Save(windowName, LAVEL, data, typeid(num), sizeof(num), changeFlag);
	//		guiVector3 = num;
	//	}
	//}

	//// bool
	//const std::unordered_map<std::string, GuiBool*>& refBool = boolValues[windowName];
	//for (auto& value : refBool)
	//{
	//	const std::string& LAVEL = value.first;
	//	GuiBool& guiBool = *value.second;
	//	bool flag = guiBool.GetValue();
	//	changeFlag = ImguiManager::GetInstance()->DrawCheckBox(LAVEL, flag);
	//	if (changeFlag)
	//	{
	//		const char* data = reinterpret_cast<char*>(&flag);
	//		Save(windowName, LAVEL, data, typeid(flag), sizeof(flag), changeFlag);
	//		guiBool = flag;
	//	}
	//}

	//ImguiManager::GetInstance()->EndDrawWindow();

#pragma endregion
}

//void MelLib::GuiValueManager::CopyGuiValue(const GameObject& object, const GameObject& object2)
//{
//	 �o�^����Ƃ���GameObject��GuiValue�̒l�̃|�C���^�ƃE�B���h�E�ƃ��x������ۑ�����
//	 ��̏����Q�l��2�ɃR�s�[����
//
//
//}

void MelLib::GuiValueManager::CopyGuiValue(const std::string& windowName1, const std::string& windowName2)
{
	// int
	for (const auto& gui1 : intValues[windowName1])
	{
		const std::string LAVEL1 = gui1.first;

		for (auto& gui2 : intValues[windowName2]) 
		{
			const std::string LAVEL2 = gui2.first;

			if (LAVEL1 == LAVEL2) 
			{
				gui2.second->SetValue(gui1.second->GetValue());
			}
		}
	}

	// float
	for (const auto& gui1 : floatValues[windowName1])
	{
		const std::string LAVEL1 = gui1.first;

		for (auto& gui2 : floatValues[windowName2])
		{
			const std::string LAVEL2 = gui2.first;

			if (LAVEL1 == LAVEL2)
			{
				gui2.second->SetValue(gui1.second->GetValue());
			}
		}
	}

	// Vector3
	for (const auto& gui1 : vector3Values[windowName1])
	{
		const std::string LAVEL1 = gui1.first;

		for (auto& gui2 : vector3Values[windowName2])
		{
			const std::string LAVEL2 = gui2.first;

			if (LAVEL1 == LAVEL2)
			{
				gui2.second->SetValue(gui1.second->GetValue());
			}
		}
	}

	// bool
	for (const auto& gui1 : boolValues[windowName1])
	{
		const std::string LAVEL1 = gui1.first;

		for (auto& gui2 : boolValues[windowName2])
		{
			const std::string LAVEL2 = gui2.first;

			if (LAVEL1 == LAVEL2)
			{
				gui2.second->SetValue(gui1.second->GetValue());
			}
		}
	}
}

void MelLib::GuiValueManager::ChangeWindowName(const std::string& windowName, const std::string& newWindowName)
{
	for (const auto& window : valueDatas) 
	{
		const std::string WINDOW_NAME = window.first;
		if (windowName != WINDOW_NAME)continue;

		std::unordered_map<std::string, std::string> newMap = valueDatas.at(WINDOW_NAME);

		valueDatas.erase(WINDOW_NAME);
		valueDatas.emplace(newWindowName, newMap);

		break;
	}

	for (const auto& window : addOrders)
	{
		const std::string WINDOW_NAME = window.first;
		if (windowName != WINDOW_NAME)continue;

		std::vector<std::string> newVector = addOrders.at(WINDOW_NAME);

		addOrders.erase(WINDOW_NAME);
		addOrders.emplace(newWindowName, newVector);

		break;
	}

	for (const auto& window : drawWindowFlag)
	{
		const std::string WINDOW_NAME = window.first;
		if (windowName != WINDOW_NAME)continue;

		bool newFlag = drawWindowFlag.at(WINDOW_NAME);

		drawWindowFlag.erase(WINDOW_NAME);
		drawWindowFlag.emplace(newWindowName, newFlag);

		break;
	}
	for (const auto& window : typingInputFlag)
	{
		const std::string WINDOW_NAME = window.first;
		if (windowName != WINDOW_NAME)continue;

		bool newFlag = typingInputFlag.at(WINDOW_NAME);

		typingInputFlag.erase(WINDOW_NAME);
		typingInputFlag.emplace(newWindowName, newFlag);

		break;
	}

	

	// int
	for (const auto& window : intValues)
	{
		const std::string WINDOW_NAME = window.first;
		if (windowName != WINDOW_NAME)continue;

		std::unordered_map<std::string, GuiInt*> newMap = intValues.at(WINDOW_NAME);

		intValues.erase(WINDOW_NAME);
		intValues.emplace(newWindowName, newMap);

		break;
	}

	for (const auto& window : floatValues)
	{
		const std::string WINDOW_NAME = window.first;
		if (windowName != WINDOW_NAME)continue;

		std::unordered_map<std::string, GuiFloat*> newMap = floatValues.at(WINDOW_NAME);

		floatValues.erase(WINDOW_NAME);
		floatValues.emplace(newWindowName, newMap);
		break;
	}

	// Vector3
	for (const auto& window : vector3Values)
	{
		const std::string WINDOW_NAME = window.first;
		if (windowName != WINDOW_NAME)continue;

		std::unordered_map<std::string, GuiVector3*> newMap = vector3Values.at(WINDOW_NAME);

		vector3Values.erase(WINDOW_NAME);
		vector3Values.emplace(newWindowName, newMap);
		break;
	}

	// bool
	for (const auto& window : boolValues)
	{
		const std::string WINDOW_NAME = window.first;
		if (windowName != WINDOW_NAME)continue;

		std::unordered_map<std::string, GuiBool*> newMap = boolValues.at(WINDOW_NAME);

		boolValues.erase(WINDOW_NAME);
		boolValues.emplace(newWindowName, newMap);
		break;
	}
}

void MelLib::GuiValueManager::LoadGUIFileData(const std::string& windowName)
{

	for (auto& guiValue : intValues[windowName])
	{
		int value = 0;
		GetGuiData(guiValue.second,value,windowName,guiValue.first);
		guiValue.second->SetValue(value);
	}

	for (auto& guiValue : floatValues[windowName])
	{
		float value = 0;
		GetGuiData(guiValue.second, value, windowName, guiValue.first);
		guiValue.second->SetValue(value);
	}
	for (auto& guiValue : vector3Values[windowName])
	{
		Vector3 value = 0;
		GetGuiData(guiValue.second, value, windowName, guiValue.first);
		guiValue.second->SetValue(value);
	}
	for (auto& guiValue : boolValues[windowName])
	{
		bool value = false;
		GetGuiData(guiValue.second, value, windowName, guiValue.first);
		guiValue.second->SetValue(value);
	}
}

void MelLib::GuiValueManager::SetTypingInputFlag(const std::string& windowName, bool flag)
{
	for(const auto& value : intValues[windowName])
	{
		value.second->SetTypingInputFlag(flag);
	}
	for (const auto& value : floatValues[windowName])
	{
		value.second->SetTypingInputFlag(flag);
	}
	for (const auto& value : vector3Values[windowName])
	{
		value.second->SetTypingInputFlag(flag);
	}
	typingInputFlag[windowName] = flag;
}

void MelLib::GuiValueManager::ChangeTypingInputFlag(const std::string& windowName)
{
	for (const auto& value : intValues[windowName])
	{
		value.second->SetTypingInputFlag(!value.second->GetTypingInputValueFlag());
	}
	for (const auto& value : floatValues[windowName])
	{
		value.second->SetTypingInputFlag(!value.second->GetTypingInputValueFlag());
	}
	for (const auto& value : vector3Values[windowName])
	{
		value.second->SetTypingInputFlag(!value.second->GetTypingInputValueFlag());
	}
	typingInputFlag[windowName] = !typingInputFlag.at(windowName);
}

bool MelLib::GuiValueManager::GetTypingInputFlag(const std::string& windowName) const
{
	return typingInputFlag.at(windowName);
}



bool MelLib::GuiValueManager::GetGuiData(GuiInt* pGuiValue, int& refInt, const std::string& windowName, const std::string& lavel)
{
	// ���݂��邩�m�F�B�Ȃ�������return
	if (valueDatas.find(windowName) == valueDatas.end())return false;
	if (valueDatas.at(windowName).find(lavel) == valueDatas.at(windowName).end())return false;

	// ��������i�[
	std::string param = valueDatas.at(windowName).at(lavel);
	char data[4];
	for (int i = 0; i < 4; i++)data[i] = param[param.size() - 4 + i];

	int* pValue = reinterpret_cast<int*>(data);
	refInt = *pValue;

	// ���ɒǉ�����Ă���l�����������ďI��
	// ��ŏ��������Ă邩�炱�̂܂�return
	
		// intValue�ɒǉ�����Ă��Ă��邩�m�F
	if (intValues[windowName].find(lavel) != intValues[windowName].end())
	{
		// �ǉ�����ĂȂ������m�F
		if (valueDatas[windowName].find(lavel) == valueDatas[windowName].end())
		{
			return false;
		}
		return true;
	}
	
	intValues[windowName].emplace(lavel, pGuiValue);
	AddCreateWindowName(windowName);

	addOrders[windowName].push_back(lavel);

	return true;
}

bool MelLib::GuiValueManager::GetGuiData(GuiFloat* pGuiValue, float& refFloat, const std::string& windowName, const std::string& lavel)
{
	// ���݂��邩�m�F
	if (valueDatas.find(windowName) == valueDatas.end())return false;
	if (valueDatas.at(windowName).find(lavel) == valueDatas.at(windowName).end())return false;

	// ��������i�[
	std::string param = valueDatas.at(windowName).at(lavel);
	char data[4];
	for (int i = 0; i < 4; i++)data[i] = param[param.size() - 4 + i];


	float* pValue = reinterpret_cast<float*>(data);
	refFloat = *pValue;
	
	if (floatValues[windowName].find(lavel) != floatValues[windowName].end())
	{
		// �ǉ�����ĂȂ������m�F
		if (valueDatas[windowName].find(lavel) == valueDatas[windowName].end())
		{
			return false;
		}
		return true;
	}
	
	floatValues[windowName].emplace(lavel, pGuiValue);
	AddCreateWindowName(windowName);
	addOrders[windowName].push_back(lavel);
	return true;
}

bool MelLib::GuiValueManager::GetGuiData(GuiBool* pGuiValue, bool& refFlag, const std::string& windowName, const std::string& lavel)
{
	// ���݂��邩�m�F
	if (valueDatas.find(windowName) == valueDatas.end())return false;
	if (valueDatas.at(windowName).find(lavel) == valueDatas.at(windowName).end())return false;

	// ��������i�[
	std::string param = valueDatas.at(windowName).at(lavel);
	char flag = param[param.size() - 1];
	refFlag = static_cast<bool>(flag);
	
	if (boolValues[windowName].find(lavel) != boolValues[windowName].end())
	{
		// �ǉ�����ĂȂ������m�F
		if (valueDatas[windowName].find(lavel) == valueDatas[windowName].end())
		{
			return false;
		}
		return true;
	}
	
	boolValues[windowName].emplace(lavel, pGuiValue);
	AddCreateWindowName(windowName);
	addOrders[windowName].push_back(lavel);
	return true;
}

bool MelLib::GuiValueManager::GetGuiData(GuiVector3* pGuiValue, Vector3& refVectior3, const std::string& windowName, const std::string& lavel)
{
	// ���݂��邩�m�F
	if (valueDatas.find(windowName) == valueDatas.end())return false;
	if (valueDatas.at(windowName).find(lavel) == valueDatas.at(windowName).end())return false;

	// ��������i�[
	std::string param = valueDatas.at(windowName).at(lavel);
	char data[sizeof(Vector3)];
	for (int i = 0; i < sizeof(Vector3); i++)data[i] = param[param.size() - sizeof(Vector3) + i];

	Vector3* pValue = reinterpret_cast<Vector3*>(data);
	refVectior3 = *pValue;

	


	if (vector3Values[windowName].find(lavel) != vector3Values[windowName].end())
	{
		// �ǉ�����ĂȂ������m�F
		if (valueDatas[windowName].find(lavel) == valueDatas[windowName].end())
		{
			return false;
		}
		return true;
	}
	

	vector3Values[windowName].emplace(lavel, pGuiValue);
	AddCreateWindowName(windowName);
	addOrders[windowName].push_back(lavel);
	return true;
}

