#pragma once
#include<d3d12.h>
#include<wrl.h>

#include<string>

#include"Vector.h"
#include"Color.h"
#include"Values.h"

#include"imgui/imgui.h"


namespace MelLib 
{

	//Imgui���Ǘ�����N���X
	// ���̃N���X����Ȃ��̂ł�
	// �t���O�̓��C�u�������ŊǗ�����΂�����
	// �e�L�X�g�{�b�N�X�Ƃ�������������������ꉞ�c���Ƃ�?
	// �`�揀���Ƃ��̏��������邩��c���Ƃ��Ă�������
	// Vector���g���̊y����
	// ImGui���ď��������ĂȂ���ԂŊ֐��Ăяo���Ƃǂ��Ȃ�񂾂����B��O�o��?
	class ImguiManager
	{
	public:

	private:
		template<class T>
		using ComPtr = Microsoft::WRL::ComPtr<T>;

		ComPtr<ID3D12DescriptorHeap>desHeap = nullptr;

		bool releaseDrawFlag = true;

		ID3D12Device* pDevice = nullptr;
		ID3D12GraphicsCommandList* pCmdList = nullptr;

		
	private:
		ImguiManager() {}
		~ImguiManager() {}

		/// <summary>
		/// �����[�X����releaseDrawFlag��false��������true
		/// </summary>
		/// <returns></returns>
		bool CheckReleaseDrawFlag();
	public:
		ImguiManager(ImguiManager& i) = delete;
		ImguiManager& operator=(ImguiManager& i) = delete;
		static ImguiManager* GetInstance();

		bool Initialize(ID3D12Device* pDev, ID3D12GraphicsCommandList* pCmdList);
		void Finalize();
		void Draw();

		void Begin();
		
		/// <summary>
		/// �E�B���h�E��`�悷�鏀�����s���܂��B
		/// </summary>
		/// <param name="name">�E�B���h�E��</param>
		/// <param name="pos">���W</param>
		/// <param name="size">�T�C�Y</param>
		void BeginDrawWindow(const std::string& name);

		/// <summary>
		/// �`�揈�����I�����܂��BBeginDrawWindow���Ăяo���A�E�B���h�E�̍��ڂ̐ݒ���s������ɕK���Ăяo���Ă��������B
		/// �����E�B���h�E���쐬����ꍇ�A�e�E�B���h�E�̍��ڂ̐ݒ肪�I�邲�ƂɌĂяo���Ă��������B
		/// </summary>
		void EndDrawWindow();

		/// <summary>
		/// �����[�X���ɃE�B���h�E��`�悷�邩��ݒ肵�܂��B�ݒ肷��ꍇ�́A���C�u�����̏��������O�ŌĂяo���Ă��������B
		/// </summary>
		/// <param name="flag"></param>
		void SetReleaseDrawFlag(const bool flag) { releaseDrawFlag = flag; }

		void SetPosition(const Vector2& pos);
		void SetSize(const Vector2& size);

		void SameLine();

#pragma region �`��

		bool DrawButton(const std::string& lavel,const Vector2& size = 0);

		/// <summary>
		/// ���W�I�{�^�����E�B���h�E�ɕ\�����܂��B
		/// </summary>
		/// <param name="label">���x����(�`�F�b�N�{�b�N�X�ׂ̗ɕ\������镶��)</param>
		/// <param name="pInt">�I������Ă���Ƃ���num�̒l��������ϐ��̃|�C���^</param>
		/// <param name="num">�I������Ă���Ƃ���pInt���Q�Ƃ��Ă���ϐ��ɑ������l</param>
		/// <returns>���݂̃t���[���őI�����ꂽ��true(���m�ɂ́A�I������Ēl�̑���������s��ꂽ��true)</returns>
		bool DrawRadioButton(const std::string& label, int& refInt, const int num);

		/// <summary>
		/// �`�F�b�N�{�b�N�X���E�B���h�E�ɕ\�����܂��B
		/// </summary>
		/// <param name="label">���x����(�`�F�b�N�{�b�N�X�ׂ̗ɕ\������镶��)</param>
		/// <param name="flag">������Ă��邩�ǂ����̃t���O��������ϐ��̃|�C���^</param>
		/// <returns></returns>
		bool DrawCheckBox(const std::string& label, bool& refBool);


		
		/// <summary>
		/// ���X�g��\�����܂��B
		/// </summary>
		/// <param name="num">�ǂꂪ�I�΂�Ă��邩��\�����l��������ϐ��̎Q��</param>
		/// <param name="texts">�\������e�L�X�g�̔z��</param>
		void DrawList(int& num,const std::vector<std::string>& texts);


#pragma region �X���C�_�[


		bool DrawSliderInt(const std::string& label, int& refInt, const int numMin, const int numMax);
		bool DrawSliderInt2(const std::string& label, Value2<int>& refInt, const int numMin, const int numMax);
		bool DrawSliderInt3(const std::string& label, Value3<int>& refInt, const int numMin, const int numMax);
		bool DrawSliderInt4(const std::string& label, Value4<int>& refInt, const int numMin, const int numMax);

		bool DrawSliderFloat(const std::string& label, float& refFloat, const float numMin, const float numMax);
		bool DrawSliderFloat2(const std::string& label, Value2<float>& refFloat, const float numMin, const float numMax);
		bool DrawSliderFloat3(const std::string& label, Value3<float>& refFloat, const float numMin, const float numMax);
		bool DrawSliderFloat4(const std::string& label, Value4<float>& refFloat, const float numMin, const float numMax);

		bool DrawSliderVector2(const std::string& label, Vector2& refVec2, const float numMin, const float numMax);
		bool DrawSliderVector3(const std::string& label, Vector3& refVec3, const float numMin, const float numMax);

#pragma endregion

#pragma region �J���[


		bool DrawColorPicker(const std::string& label, Color& refColor,const ImGuiColorEditFlags flag = 0);
#pragma endregion

#pragma region ����
		bool DrawTextBox(const std::string& label, std::string& text, const size_t maxCharNum,const ImGuiInputTextFlags flag = 0);

		bool DrawInputIntBox
		(
			const std::string& label,
			int& num,
			const int min = 0.0f,
			const int max = 0.0f,
			const ImGuiInputTextFlags flag = 0
		);

		bool DrawInputFloatBox
		(
			const std::string& label ,
			float& num, 
			const float min = 0.0f,
			const float max = 0.0f,
			const std::string& format = "%.3f",
			const ImGuiInputTextFlags flag = 0
		);

		bool DrawInputVector3Box
		(
			const std::string& label,
			Vector3& num,
			const std::string& format = "%.3f",
			const ImGuiInputTextFlags flag = 0
		);

#pragma endregion


#pragma endregion

		bool GetReleaseDrawFrag()const { return releaseDrawFlag; }
	};
}

