#include "SceneEditer.h"

#include"ImguiManager.h"
#include"GameObjectManager.h"
#include"SceneManager.h"
#include"GuiValueManager.h"
#include"Input.h"
#include"LibMath.h"

#include<fstream>
#include<filesystem>

const std::string MelLib::SceneEditer::EDIT_DATA_FORMAT = ".melsce";
const std::string MelLib::SceneEditer::TEST_START_EDIT_DATA_NAME = "TestStartEditData";
const std::string MelLib::SceneEditer::REGISTER_OBJECT_DATA_FORMAT = ".rod";
const std::string MelLib::SceneEditer::EDIT_WINDOW_NAME = "Edit";
const std::string MelLib::SceneEditer::CAMERA_WINDOW_NAME = "EditCamera";

void MelLib::SceneEditer::StartSave()
{

	if (!editorFlag || !ReleaseCheck())return;


	SaveRegisterObject();
	inpttingEditDataName = true;
	// 2022_05_01
	// �I�u�W�F�N�g�}�l�[�W���[�ɒǉ������I�u�W�F�N�g��imgui�ł������悤��
	

	//std::string name = SceneManager::GetInstance()->GetCurrentSceneName();
	//name += EDIT_DATA_FORMAT;
	//std::ofstream file(name);

	//// �����o��


	//file.close();
}

void MelLib::SceneEditer::SaveEditData(const std::string& dataName)
{
	if (!editorFlag || !ReleaseCheck())return;
	std::ofstream file(dataName + EDIT_DATA_FORMAT, std::ios_base::binary);

	const size_t ADD_OBJECT_SIZE = addObjects.size();
	if(ADD_OBJECT_SIZE == 0)file.write("0", 1);
	else file.write("1", 1);

	// �����o��
	for (size_t i = 0; i < ADD_OBJECT_SIZE ; i++)
	{
		GameObject* pObject = addObjects[i].get();

		// �N���X��
		const std::string CLASS_NAME = typeid(*pObject).name();
		file.write(CLASS_NAME.c_str(), CLASS_NAME.size());
		file.write("\0", 1);

		// �I�u�W�F�N�g��
		const std::string OBJECT_NAME = pObject->GetObjectName();
		file.write(OBJECT_NAME.c_str(), OBJECT_NAME.size());
		file.write("\0", 1);

		// �o�^��
		file.write(addObjectNames[i].c_str(), addObjectNames[i].size());
		file.write("\0", 1);
		

		//���W�Ƃ�
		Vector3 position = pObject->GetPosition();
		file.write(reinterpret_cast<char*>(&position), sizeof(Vector3));
		Vector3 angle = pObject->GetAngle();
		file.write(reinterpret_cast<char*>(&angle), sizeof(Vector3));
		Vector3 scale = pObject->GetScale();
		file.write(reinterpret_cast<char*>(&scale), sizeof(Vector3));

		char c = -1;
		if (i == ADD_OBJECT_SIZE - 1)c = -2;
		file.write(&c, 1);
	}

	file.close();
}

void MelLib::SceneEditer::InputEditDataName()
{
	if (!editorFlag || !ReleaseCheck())return;
	//ImguiManager::GetInstance()->DrawTextBox("Input Object Name", inputObjectName, 20);
	std::string s = "EditData";
	char c[21];
	for (int i = 0; i < 21; i++)
	{
		if (i < s.size())c[i] = s[i];
		else c[i] = ' ';
	}
	c[20] = '\0';


	ImGui::Text("Input Edit Data Name");
	ImGui::InputText("", c, 21);



	if (Input::KeyTrigger(DIK_RETURN))
	{
		std::string inputEditDataName = c;
		inpttingEditDataName = false;
		SaveEditData(inputEditDataName);
	}
}

void MelLib::SceneEditer::SaveRegisterObject()
{
	// �����o������
	// �I�u�W�F�N�g��(�t�@�C����)
	// �N���X��
	// �p�����[�^�[
	if (!editorFlag || !ReleaseCheck())return;

	for (const auto& o : pRegisterObjects) 
	{
		for (const auto& o2 : o.second) 
		{
			GameObject* pObject = o2.second.get();
			
			const std::string OBJECT_NAME = pObject->GetObjectName();

			// �I�������I�u�W�F�N�g����Ȃ�������ۑ����Ȃ�
			bool selectObject = false;
			for (const auto& name : registerSelectObjectNames) 
			{
				if (name == OBJECT_NAME) 
				{
					selectObject = true;
					break;
				}
			}
			if (!selectObject)break;


			std::ofstream registerObjectFile(OBJECT_NAME + REGISTER_OBJECT_DATA_FORMAT,std::ios_base::binary);
			
			registerObjectFile.write(o.first.c_str() , o.first.size());
			registerObjectFile.write("\0" , 1);

			std::string className = typeid(*pObject).name();
			registerObjectFile.write(className.c_str(), className.size());
			registerObjectFile.write("\0", 1);

			// ���W�͏����o���K�v�Ȃ�
			Vector3 angle = pObject->GetAngle();
			registerObjectFile.write(reinterpret_cast<char*>(&angle), sizeof(Vector3));
			Vector3 scale = pObject->GetScale();
			registerObjectFile.write(reinterpret_cast<char*>(&scale), sizeof(Vector3));

			registerObjectFile.close();
		}
	}

}


void MelLib::SceneEditer::Load()
{
	if (!editorFlag || !ReleaseCheck())return;
	// �V�[�������i�[
	for (const auto& dirEntry : std::filesystem::directory_iterator("."))
	{
		std::string name = dirEntry.path().string();
		if (name.find(EDIT_DATA_FORMAT) == std::string::npos)continue;
		name.erase(name.begin(), name.begin() + 2);
		name.erase(name.end() - EDIT_DATA_FORMAT.size(), name.end());
		
		if (name == TEST_START_EDIT_DATA_NAME)continue;
		sceneFileNames.push_back(name);
	}

	if (sceneFileNames.size() == 0)return;

	selectingEditData = true;
}


void MelLib::SceneEditer::LoadRegisterSelectObject()
{

	if (!editorFlag || !ReleaseCheck())return;
	for (const auto& dirEntry : std::filesystem::directory_iterator("."))
	{
		const std::string FILE_NAME = dirEntry.path().string();
		if (FILE_NAME.find(REGISTER_OBJECT_DATA_FORMAT) == std::string::npos)continue;

		std::ifstream file(FILE_NAME, std::ios_base::binary);

		ObjectData data;
		data.objectName = FILE_NAME;
		data.objectName.erase(data.objectName.begin(), data.objectName.begin() + 2);
		data.objectName.erase(data.objectName.end() - REGISTER_OBJECT_DATA_FORMAT.size(), data.objectName.end());

		//for(size_t i = objectName.size() - 1 ;;)

		LoadFileName(file, data.typeName);
		LoadFileName(file, data.className);
		
		// ���W�Ƃ��ǂݍ���
		file.read(reinterpret_cast<char*>(&data.angle), sizeof(Vector3));
		file.read(reinterpret_cast<char*>(&data.scale), sizeof(Vector3));


		//// �f�[�^��ǉ�
		loadSelectRegisterObjectDatas.push_back(data);

		// ��Unullptr����Č�Ŋm��
		pRegisterObjects[data.typeName].emplace(data.objectName, nullptr);

		


		file.close();
	}

}

void MelLib::SceneEditer::LoadFileName(std::ifstream& stream, std::string& str)
{
	while (1)
	{
		char c;
		stream.read(&c, 1);
		if (c == '\0')break;
		str += c;
	}
}

void MelLib::SceneEditer::SelectEditData()
{
	if (!editorFlag || !ReleaseCheck())return;
	ImguiManager::GetInstance()->BeginDrawWindow("SelectScene");
	ImguiManager::GetInstance()->DrawList(selectEditDataNum, sceneFileNames);
	ImguiManager::GetInstance()->EndDrawWindow();

	if (Input::KeyTrigger(DIK_RETURN))
	{
		// �ǂݍ���
		LoadEditData(sceneFileNames[selectEditDataNum]);

		sceneFileNames.clear();
		selectingEditData = false;


	}
}

void MelLib::SceneEditer::LoadEditData(const std::string& sceneName)
{
	std::ifstream file(sceneName + EDIT_DATA_FORMAT, std::ios_base::binary);

	// ������������ďI��
	if (!file)
	{
		file.close();
		return;
	}

	// �I�u�W�F�N�g�̍폜
	addObjects.clear();
	GameObjectManager::GetInstance()->AllEraseObject();

	// �I�u�W�F�N�g�����邩�m�F
	char c = 0;
	file.read(&c,1);



	// �ǂݍ���
	while (1)
	{
		// �N���X��
		std::string className;
		LoadFileName(file, className);

		// �I�u�W�F�N�g��
		std::string objectName;
		LoadFileName(file, objectName);
		addObjectNames.push_back(objectName);

		// �o�^��
		std::string regName;
		LoadFileName(file, regName);

		// �����N���X�A�����I�u�W�F�N�g����T���Amake_shared��Ԃ��Ă��炤
		std::shared_ptr<GameObject> pObject;
		for (const auto& m : pRegisterObjects)
		{
			for (const auto& pRegisterObject : m.second)
			{
				GameObject* p = pRegisterObject.second.get();

				if (className == typeid(*p).name())
				{
					pObject = p->GetNewPtr();
					break;
				}
			}
			if (pObject)break;
		}

		// pObject��nullptr��������(�o�^�֐��œo�^����ĂȂ�����)�X�L�b�v
		if (!pObject)
		{
			// �����Ƀ��b�Z�[�W
			break;
		}


		// ���f���I�u�W�F�N�g��SetPosition���Ɍ��̍��W�ɉ��Z���Ă��܂����߁AGUI����f�[�^��ǂݍ��ނƂ��̒l�����Z����Ă��܂����߁A
		// �ʒu�����������Ȃ�
		
		//2023_04_14
		// �I�u�W�F�N�g�̍��W�ƃ��f���I�u�W�F�N�g�̍��W�𓯂��ɂ��Ă�̂ɃY���ĕۑ������̂͂��������̂ł́H
		// �������������΂悳����

		// �ǉ��I�u�W�F�N�g�ꗗ�ɒǉ�
		addObjects.push_back(pObject);

		//���W�Ƃ�
		Vector3 position;
		file.read(reinterpret_cast<char*>(&position), sizeof(Vector3));
		pObject->SetPosition(position);
		Vector3 angle;
		file.read(reinterpret_cast<char*>(&angle), sizeof(Vector3));
		pObject->SetAngle(angle);
		Vector3 scale;
		file.read(reinterpret_cast<char*>(&scale), sizeof(Vector3));
		pObject->SetScale(scale);



		// �Ǘ��N���X�ɒǉ�
		GameObjectManager::GetInstance()->AddObject(pObject);

		char c;
		file.read(&c, 1);
		if (c == -2)break;
	}

	file.close();
}

void MelLib::SceneEditer::UpdateSelectObject()
{
	if (!editorFlag || !ReleaseCheck())return;
	if (!isEdit)return;
	

#ifdef _DEBUG

#else
	if (!releaseEdit)return;
#endif // _DEBUG


	/*MelLib::Vector3 pos = pEditSelectObject->GetPosition();
	ImguiManager::GetInstance()->DrawSliderVector3("Position", pos, -1000, 1000);
	pEditSelectObject->SetPosition(pos);

	MelLib::Vector3 angle = pEditSelectObject->GetAngle();
	ImguiManager::GetInstance()->DrawSliderVector3("Angle", angle, 0, 359);
	pEditSelectObject->SetAngle(angle);

	MelLib::Vector3 scale = pEditSelectObject->GetScale();
	ImguiManager::GetInstance()->DrawSliderVector3("Scale", scale, 0, 359);
	pEditSelectObject->SetScale(scale);*/
}

void MelLib::SceneEditer::UpdateCamera()
{
	if (!editorFlag || !ReleaseCheck())return;
	// �G�f�B�^�[�p�̃J���������
	// ���C���J�����g���Ɛݒ�Ƃ��ϐ��̐ؑւ����������߂�ǂ�������
	// ���ƕϐ�����



	/*ImguiManager::GetInstance()->BeginDrawWindow(CAMERA_WINDOW_NAME);

	Vector3 cameraPosition = pEditerCamera->GetCameraPosition();
	ImguiManager::GetInstance()->DrawSliderVector3("CameraPosition", cameraPosition, -1000, 1000);

	Vector3 cameraAngle = pEditerCamera->GetAngle();
	ImguiManager::GetInstance()->DrawSliderVector3("CameraAngle", cameraAngle, -359, 359);

	ImguiManager::GetInstance()->EndDrawWindow();

	pEditerCamera->SetRotateCriteriaPosition(cameraPosition);
	pEditerCamera->SetAngle(cameraAngle);*/

}

void MelLib::SceneEditer::DrawObjectList()
{
	if (!editorFlag || !ReleaseCheck())return;
	// �I�u�W�F�N�g�}�l�[�W���[�ɖ��O�����擾����֐�����Ă���������
	// ����string�̔z��ɓ����Ə����x���Ȃ邩��I�u�W�F�N�g�ǉ����ɖ��O�ǉ����Ă�������
	// ���O�ύX�������ɕύX���鏈������Ȃ��Ƃ���o���Ȃ�
	// �Q�[�����ɂ͎g��Ȃ����낤���炠����x�x���Ă����Ċ����͂���
	// umap�ɂ��ăL�[��GameObject�ɂ���΃A�N�Z�X�y����
	// 
	// �������O�̃I�u�W�F�N�g���������疼�O�ɔԍ��t������
	// OBJECT_NAME��萔����ϐ��ɕς��Ȃ��Ƃ����Ȃ�
	// �ϐ��ɂ����炠�Ƃ��疼�O�ς�����ł��邩��ϐ��ɂ�������Ă���
	

	// �����\�[�g���Ă���������
	// ����const�ɂ�����
	// GameObjectManager�Ŗ��O�̔z�����ĊǗ��o����悤�ɂ��Ă���const�ɕύX����
	std::vector<std::string>objectNames;
	objectNames.reserve(addObjects.size());
	for (auto& object : addObjects) 
	{
		objectNames.push_back(object->GetObjectName());
	}
	
	// ImguiManager��vector���ă��X�g�\���ł���悤�ɂ��邩��AGetObjectName��char�̔z��n���Ď󂯎�鏈�����K�v�Ȃ�
	const size_t OBJECT_SIZE = objectNames.size();
	
	if (OBJECT_SIZE == 0)return;
	
	ImguiManager::GetInstance()->BeginDrawWindow("ObjectList");
	ImguiManager::GetInstance()->DrawList(selectListObjectNum, objectNames);
	selectListObjectName = objectNames[selectListObjectNum];
	ImguiManager::GetInstance()->EndDrawWindow();

	SetDrawWindowFlag(objectNames);

	for (const auto& p : addObjects)
	{
		if (p->GetObjectName() == selectListObjectName)
		{
			pSelectListObject = p.get();
		}
	}
}

void MelLib::SceneEditer::SetDrawWindowFlag(const std::vector<std::string>& objNames)
{
	if (!editorFlag || !ReleaseCheck())return;
	for (const auto& name : objNames) 
	{
		bool drawFlag = false;
		if (name == selectListObjectName) drawFlag = true;
		GuiValueManager::GetInstance()->SetDrawWindowFlag(name, drawFlag);
	}
}

void MelLib::SceneEditer::RegisterSelectObject()
{
	// �ǂݍ��ނƂ��́AGetNewPtr�֐����g����New����΂���
	// �������������o���Ȃ�?
	// �N���X�����������o���āARegisterObject�œo�^�����I�u�W�F�N�g��GetNewPtr���Ăяo����OK
	if (!editorFlag || !ReleaseCheck())return;


	// �R�s�[�𐶐�
	std::shared_ptr<GameObject> object = pEditSelectObject->GetNewPtr();
	pEditSelectObject->CopyObjectData(*object, GameObject::CopyGameObjectContent::EDIT);
	
	// �E�B���h�E���ύX
	GuiValueManager::GetInstance()->ChangeWindowName(object->GetObjectName(), inputObjectName);
	// ���͂��ꂽ���O��ݒ�
	object->SetObjectName(inputObjectName);
	// GUI�̒l�̃R�s�[
	GuiValueManager::GetInstance()->CopyGuiValue(pEditSelectObject->GetObjectName(), object->GetObjectName());


	// ���O�̓o�^
	registerSelectObjectNames.push_back(inputObjectName);


	object->SetPreData();
	// �o�^
	RegisterObject(object, inputObjectType);
}

void MelLib::SceneEditer::InputObjectName()
{
	if (!editorFlag || !ReleaseCheck())return;
	//ImguiManager::GetInstance()->DrawTextBox("Input Object Name", inputObjectName, 20);
	std::string s = "Object";
	char c[21];
	for (int i = 0; i < 21; i++) 
	{
		if (i < s.size())c[i] = s[i];
		else c[i] = ' ';
	}
	c[20] = '\0';
	
	ImGui::Text("Input Object Name");
	ImGui::InputText("",c, 21);

	inputObjectName = c;

	if (Input::KeyTrigger(DIK_RETURN))
	{
		// ���O�����ɓo�^����Ă��邩�`�F�b�N
		for (const auto& m : pRegisterObjects) 
		{
			if (m.second.find(inputObjectName) == m.second.end())inpttingObjectName = false;
		}

	}
}

void MelLib::SceneEditer::InputObjectType()
{
	if (!editorFlag || !ReleaseCheck())return;
	//ImguiManager::GetInstance()->DrawTextBox("Input Object Type", inputObjectType, 20);

	std::string s = "Object";
	char c[21];
	for (int i = 0; i < 21; i++)
	{
		if (i < s.size())c[i] = s[i];
		else c[i] = ' ';
	}
	c[20] = '\0';

	ImGui::Text("Input Object Type");
	ImGui::InputText("", c, 21);

	inputObjectType = c;

	if (Input::KeyTrigger(DIK_RETURN))
	{
		inpttingObjectType = false;
	}
}

void MelLib::SceneEditer::OtherCameraGuiDrawFlagFalse()
{
	if (!editorFlag || !ReleaseCheck())return;
	std::vector<std::string>cameraNames;
	Camera::GetCameraNames(cameraNames);

	for (const auto& name : cameraNames) 
	{
		if (name == pEditerCamera->GetCameraName())continue;
		GuiValueManager::GetInstance()->SetDrawWindowFlag(name, false);
	}
}

void MelLib::SceneEditer::Reset()
{
	if (!editorFlag || !ReleaseCheck())return;
}

void MelLib::SceneEditer::SetAddObjectsGUIData()
{
	for (auto& object : addObjects)
	{
		object->SetPreData();
		object->SetGUIData();
		object->SetPreDataPositions();
	}

}

bool MelLib::SceneEditer::ReleaseCheck()
{

#ifdef _DEBUG

#else
	return !releaseEdit;
#endif // _DEBUG

	return true;
}



MelLib::SceneEditer* MelLib::SceneEditer::GetInstance()
{
	static SceneEditer s;
	return &s;
}

void MelLib::SceneEditer::RegisterObject(const std::shared_ptr<MelLib::GameObject>& pObject , const std::string& objectType)
{


	const std::string OBJECT_NAME = pObject->GetObjectName();
	// C++20��contains�ɒu�������ł���
	if (pRegisterObjects[objectType].find(OBJECT_NAME) != pRegisterObjects[objectType].end())
	{
		std::string text = "�V�[���G�f�B�^�ɂ͊���"+ OBJECT_NAME + "�Ƃ������O�̃I�u�W�F�N�g���o�^����Ă��܂��B\0";
		OutputDebugStringA(text.c_str());
		return;
	}

	pRegisterObjects.try_emplace(objectType, std::map<std::string,std::shared_ptr<MelLib::GameObject>>());
	pRegisterObjects[objectType].emplace(OBJECT_NAME,pObject);
	//test.push_back(pObject);
	//registerObjectOrderDatas.try_emplace(pRegisterObjects.size() - 1, objectType);

	pObject->SetPreData();

	registerObjectNames.clear();
	registerObjectTypes.clear();
	registerObjectOrderDatas.clear();

	for (const auto& m : pRegisterObjects) 
	{
		registerObjectTypes.push_back(m.first);
		

		registerObjectOrderDatas.emplace(m.first, std::unordered_map<int, std::string>());
		registerObjectNames.emplace(m.first, std::vector<std::string>());
		
		int i = 0;
		for (const auto& object : m.second)
		{
			registerObjectOrderDatas[m.first].emplace(i, object.first);
			registerObjectNames[m.first].push_back(object.first);

			i++;
		}
	}


	for (const auto& data : loadSelectRegisterObjectDatas) 
	{
		// ��v���Ȃ��܂��͊��Ƀ��������m�ۂ��Ă����玟��
		if (data.className != typeid(*pObject).name() 
			|| pRegisterObjects.at(data.typeName).at(data.objectName))continue;

		// �Q�Ƃ���
		std::shared_ptr<GameObject>&pRefObject = pRegisterObjects[data.typeName][data.objectName];

		// �������m�ۂ��Ċi�[
		pRefObject = pObject->GetNewPtr();

		const std::string PRE_NAME = pRefObject->GetObjectName();

		// �f�[�^���Z�b�g
		pRefObject->SetObjectName(data.objectName);
		pRefObject->SetAngle(data.angle);
		pRefObject->SetScale(data.scale);

		GuiValueManager::GetInstance()->ChangeWindowName(PRE_NAME, data.objectName);
		GuiValueManager::GetInstance()->LoadGUIFileData(data.objectName);

		pRefObject->SetPreData();
		
	}
	GuiValueManager::GetInstance()->ChangeWindowName(pObject->GetObjectName(),OBJECT_NAME);
	
}

void MelLib::SceneEditer::MouseInputCamera()
{
	Camera* pCamera = Camera::Get(CAMERA_WINDOW_NAME);

#pragma region �O��ړ�(�z�C�[��)����
	

	long wheelValue = Input::GetMouseWheelValue();

	if (wheelValue != 0) 
	{
		// 1�l�̎��_���ƒ����_���������炱�ꂶ��ʖ�
		//pCamera->SetCameraToTargetDistance(pCamera->GetCameraToTargetDistance() + wheelValue);
		wheelValue /= 45;

		Vector3 rotMoveVector =
			LibMath::RotateZXYVector3(Vector3(0, 0, wheelValue), 
				pCamera->GetAngle());

		pCamera->SetRotateCriteriaPosition(pCamera->GetRotateCriteriaPosition() + rotMoveVector);
	}
#pragma endregion


	Vector2 mousePos = MelLib::Input::GetMousePosition();
	Vector2 mousePosDifference = mousePos - preMousePos;
	

#pragma region ��](�E�N���b�N)����


	if (Input::MouseButtonState(MouseButton::RIGHT)) 
	{
		Vector2 movePos = mousePosDifference / 5;
		pCamera->SetAngle(pCamera->GetAngle() + Vector3(movePos.y, movePos.x, 0));
	}

#pragma endregion

#pragma region �㉺���E�ړ�(���{�^��)����
	
	if (Input::MouseButtonState(MouseButton::CENTER)) 
	{
		Vector2 movePos = mousePosDifference / 2;
		Vector3 rotMoveVector = 
			LibMath::RotateZXYVector3(Vector3(-movePos.x, movePos.y, 0),
				pCamera->GetAngle());
		pCamera->SetRotateCriteriaPosition(pCamera->GetRotateCriteriaPosition() + rotMoveVector);
	}

#pragma endregion


	preMousePos = mousePos;

}


void MelLib::SceneEditer::Initialize()
{

	if (!editorFlag || !ReleaseCheck())return;
	LoadRegisterSelectObject();

	SaveEditData(TEST_START_EDIT_DATA_NAME);

	Camera::Create(CAMERA_WINDOW_NAME);
	pEditerCamera = Camera::Get(CAMERA_WINDOW_NAME);
	RenderTarget::Get()->SetCamera(pEditerCamera);
	pEditerCamera->SetFar(FLT_MAX);

	
	OtherCameraGuiDrawFlagFalse();
	isEdit = true;
}

void MelLib::SceneEditer::Update()
{
	if (!editorFlag || !ReleaseCheck())return;


	// �V�[���̍X�V�I���I�t����
	if (Input::KeyTrigger(DIK_F5))
	{
		isEdit = !isEdit;

		if (isEdit) 
		{
			// �X�R�A�Ȃǂ����Z�b�g���邽�߂ɓǂݍ��݂Ȃ���
			SceneManager::GetInstance()->ReLoadScene();
			LoadEditData(TEST_START_EDIT_DATA_NAME);


			RenderTarget::RenderTargetDrawData data;
			data.rt = RenderTarget::Get();
			std::vector<RenderTarget::RenderTargetDrawData>datas;
			datas.push_back(data);

			data.rt->SetCamera(pEditerCamera);
		}
		else 
		{
			SceneManager::GetInstance()->ReLoadScene();
			// �I�u�W�F�N�g�S���������̂ɒǉ�������0�ԂƂ��ɂȂ�Ȃ��s��C������
			// �O��̔ԍ��̏�Ԃ����s���ɔ��f������悤�ɂ���
			// �G�f�B�b�g���͗��p�҂�����������_�[�^�[�Q�b�g���g�p���Ȃ��悤�ɂ���

			// �J�n���_�̃f�[�^�������o��
			SaveEditData(TEST_START_EDIT_DATA_NAME);


			// �؂�ւ����u�Ԃ̃J�����̖��O��ۑ����Ă����A�G�f�B�b�g�I�t�ɂ����Ƃ��ɂ���ɐ؂�ւ���悤�ɂ���
			// �؂�ւ����u�Ԃ̃J�������ŏ��Ɏg����J�����Ƃ͌���Ȃ��̂ł悭�Ȃ�
			// 
			// �G�f�B�^�[�ŊJ�n���̃J�������Z�b�g�ł���悤�ɂ���΂悳����
			
			// �X���C�_�[��������Ȃ��Ē��ړ��͂��ł���悤�ɂ��������悳����

			RenderTarget::Get()->SetCamera(Camera::Get());

			// �I�u�W�F�N�g�ǉ�
			for (auto& object : addObjects) 
			{
				GameObjectManager::GetInstance()->AddObject(object);
			}
			
		}

	}
	if (!isEdit)return;

	// �X�V
	SetAddObjectsGUIData();

	if (pRegisterObjects.size() == 0 || !ImguiManager::GetInstance()->GetReleaseDrawFrag())return;

#pragma region �I��

	ImguiManager::GetInstance()->BeginDrawWindow(EDIT_WINDOW_NAME);

	// �X���C�_�[�ƃ{�b�N�X�؂�ւ�����悤�ɂ���
	bool pushChangeButton = ImguiManager::GetInstance()->DrawButton("ChangeSlider Box");
	

	// �o�^�{�^���ǉ�
	bool push = false;
	push = ImguiManager::GetInstance()->DrawButton("RegisterSelectObject");
	//if (push)RegisterSelectObject();
	if (push && !inpttingObjectName && !inpttingObjectType && !inpttingEditDataName && !selectingEditData)
	{
		inpttingObjectName = true;
		inputObjectName = "Object";
		inputObjectType = "Object";
	}

	if (inpttingObjectType)
	{
		InputObjectType();
		
		// �����̊֐��̃G���^�[�����������if�̒��ɏ����Ă�������
		if (!inpttingObjectType)
		{
			inpttingObjectType = false;
			RegisterSelectObject();
		}
	}
	if (inpttingObjectName)
	{
		InputObjectName();
		if (!inpttingObjectName)
		{
			inpttingObjectName = false;
			inpttingObjectType = true;
		}
	}


	// �L�[�̐��������W�I�{�^���`��
	int count = 0;
	for (const auto& object : pRegisterObjects)
	{
		ImguiManager::GetInstance()->DrawRadioButton(object.first, selectType, count);
		count++;
	}


	// ���W�I�{�^���őI�������I�u�W�F�N�g�ꗗ�̎Q��
	std::map <std::string, std::shared_ptr<MelLib::GameObject>>& refObjects = pRegisterObjects[registerObjectTypes[selectType]];


	// ���X�g����
	ImguiManager::GetInstance()->DrawList(registerObjectListNum, registerObjectNames[registerObjectTypes[selectType]]);

	//// �����ɃX���C�_�[�쐬����
	//int sliderNum = 0;
	//ImguiManager::GetInstance()->DrawSliderInt("Object", sliderNum, 0, refObjects.size() - 1);

	if (registerObjectListNum >= registerObjectOrderDatas[registerObjectTypes[selectType]].size())
	{
		registerObjectListNum = registerObjectOrderDatas[registerObjectTypes[selectType]].size() - 1;
	}

	//// �I�΂ꂽ�I�u�W�F�N�g�̃|�C���^��pSelectObject�ɑ��
	const std::string OBJECT_NAME = registerObjectOrderDatas[registerObjectTypes[selectType]][registerObjectListNum];

	pEditSelectObject = refObjects[OBJECT_NAME].get();
	pEditSelectObject->SetGUIData();

	// pEditSelectObject�̃E�B���h�E�`��ݒ�
	for (const auto& p : pRegisterObjects) 
	{
		for (const auto& object : p.second)
		{
			if(object.first == OBJECT_NAME)GuiValueManager::GetInstance()->SetDrawWindowFlag(object.first, true);
			else GuiValueManager::GetInstance()->SetDrawWindowFlag(object.first, false);
		}
	}


	if (pushChangeButton && !inpttingObjectName && !inpttingObjectType && !inpttingEditDataName && !selectingEditData)
	{
		typingInputFlag = !typingInputFlag;

		GuiValueManager::GetInstance()->ChangeTypingInputFlag(pEditSelectObject->GetObjectName());
		GuiValueManager::GetInstance()->ChangeTypingInputFlag(CAMERA_WINDOW_NAME);

		if (pSelectListObject)GuiValueManager::GetInstance()->ChangeTypingInputFlag(pSelectListObject->GetObjectName());



	}

#pragma endregion

	UpdateSelectObject();

	// �ǉ�
	if (Input::KeyTrigger(DIK_SPACE))
	{
		// GetNewPtr�֐��Ń������m��
		std::shared_ptr<GameObject>pObject = pEditSelectObject->GetNewPtr();

		if (pObject)
		{
			// �ǉ����ɍ��W�Ƃ��Z�b�g�������̂߂�ǂ�����
			// pSelectObject���Ǘ��N���X�ɒǉ�����
			// �߂�l�̂�V����map�ɓ���Ă���������
			// �ǂ����݂��Z�b�g���Ȃ��Ƃ����Ȃ�����
			/*pObject->SetPosition(pEditSelectObject->GetPosition());
			pObject->SetAngle(pEditSelectObject->GetAngle());
			pObject->SetScale(pEditSelectObject->GetScale());*/

			// �R�s�[
			pEditSelectObject->CopyObjectData(*pObject, GameObject::CopyGameObjectContent::EDIT);

			// GUI�̃R�s�[
			// ������Ȃ��̃R�s�[����Ȃ�����
			GuiValueManager::GetInstance()->CopyGuiValue(pEditSelectObject->GetObjectName(), pObject->GetObjectName());

			// �ǉ�
			//GameObjectManager::GetInstance()->AddObject(pObject);

			// �S�č폜
			// (�o�^�����I�u�W�F�N�g�̃R���X�g���N�^�ɃI�u�W�F�N�g��ǉ����鏈��������ꍇ�A�ǉ����ꂽ�I�u�W�F�N�g���\������邽��)
			GameObjectManager::GetInstance()->AllEraseObject();
			addObjects.push_back(pObject);
			addObjectNames.push_back(pEditSelectObject->GetObjectName());

			// �t���O�Z�b�g
			GuiValueManager::GetInstance()->SetTypingInputFlag(pObject->GetObjectName(),typingInputFlag);
		}
		else
		{
			std::string text;
			text = "�I�u�W�F�N�g�ǉ�����GetNewPtr()��nullptr��Ԃ��܂����B�I�u�W�F�N�g��ǉ��ł��܂���B\n";
			OutputDebugStringA(text.c_str());
		}
	}

	ImguiManager::GetInstance()->EndDrawWindow();

	bool pushControl = Input::KeyState(DIK_LCONTROL) || Input::KeyState(DIK_RCONTROL);

	


	// �ۑ�
	if (pushControl && Input::KeyTrigger(DIK_S) && !inpttingObjectName && !inpttingObjectType && !inpttingEditDataName && !selectingEditData)StartSave();
	if (inpttingEditDataName)InputEditDataName();
	// �Ǎ�
	if (pushControl && Input::KeyTrigger(DIK_L) && !inpttingObjectName && !inpttingObjectType && !inpttingEditDataName && !selectingEditData)Load();
	if (selectingEditData)SelectEditData();

	// �I�u�W�F�N�g�ꗗ�̕`��
	DrawObjectList();

	// �폜
	if (pushControl && Input::KeyTrigger(DIK_D) && pSelectListObject)
	{
		pSelectListObject->TrueEraseManager();

		const size_t SIZE = addObjects.size();
		for (size_t i = 0; i < SIZE; i++)
		{
			if (pSelectListObject == addObjects[i].get())
			{
				addObjects.erase(addObjects.begin() + i);
				addObjectNames.erase(addObjectNames.begin() + i);
				//GameObjectManager::GetInstance()->EraseObject(pSelectListObject);
				pSelectListObject = nullptr;
			}
		}
	}

	// ���Z�b�g
	if (Input::KeyTrigger(DIK_ESCAPE)) 
	{
		// �J�n���̃I�u�W�F�N�g�̃f�[�^�Ɠr���ǉ������I�u�W�F�N�g�̃f�[�^������
		// ���ꂩ�t�@�C���Ђ炫���ςɂ��Ă���
		// �J�����ςȂ��̕����e�X�g�v���C���Ƀt�@�C�������Ď~�܂�Ƃ������������ƂȂ��Ȃ邩��
	}

	//// �`��ݒ�
	//for (auto& obj : addObjects) 
	//{
	//	if (pEditSelectObject == obj)pEditSelectObject->SetDrawGUIFlag(true);
	//	else obj->SetDrawGUIFlag(false);
	//}

	pEditSelectObject->SetPreData();


	UpdateCamera();
	MouseInputCamera();
}

void MelLib::SceneEditer::Draw()
{
	if (!editorFlag || !ReleaseCheck())return;

	if (!isEdit)return;


	if (pRegisterObjects.size() == 0 || !ImguiManager::GetInstance()->GetReleaseDrawFrag())return;

	pEditSelectObject->Draw();

	for (auto& addObject : addObjects) 
	{
		addObject->Draw();
	}
}

void MelLib::SceneEditer::SetReleaseEditFlag(const bool flag)
{
	releaseEdit = flag;
	if (!flag)isEdit = false;
}

//
//void MelLib::SceneEditer::SetEditFlag(const bool flag)
//{
//	// Debug���̓����[�X�r���h�̃t���O�m�F����
//#ifdef _DEBUG
//
//#else
//	if (!releaseEdit)
//	{
//		return;
//	}
//#endif // _DEBUG
//
//	isEdit = flag;
//}
