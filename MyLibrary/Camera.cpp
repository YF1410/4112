#include "Camera.h"
#include"Library.h"
#include"LibMath.h"
#include"Input.h"
#include"GuiValueManager.h"

using namespace MelLib;
std::unordered_map<std::string, std::unique_ptr<Camera>>Camera::pCameras;
UINT Camera::createCount = 0;
std::string Camera::mainCameraName = "";

void Camera::CalcCameraData()
{
	//�x�N�g�����J�����̊p�x�ɉ����ĉ�]������֐�
	auto RotateVectorCameraAngle = [&](Vector3& vector)
	{
		vector = LibMath::RotateVector3(vector, Vector3(0, 0, -1), angle.z);
		vector = LibMath::RotateVector3(vector, Vector3(-1, 0, 0), angle.x);
		vector = LibMath::RotateVector3(vector, Vector3(0, -1, 0), angle.y);
	}; 


	//���̃A���S���Y���ŁA�J������90�x��](X��-�����Ɍ����Ă�)�Ƃ��ɁAFPS����TPS�؂�ւ���ƁA
	//�J�������v���C���[�̌��ɍs���āA�X�J�C�����̃J�����؂�ւ��݂����ɂȂ�
	if (rotatePoint == RotatePoint::ROTATE_POINT_CAMERA_POSITION)
	{
		targetPosition = Vector3(0, 0, cameraToTargetDistance);
		RotateVectorCameraAngle(targetPosition);
		targetPosition += rotatePointPosition;

		cameraPosition = rotatePointPosition;
	}
	else
	{
		cameraPosition = Vector3(0, 0, -cameraToTargetDistance);
		RotateVectorCameraAngle(cameraPosition);
		cameraPosition += rotatePointPosition;

		targetPosition = rotatePointPosition;
	}

	upVector = Vector3(0, 1, 0);
	RotateVectorCameraAngle(upVector);

}


MelLib::Camera::Camera()
	:rotatePointPosition(guiRotatePointPosition.GetRefValue())
	, angle(guiAngle.GetRefValue())
{
}

MelLib::Camera::~Camera()
{
	GuiValueManager* g = GuiValueManager::GetInstance();
	g->DeleteWindow(cameraName);
}

MelLib::Camera::Camera(const std::string& name)
	:cameraName(name) 
	, rotatePointPosition(guiRotatePointPosition.GetRefValue())
	, angle(guiAngle.GetRefValue())
{
	guiRotatePointPosition.SetData(Vector3(0, 0, -10), name, "Position", -1000, 1000);
	guiAngle.SetData(Vector3(0, 0, 0), name, "Angle", -359, 359);
}

void MelLib::Camera::Finalize()
{
	pCameras.clear();
}

void MelLib::Camera::AllCalcCameraData()
{
	for (auto& camera : pCameras) 
	{
		camera.second->CalcCameraData();
	}
}

void Camera::Create(const std::string& name)
{
	std::string key = name;
	if (key == "")key = "Camera_" + std::to_string(createCount);
	createCount++;

	pCameras.emplace(key, std::make_unique<Camera>(key));

	if (mainCameraName == "")mainCameraName = name;

}

void Camera::Delete(const std::string& name)
{
	pCameras.erase(name);
}

void MelLib::Camera::GetCameraNames(std::vector<std::string>& names)
{
	names.clear();
	names.reserve(pCameras.size());

	for (const auto& pCamera : pCameras) 
	{
		names.push_back(pCamera.first);
	}
}


void Camera::SetRotateCriteriaPosition(const Vector3& position)
{
	this->rotatePointPosition = position;
	CalcCameraData();
	//��
	Input::SetViewMatrix(GetViewMatrix());
	Input::SetProjectionMatrix(GetProjectionMatrix());

}

void Camera::SetAngle(const Vector3& angle)
{
	this->angle = angle;
	CalcCameraData();
	//��
	Input::SetViewMatrix(GetViewMatrix());
	Input::SetProjectionMatrix(GetProjectionMatrix());
}

void Camera::SetCameraToTargetDistance(const float distance)
{
	cameraToTargetDistance = distance;
	CalcCameraData();
}

void Camera::SetRotatePoint(const RotatePoint rotatePoint)
{
	this->rotatePoint = rotatePoint;
	CalcCameraData();
}

DirectX::XMMATRIX Camera::GetViewMatrix() const
{
	DirectX::XMMATRIX matView = DirectX::XMMatrixLookAtLH
	(
		DirectX::XMLoadFloat3(&cameraPosition.ToXMFLOAT3()),
		DirectX::XMLoadFloat3(&targetPosition.ToXMFLOAT3()),
		DirectX::XMLoadFloat3(&upVector.ToXMFLOAT3())
	);
	return matView;
}

DirectX::XMMATRIX Camera::GetProjectionMatrix() const
{

	DirectX::XMMATRIX mapProjection = DirectX::XMMatrixPerspectiveFovLH
	(
		DirectX::XMConvertToRadians(fovY), //��p
		(float)Library::GetWindowWidth() / (float)Library::GetWindowHeight(),//�A�X�y�N�g��
		nearNum, farNum//�`��͈�
	);
	return mapProjection;
}

DirectX::XMMATRIX Camera::GetViewAndProjectionMat()const
{
	return GetViewMatrix() * GetProjectionMatrix();
}

Vector3 MelLib::Camera::GetRotateCriteriaPosition() const
{
	if (rotatePoint == RotatePoint::ROTATE_POINT_CAMERA_POSITION)return cameraPosition;
	return targetPosition;
}
