#pragma once
#include"Vector.h"
#include"GuiValue.h"

#include<vector>
#include<string>
#include<memory>
#include<unordered_map>

//#include"Input.h"

namespace MelLib
{
	// �J�����ƕǂ������������m�F���邽�߂ɂ�GameObject�N���X���p�����Ȃ��Ƃ����Ȃ�
	// �J������������GameObject�N���X�����Όp�����Ȃ��Ă����v
	// ������������W�Ƃ�2�ɂȂ邯�ǒ������₷��������Ń��f���Ƃ����������Ă邵�����񂶂�Ȃ���

	class Camera
	{
	public:

		//��]������Ƃ��̊�ƂȂ�ʒu
		enum class RotatePoint
		{
			ROTATE_POINT_CAMERA_POSITION,//�J�����̍��W�𒆐S�ɒ����_����]
			ROTATE_POINT_TARGET_POSITION,//�����_�̍��W�𒆐S�ɃJ�������W����]
		};

	private:
		using UINT = unsigned int;

		static std::unordered_map<std::string, std::unique_ptr<Camera>>pCameras;
		static UINT createCount;
		static std::string mainCameraName;

		//��]�
		RotatePoint rotatePoint = RotatePoint::ROTATE_POINT_CAMERA_POSITION;


		//��p
		float fovY = 60.0f;
		//�ŋߓ_
		float nearNum = 0.01f;
		//�ŉ��_
		float farNum = 1000.0f;

		//�J�������W���璍���_�̋���
		float cameraToTargetDistance = 1.0f;

		//��]������Ƃ��̊�ʒu
		Vector3& rotatePointPosition;
		Vector3& angle;

		GuiVector3 guiRotatePointPosition;
		GuiVector3 guiAngle;


		Vector3 cameraPosition = rotatePointPosition;
		Vector3 targetPosition = rotatePointPosition + Vector3(0, 0, cameraToTargetDistance);
		Vector3 upVector = Vector3(0, 1, 0);

		std::string cameraName;

		void CalcCameraData();

	public:
		Camera();
		Camera(const std::string& name);
		~Camera();

		static void AllCalcCameraData();

		static void Finalize();

		/// <summary>
		/// �J�����𐶐����܂��B
		/// </summary>
		/// <param name="name"></param>
		static void Create(const std::string& name = "");

		/// <summary>
		/// �J�������폜���܂��B
		/// </summary>
		/// <param name="name"></param>
		static void Delete(const std::string& name);

		/// <summary>
		/// �J�����̃|�C���^���擾���܂��B
		/// </summary>
		/// <param name="name">�J�����̖��O</param>
		/// <returns></returns>
		static Camera* Get(const std::string& name = mainCameraName) { return pCameras[name].get(); }

		static void GetCameraNames(std::vector<std::string>& names);
	


#pragma region �Z�b�g

		/// <summary>
		/// �J��������]������Ƃ��Ɋ�ƂȂ���W���Z�b�g���܂��B
		/// </summary>
		/// <param name="position">��]������Ƃ��Ɋ�ƂȂ���W</param>
		void SetRotateCriteriaPosition(const Vector3& position);


		/// <summary>
		/// �p�x���Z�b�g���܂��B�p�x��(0,0,0)�̏ꍇ�A�J������0,0,1�̕����������܂��B
		/// </summary>
		/// <param name="angle">�J�����̊p�x</param>
		void SetAngle(const Vector3& angle);

		/// <summary>
		/// ��p���Z�b�g���܂��B
		/// </summary>
		/// <param name="fovY">��p</param>
		void SetFovY(const float fovY) { this->fovY = fovY; }

		/// <summary>
		/// �J��������J�����̕\���͈͂̈�ԋ߂��ꏊ�܂ł̋������Z�b�g���܂��B
		/// </summary>
		/// <param name="num">near�l</param>
		void SetNear(const float num)
		{
			nearNum = num;
			//Input::SetNear(nearNum);
		}

		/// <summary>
		/// �J��������J�����̕\���͈͂̈�ԉ����ꏊ�܂ł̋������Z�b�g���܂��B
		/// </summary>
		/// <param name="num">far�l</param>
		void SetFar(const float num)
		{
			farNum = num;
			//Input::SetFar(farNum);
		}

		/// <summary>
		/// �J�����ƒ����_�̋������Z�b�g���܂��B�����l��1.0f�ł��B
		/// </summary>
		/// <param name="distance">�J�����ƒ����_�̋���</param>
		void SetCameraToTargetDistance(const float distance);

		/// <summary>
		/// �J��������]������Ƃ��A�ǂ�����ɉ�]�����邩�����߂܂��B
		/// </summary>
		/// <param name="rotatePoint"></param>
		void SetRotatePoint(const RotatePoint rotatePoint);

#pragma endregion

#pragma region �Q�b�g


		/// <summary>
		/// ���C���J����(���C�u�����ŗp�ӂ��ꂽ�J����)�̖��O���擾���܂��B
		/// </summary>
		/// <returns></returns>
		static const std::string& GetMainCameraName() { return mainCameraName; }

		DirectX::XMMATRIX GetViewMatrix()const;
		DirectX::XMMATRIX GetProjectionMatrix()const;

		/// <summary>
		/// �r���[�s��ƃv���W�F�N�V�����s����|�����s����擾���܂��B
		/// </summary>
		/// <returns></returns>
		DirectX::XMMATRIX GetViewAndProjectionMat()const;

		/// <summary>
		/// �J�����̍��W���擾���܂��B
		/// </summary>
		/// <returns>�J�����̍��W</returns>
		Vector3 GetCameraPosition()const { return cameraPosition; }

		/// <summary>
		/// �����_�̍��W���擾���܂��B
		/// </summary>
		/// <returns>�����_�̍��W</returns>
		Vector3 GetTargetPosition()const { return targetPosition; }

		Vector3 GetRotateCriteriaPosition()const;

		/// <summary>
		/// ��x�N�g�����擾���܂��B
		/// </summary>
		/// <returns>��x�N�g��</returns>
		Vector3 GetUpVector()const { return upVector; }

		Vector3 GetAngle()const { return angle; }

		std::string GetCameraName()const { return cameraName; }

		float GetCameraToTargetDistance()const { return cameraToTargetDistance; }
#pragma endregion
	};
}
