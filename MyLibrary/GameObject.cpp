#include "GameObject.h"

#include"GuiValueManager.h"

#include"LibMath.h"
#include"Physics.h"

using namespace MelLib;

float GameObject::gravutationalAcc = Physics::GRAVITATIONAL_ACCELERATION_EARTH / 30;

std::unordered_map<std::string, int>GameObject::objectCreateNumber;

#ifdef _DEBUG


//����m�F�p���f���̃p�C�v���C��
PipelineState GameObject::collisionCheckModelPipelineState;

//�}�e���A��
ADSAMaterial GameObject::material;
#endif // _DEBUG

void MelLib::GameObject::SetModelPosition(const Vector3& vec)
{
	for (auto& m : modelObjects)
	{
		m.second.SetPosition(m.second.GetPosition() + vec);
	}
}

void MelLib::GameObject::SetDataPosition(const Vector3& vec)
{
	for (auto& d : sphereDatas)
	{
		for (auto& d2 : d.second) 
		{
			d2.SetPosition(d2.GetPosition() + vec);
		}
	}
	for (auto& d : boxDatas) 
	{
		for (auto& d2 : d.second)
		{
			d2.SetPosition(d2.GetPosition() + vec);
		}
	}
	for (auto& d : obbDatas)
	{
		for (auto& d2 : d.second)
		{
			d2.SetPosition(d2.GetPosition() + vec);
		}
	}
	for (auto& d : triangleDatas)
	{
		for (auto& d2 : d.second)
		{
			d2.SetPosition(d2.GetPosition() + vec);
		}
	}
	for (auto& d : rayDatas)
	{
		for (auto& d2 : d.second)
		{
			d2.SetPosition(d2.GetPosition() + vec);
		}
	}

	for (auto& d : boardDatas)
	{
		for (auto& d2 : d.second)
		{
			d2.SetPosition(d2.GetPosition() + vec);
		}
	}
	for (auto& d : segment3DDatas)
	{
		for (auto& d2 : d.second)
		{
			d2.SetPosition(d2.GetPosition() + vec);
		}
	}
	for (auto& d : capsuleDatas)
	{
		for (auto& d2 : d.second)
		{
			d2.GetRefSegment3DData().SetPosition(d2.GetSegment3DData().GetPosition() + vec);
		}
	}
	
}

void MelLib::GameObject::SetModelAngle(const Vector3& angle)
{

	for (auto& m : modelObjects)
	{
		m.second.SetAngle(angle);
	}
}

void MelLib::GameObject::SetDataAngle(const Vector3& angle)
{
	for (auto& d : obbDatas)
	{
		for (auto& d2 : d.second)
		{
			d2.SetAngle(angle);
		}
	}
	for (auto& d : triangleDatas)
	{
		for (auto& d2 : d.second)
		{
			d2.SetAngle(angle);
		}
	}

	for (auto& d : boardDatas)
	{
		for (auto& d2 : d.second)
		{
			d2.SetAngle(angle);
		}
	}
	for (auto& d : segment3DDatas)
	{
		for (auto& d2 : d.second)
		{
			d2.SetAngle(angle);
		}
	}
	for (auto& d : capsuleDatas)
	{
		for (auto& d2 : d.second)
		{
			d2.GetRefSegment3DData().SetAngle(angle);
		}
	}
}

void MelLib::GameObject::SetModelScale(const Vector3& scale)
{
	for (auto& m : modelObjects)
	{
		m.second.SetScale(scale);
	}
}

void MelLib::GameObject::SetDataScale(const Vector3& scale)
{
	for (auto& d : sphereDatas)
	{
		for (auto& d2 : d.second)
		{
			float setRadius = scale.x;
			if (setRadius < scale.y)setRadius = scale.y;
			if (setRadius < scale.z)setRadius = scale.z;

			d2.SetRadius(setRadius / 2);
		}
	}
	for (auto& d : boxDatas)
	{
		for (auto& d2 : d.second)
		{
			d2.SetSize(scale);
		}
	}
	for (auto& d : obbDatas)
	{
		for (auto& d2 : d.second)
		{
			d2.SetSize(scale);
		}
	}
	for (auto& d : boardDatas)
	{
		for (auto& d2 : d.second)
		{
			d2.SetSize(scale.ToVector2());
		}
	}
	for (auto& d : segment3DDatas)
	{
		for (auto& d2 : d.second)
		{
			d2.SetPosition(d2.GetPosition() * scale);
		}
	}
	for (auto& d : capsuleDatas)
	{
		for (auto& d2 : d.second)
		{
			d2.GetRefSegment3DData().SetPosition(d2.GetSegment3DData().GetPosition() * scale);
		}
	}

	for (auto& d : triangleDatas)
	{
		for (auto& d2 : d.second)
		{
			d2.SetScale(scale);
		}
	}
}

GameObject::GameObject(const std::string& name)
	:objectName(name)
	,position(guiPosition.GetRefValue())
	,angle(guiAngle.GetRefValue())
	,scale(guiScale.GetRefValue())
{
	// ���̂܂܂��Ɠo�^����������GUI�ɒǉ������
	// �I�u�W�F�N�g�}�l�[�W���[�ɒǉ��������ɒǉ�����(SetData���Ăяo��)�悤�ɂ���Ζ��Ȃ�
	// SetData���Ăяo���֐����
	// GameObject���g�����ǃI�u�W�F�N�g�}�l�[�W���[���g��Ȃ����͂��̊֐��������ŌĂяo���Ă��炤


	// ���Z
	objectCreateNumber[objectName]++;

	// 0�ȊO�͔ԍ��t����
	if (objectCreateNumber[objectName] != 1) 
	{
		objectName += "_" + std::to_string(objectCreateNumber[objectName] - 1);
	}




	guiPosition.SetData(0, objectName, "Position", -1000, 1000);
	guiAngle.SetData(0, objectName, "Angle", -359, 359);
	guiScale.SetData(1, objectName, "Scale", -10, 10);





	SetPreData();
	//SetGUIData();
	//SetPreDataPositions();
}


GameObject::~GameObject()
{
	GuiValueManager::GetInstance()->DeleteWindow(objectName);
}

void MelLib::GameObject::Initialize()
{
}

//void GameObject::Initialize()
//{
//}

void GameObject::Update()
{
}

void GameObject::Draw()
{

}

//void GameObject::Hit
//(
//	const GameObject* const  object,
//	const ShapeType3D& collisionType,
//	const int arrayNum,
//	const ShapeType3D& hitObjColType,
//	const int hitObjArrayNum
//)
//{
//}
//
//const void* GameObject::GetPtr() const
//{
//	return nullptr;
//}

//void GameObject::CalcHitPhysics(GameObject* hitObject, const Vector3& hutPreVelocity, const CollisionType& collisionType)
//{
//
//	//switch (collisionType)
//	//{
//	//case CollisionType::COLLISION_SPHERE:
//	//	
//	//	velocity = Physics::CalcRepulsionVelocity
//	//	(
//	//	);
//	//	break;
//	//}
//}


void MelLib::GameObject::Hit
(
	const GameObject& object,
	const ShapeType3D shapeType,
	const std::string& shapeName,
	const ShapeType3D hitObjShapeType,
	const std::string& hitShapeName
)
{
}



void GameObject::FalseEraseManager()
{
	eraseManager = false;
}

void MelLib::GameObject::AddPosition(const Vector3& vec)
{
	SetModelPosition(vec);
	SetDataPosition(vec);
	position += vec;
}

void MelLib::GameObject::SetPosition(const Vector3& pos)
{
	SetModelPosition(pos - position);
	SetDataPosition(pos - position);
	 
	position = pos;

}

void MelLib::GameObject::SetObjectAndModelPosition(const Vector3& pos)
{
	for (auto& m : modelObjects)
	{
		m.second.SetPosition(pos);
	}
	SetDataPosition(pos - position);

	position = pos;
}

void MelLib::GameObject::SetAngle(const Vector3& angle)
{
	//if (this->angle == angle)return;

	this->angle = angle;

	SetModelAngle(angle);
	SetDataAngle(angle);
}

void MelLib::GameObject::SetScale(const Vector3& scale)
{
	//if (this->scale == scale)return;

	this->scale = scale;

	SetModelScale(scale);
	SetDataScale(scale);
}

void MelLib::GameObject::SetAddColor(const Color& color)
{
	for (auto& object : modelObjects)
	{
		object.second.SetAddColor(color);
	}
}

void MelLib::GameObject::SetSubColor(const Color& color)
{
	for (auto& object : modelObjects)
	{
		object.second.SetSubColor(color);
	}
}

void MelLib::GameObject::SetMulColor(const Color& color)
{
	for (auto& object : modelObjects)
	{
		object.second.SetMulColor(color);
	}
}

void MelLib::GameObject::SetDrawGUIFlag(bool flag)
{
	GuiValueManager::GetInstance()->SetDrawWindowFlag(objectName, flag);
}

void MelLib::GameObject::SetPreData()
{
	prePosition = position;
	preAngle = angle;
	preScale = scale;
}


void MelLib::GameObject::SetGUIData()
{
	SetModelPosition(position - prePosition);
	SetDataPosition(position - prePosition);

	SetAngle(angle);
	SetScale(scale);
}


void MelLib::GameObject::CopyObjectData(GameObject& object, CopyGameObjectContent content)
{
	object.position = position;
	object.angle = angle;
	object.scale = scale;

	// �����蔻����R�s�[���s���悤�ɂ���
	object.sphereDatas = sphereDatas;
	object.boxDatas = boxDatas;
	object.segment3DDatas = segment3DDatas;
	object.rayDatas = rayDatas;
	object.boardDatas = boardDatas;
	object.capsuleDatas = capsuleDatas;
	object.obbDatas = obbDatas;
	object.triangleDatas = triangleDatas;
	object.collisionFlag = collisionFlag;

	
	// ���ꂾ��Create���Ăяo���ꂿ�Ⴄ���獇�v��2��Ăяo�����\��������
	// �������č�蒼���Ȃ��Ƃ����Ȃ�
	// ��肭�p�����[�^�[�⃂�f�����������ւ���
	// ���l���R�s�[���邩�S���R�s�[���邩�I�ׂ�悤�ɂ���
	// 2��Create�����R�s�[���鏈���͂܂������ĂȂ�2022_11_06
	// clear()�Ăяo���Ă邩��Create������������̂ł�

	ModelObject::CopyModelObjectContent modelObjectC = ModelObject::CopyModelObjectContent::NUMBER_FLAG;
	
	// �S���R�s�[����ꍇ�͏���
	if (content == CopyGameObjectContent::ALL)
	{
		object.modelObjects.clear();
		modelObjectC = ModelObject::CopyModelObjectContent::ALL;
	}

	for (const auto& modelObj : modelObjects)
	{
		//object.modelObjects[modelObj.first] = modelObjects[modelObj.first];
		// ���l�A�t���O�����R�s�[
		object.modelObjects[modelObj.first].CopyModelObject
		(modelObjects[modelObj.first], object.objectName, modelObjectC);
	}

	//object.objectName = objectName;
}

std::shared_ptr<GameObject> MelLib::GameObject::GetNewPtr()
{
	return nullptr;
}

void MelLib::GameObject::SetPreDataPositions()
{
	for (auto& data : sphereDatas)
	{
		std::vector<Vector3>& refPrePositions = sphereDataPrePositions[data.first];

		// �T�C�Y���������ύX
		size_t preDataSize = refPrePositions.size();
		size_t dataSize = data.second.size();
		if (preDataSize != dataSize)refPrePositions.resize(dataSize);

		for (int i = 0; i < dataSize; i++) 
		{
			refPrePositions[i] = data.second[i].GetPosition();
		}
	}
	for (auto& data : boxDatas)
	{
		std::vector<Vector3>& refPrePositions = boxDataPrePositions[data.first];

		// �T�C�Y���������ύX
		size_t preDataSize = refPrePositions.size();
		size_t dataSize = data.second.size();
		if (preDataSize != dataSize)refPrePositions.resize(dataSize);

		for (int i = 0; i < dataSize; i++)
		{
			refPrePositions[i] = data.second[i].GetPosition();
		}
	}
	for (auto& data : segment3DDatas)
	{
		std::vector<Value2<Vector3>>& refPrePositions = segment3DDataPrePositions[data.first];

		// �T�C�Y���������ύX
		size_t preDataSize = refPrePositions.size();
		size_t dataSize = data.second.size();
		if (preDataSize != dataSize)refPrePositions.resize(dataSize);

		for (int i = 0; i < dataSize; i++)
		{
			refPrePositions[i] = data.second[i].GetPosition();
		}
	}
	for (auto& data : rayDatas)
	{
		std::vector<Vector3>& refPrePositions = rayDataPrePositions[data.first];

		// �T�C�Y���������ύX
		size_t preDataSize = refPrePositions.size();
		size_t dataSize = data.second.size();
		if (preDataSize != dataSize)refPrePositions.resize(dataSize);

		for (int i = 0; i < dataSize; i++)
		{
			refPrePositions[i] = data.second[i].GetPosition();
		}
	}
	for (auto& data : planeDatas)
	{
		std::vector<Vector3>& refPrePositions = planeDataPrePositions[data.first];

		// �T�C�Y���������ύX
		size_t preDataSize = refPrePositions.size();
		size_t dataSize = data.second.size();
		if (preDataSize != dataSize)refPrePositions.resize(dataSize);

		for (int i = 0; i < dataSize; i++)
		{
			refPrePositions[i] = data.second[i].GetPosition();
		}
	}
	for (auto& data : boardDatas)
	{
		std::vector<Vector3>& refPrePositions = boardDataPrePositions[data.first];

		// �T�C�Y���������ύX
		size_t preDataSize = refPrePositions.size();
		size_t dataSize = data.second.size();
		if (preDataSize != dataSize)refPrePositions.resize(dataSize);

		for (int i = 0; i < dataSize; i++)
		{
			refPrePositions[i] = data.second[i].GetPosition();
		}
	}
	for (auto& data : capsuleDatas)
	{
		std::vector<Value2<Vector3>>& refPrePositions = capsuleDataPrePositions[data.first];

		// �T�C�Y���������ύX
		size_t preDataSize = refPrePositions.size();
		size_t dataSize = data.second.size();
		if (preDataSize != dataSize)refPrePositions.resize(dataSize);

		for (int i = 0; i < dataSize; i++)
		{
			refPrePositions[i] = data.second[i].GetSegment3DData().GetPosition();
		}
	}
	for (auto& data : triangleDatas)
	{
		std::vector<Value3<Vector3>>& refPrePositions = triangleDataPrePositions[data.first];

		// �T�C�Y���������ύX
		size_t preDataSize = refPrePositions.size();
		size_t dataSize = data.second.size();
		if (preDataSize != dataSize)refPrePositions.resize(dataSize);

		for (int i = 0; i < dataSize; i++)
		{
			refPrePositions[i] = data.second[i].GetPosition();
		}
	}
	for (auto& data : obbDatas)
	{
		std::vector<Vector3>& refPrePositions = obbDataPrePositions[data.first];

		// �T�C�Y���������ύX
		size_t preDataSize = refPrePositions.size();
		size_t dataSize = data.second.size();
		if (preDataSize != dataSize)refPrePositions.resize(dataSize);

		for (int i = 0; i < dataSize; i++)
		{
			refPrePositions[i] = data.second[i].GetPosition();
		}
	}
}

void GameObject::CalcMovePhysics()
{
	Vector3 prePos = position;
	//�͂Əd���Ɖ����x�Ƒ��x�ō��W���v�Z
	position += Physics::CalcMoveResult
	(
		velocity,
		acceleration,
		force,
		mass
	);

	//�������̑��x
	if (isFall)
	{
		fallTime++;


		const float PRE_VEL_Y = currentFallVelovity;
		currentFallVelovity = Physics::CalcFallVelocity(fallStartSpeed, gravutationalAcc, fallTime);
		const float ADD_VEL_Y = currentFallVelovity - PRE_VEL_Y;

		//Velocity�擾���ɔ��f�����邽�߂�velocity�ɑ��
		//Get�֐���velocity�ɉ��Z����悤�ɂ���? 

		//�v�Z�ł́A���܂ł̑��x�����Z�������v���x�ł͂Ȃ��A���݂̑��x�����߂邽�߁A
		//velovcity.y��0�b�̎��̑��x + ���݂̑��x�ɂȂ�悤�ɂ��Ȃ���
		//velocity.y + upThrowVelocity(���Z����ƌ��݂̑��x)�Ɠ����ɂȂ�Ȃ��B
		velocity.y += ADD_VEL_Y;

		//���t���[�����x�����Z
		position.y += currentFallVelovity;

	}


	//�d�͉����x�͗������̂ݓK������悤�ɕύX����!!!!!!!
	//�d�͉����x�K��
	//position.y -= gravutationalAcc;


	//�����ňړ��ʂ��v�Z���āA����̍��W�𒲐�����?
	//���f���͂ǂ�����?

	//�����̌v�Z�֐���hit�֐��ŌĂяo���Ă��炤?

	SetModelPosition(position - prePos);
	SetDataPosition(position - prePos);
}


void GameObject::AllDraw() 
{
	for(auto& object:modelObjects)
	{
		object.second.Draw();
	}
}


unsigned int MelLib::GameObject::GetFrameHitCheckNumber(ShapeType3D type) const
{
	switch (type)
	{
	case MelLib::ShapeType3D::SPHERE:
		return sphereFrameHitCheckNum;
		break;
	case MelLib::ShapeType3D::BOX:
		return boxFrameHitCheckNum;
		break;
	case MelLib::ShapeType3D::OBB:
		return obbFrameHitCheckNum;
		break;
	case MelLib::ShapeType3D::PLANE:
		return planeFrameHitCheckNum;
		break;
	case MelLib::ShapeType3D::TRIANGLE:
		return triangleFrameHitCheckNum;
		break;
	case MelLib::ShapeType3D::BOARD:
		return boardFrameHitCheckNum;
		break;
	case MelLib::ShapeType3D::SEGMENT:
		return segment3DFrameHitCheckNum;
		break;
	case MelLib::ShapeType3D::RAY:
		return rayFrameHitCheckNum;
		break;
	case MelLib::ShapeType3D::CAPSULE:
		return capsuleFrameHitCheckNum;
		break;
	default:
		break;
	}

	return 1;
}


#ifdef _DEBUG



void MelLib::GameObject::CreateCollisionCheckModelPipelineState()
{
	DrawOption pData = PipelineState::GetDefaultDrawData(PipelineStateType::MODEL);
	pData.drawMode = DrawMode::WIREFRAME;
	pData.cullMode = CullMode::NONE;

	ShaderDataSet set =
	{
		{L"LIB","",""},
		{L"NULL","",""},
		{L"NULL","",""},
		{L"LIB","",""},
		{L"LIB","",""}
	};

	collisionCheckModelPipelineState.CreatePipeline
	(
		pData,
		set,
		PipelineStateType::MODEL,
		nullptr,
		1
	);

	DrawOption data = PipelineState::GetDefaultDrawData(PipelineStateType::MODEL);
	data.cullMode = CullMode::NONE;
	data.drawMode = DrawMode::WIREFRAME;

	material.Create(data,1);

}

void MelLib::GameObject::CollisionCheckModelCreateOrDelete
(
	const std::unordered_map<std::string,size_t>& datas, 
	std::unordered_map<std::string, std::vector<ModelObject>>& modelObjects,
	const ShapeType3D type
)
{
	// umap�̗v�f��(���O�̐���)���[�v
	for(const auto& data: datas)
	{
		// ���݂̊m�F�p���f�������擾
		size_t objNum = modelObjects.at(data.first).size();

		// ���݂̓����蔻�萔���擾
		size_t dataNum = data.second;

		if (dataNum > objNum)
		{
			modelObjects[data.first].resize(dataNum);

			//�s��������
			for (int i = objNum; i < dataNum; i++)
			{
				modelObjects[data.first][i].Create(ModelData::Get(type),objectName, nullptr);

				modelObjects[data.first][i].SetMaterial(&material);
			}
		}
		else if (dataNum < objNum)
		{
			modelObjects[data.first].resize(dataNum);
		}
	}


}

void MelLib::GameObject::CreateCollisionCheckModel()
{
	////���萔�ɉ����ă��f���𐶐�������폜�����肵�܂�
	//auto createOrDeleteModel = [](const size_t& dataNum, std::vector<ModelObject>& modelObjcts, const ShapeType3D type)
	//{
	//	size_t objNum = modelObjcts.size();
	//	if (dataNum > objNum)
	//	{
	//		size_t addSize = dataNum - objNum;
	//		modelObjcts.resize(dataNum);

	//		//�s��������
	//		for (int i = objNum; i < dataNum; i++)
	//		{
	//			modelObjcts[i].Create(ModelData::Get(type), nullptr);

	//			modelObjcts[i].SetMaterial(&material);
	//		}
	//	}
	//	else if (dataNum < objNum)
	//	{
	//		modelObjcts.resize(objNum);
	//	}

	//};

	////Box
	//createOrDeleteModel(boxDatas.size(), boxModelObjects, ShapeType3D::BOX);

	////Sphere
	//createOrDeleteModel(sphereDatas.size(), sphereModelObjects, ShapeType3D::BOX);

	////Board
	//createOrDeleteModel(boardDatas.size(), boardModelObjects, ShapeType3D::BOARD);

	////Segment
	//createOrDeleteModel(segment3DDatas.size(), segmentModelObjects[0], ShapeType3D::BOX);
	//createOrDeleteModel(segment3DDatas.size(), segmentModelObjects[1], ShapeType3D::BOX);

	////Capsule
	////�����쐬
	//createOrDeleteModel(capsuleDatas.size(), capsuleModelObjects[0], ShapeType3D::BOX);
	//createOrDeleteModel(capsuleDatas.size(), capsuleModelObjects[1], ShapeType3D::BOX);
	////�~�����쐬
	////createOrDeleteModel(capsuleData.size(), capsuleModelObjects[2], ShapeType3D::BOX);


	// �V�������

}


void MelLib::GameObject::SetCollisionCheckModelData()
{
	//Box
	for (auto& data : boxModelObjects)
	{
		for (size_t i = 0; i < data.second.size(); i++)
		{
			data.second[i].SetScale(boxDatas[data.first][i].GetSize());
			data.second[i].SetPosition(boxDatas[data.first][i].GetPosition());


			/*boxModelObjects[i].SetScale(boxData[i].GetSize());
			boxModelObjects[i].SetPosition(boxData[i].GetPosition());*/
		}
	}

	//Sphere
	for (auto& data : sphereModelObjects)
	{
		for (size_t i = 0; i < data.second.size(); i++)
		{
			data.second[i].SetScale(sphereDatas[data.first][i].GetRadius() * 2);
			data.second[i].SetPosition(sphereDatas[data.first][i].GetPosition());

		}
	}

	//Board
	for (auto& data : boardModelObjects)
	{
		for (size_t i = 0; i < data.second.size(); i++)
		{
			boardModelObjects[data.first][i].SetScale(boardDatas[data.first][i].GetSize().ToVector3());
			boardModelObjects[data.first][i].SetAngle(boardDatas[data.first][i].GetAngle());
			boardModelObjects[data.first][i].SetPosition(boardDatas[data.first][i].GetPosition());
		}
	}

	//Segment
	for (auto& data : segmentModelObjects)
	{
		for (size_t i = 0; i < data.second.size(); i++)
		{
			Value2<Vector3>& lineSegmentPos = segment3DDatas[data.first][i].GetRotatePosition();


			segmentModelObjects[data.first][0][i].SetScale(1);
			segmentModelObjects[data.first][0][i].SetPosition(lineSegmentPos.v1);
			segmentModelObjects[data.first][0][i].SetAngle(segment3DDatas[data.first][i].GetAngle());

			segmentModelObjects[data.first][1][i].SetScale(1);
			segmentModelObjects[data.first][1][i].SetPosition(lineSegmentPos.v2);
			segmentModelObjects[data.first][1][i].SetAngle(segment3DDatas[data.first][i].GetAngle());
		}
	}
	
	

	//Capsule
	for (auto& data : capsuleModelObjects)
	{
		for (size_t i = 0; i < data.second.size(); i++)
		{
			Value2<Vector3>& lineSegmentPos = capsuleDatas[data.first][i].GetSegment3DData().GetRotatePosition();


			capsuleModelObjects[data.first][0][i].SetScale(1);
			capsuleModelObjects[data.first][0][i].SetPosition(lineSegmentPos.v1);
			capsuleModelObjects[data.first][0][i].SetAngle(segment3DDatas[data.first][i].GetAngle());

			capsuleModelObjects[data.first][1][i].SetScale(1);
			capsuleModelObjects[data.first][1][i].SetPosition(lineSegmentPos.v2);
			capsuleModelObjects[data.first][1][i].SetAngle(segment3DDatas[data.first][i].GetAngle());
		}
	}


}

void MelLib::GameObject::DrawCollisionCheckModel()
{
	if (!drawCollisionModel)return;

	for (auto& objects : boxModelObjects)
	{
		for (auto& obj : objects.second) 
		{
			obj.Draw();
		}
	}

	for (auto& objects : sphereModelObjects)
	{
		for (auto& obj : objects.second)
		{
			obj.Draw();
		}
	}



	for (int i = 0, size = capsuleModelObjects.size(); i < size; i++)
	{
		for (auto& objects : capsuleModelObjects)
		{
			for (auto& obj : objects.second[i])
			{
				obj.Draw();
			}
		}
	}
}

#endif // _DEBUG
