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

	//Imguiを管理するクラス
	// このクラスいらないのでは
	// フラグはライブラリ側で管理すればいいし
	// テキストボックスとかあった方がいいから一応残しとく?
	// 描画準備とかの処理もあるから残しといていいかも
	// Vectorも使うの楽だし
	// ImGuiって初期化してない状態で関数呼び出すとどうなるんだっけ。例外出る?
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
		/// リリースかつreleaseDrawFlagがfalseだったらtrue
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
		/// ウィンドウを描画する準備を行います。
		/// </summary>
		/// <param name="name">ウィンドウ名</param>
		/// <param name="pos">座標</param>
		/// <param name="size">サイズ</param>
		void BeginDrawWindow(const std::string& name);

		/// <summary>
		/// 描画処理を終了します。BeginDrawWindowを呼び出し、ウィンドウの項目の設定を行った後に必ず呼び出してください。
		/// 複数ウィンドウを作成する場合、各ウィンドウの項目の設定が終るごとに呼び出してください。
		/// </summary>
		void EndDrawWindow();

		/// <summary>
		/// リリース時にウィンドウを描画するかを設定します。設定する場合は、ライブラリの初期化より前で呼び出してください。
		/// </summary>
		/// <param name="flag"></param>
		void SetReleaseDrawFlag(const bool flag) { releaseDrawFlag = flag; }

		void SetPosition(const Vector2& pos);
		void SetSize(const Vector2& size);

		void SameLine();

#pragma region 描画

		bool DrawButton(const std::string& lavel,const Vector2& size = 0);

		/// <summary>
		/// ラジオボタンをウィンドウに表示します。
		/// </summary>
		/// <param name="label">ラベル名(チェックボックスの隣に表示される文字)</param>
		/// <param name="pInt">選択されているときにnumの値を代入する変数のポインタ</param>
		/// <param name="num">選択されているときにpIntが参照している変数に代入する値</param>
		/// <returns>現在のフレームで選択されたらtrue(正確には、選択されて値の代入処理が行われたらtrue)</returns>
		bool DrawRadioButton(const std::string& label, int& refInt, const int num);

		/// <summary>
		/// チェックボックスをウィンドウに表示します。
		/// </summary>
		/// <param name="label">ラベル名(チェックボックスの隣に表示される文字)</param>
		/// <param name="flag">押されているかどうかのフラグを代入する変数のポインタ</param>
		/// <returns></returns>
		bool DrawCheckBox(const std::string& label, bool& refBool);


		
		/// <summary>
		/// リストを表示します。
		/// </summary>
		/// <param name="num">どれが選ばれているかを表す数値を代入する変数の参照</param>
		/// <param name="texts">表示するテキストの配列</param>
		void DrawList(int& num,const std::vector<std::string>& texts);


#pragma region スライダー


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

#pragma region カラー


		bool DrawColorPicker(const std::string& label, Color& refColor,const ImGuiColorEditFlags flag = 0);
#pragma endregion

#pragma region 入力
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

