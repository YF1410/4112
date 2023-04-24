#pragma once
#include<d2d1.h>
#pragma comment(lib,"D2d1.lib")
#include<d2d1_3.h>
#include<dwrite.h>
#pragma comment(lib,"DWrite.lib")

#pragma once
#include<d3d12.h>
#include<d3dx12.h>
#include<d3d11.h>
#include<d3d11on12.h>
#pragma comment(lib,"d3d11.lib")
#include<dxgi.h>
#pragma comment(lib,"dxgi.lib")

#include<wrl.h>
#include<string>
#include<unordered_map>

#include"Vector.h"
#include"Color.h"



//DirectWrite�`���[�g���A��URL
//https://docs.microsoft.com/ja-jp/windows/win32/directwrite/getting-started-with-directwrite#part-1-declare-directwrite-and-direct2d-resources

//pTextFormat�͕ʃN���X�ɂ���?
//�t�H���g�N���X�ƃe�L�X�g�`��N���X�𕪂���?
//�Ƃ肠�����`��܂ł���Ă݂Ă���݌v�l����

//D3D11On12���g��Ȃ��Ɩ������ۂ�
//�g�p��̂Ƃ���ɂ��������Ă�
//https://docs.microsoft.com/en-us/windows/win32/direct3d12/direct3d-11-on-12#initializing-d3d11on12


//11on12���g�p����D2D
//https://docs.microsoft.com/ja-jp/windows/win32/direct3d12/d2d-using-d3d11on12

using namespace Microsoft::WRL;

namespace MelLib
{
	//DirectWrite���g�p�����e�L�X�g�`��N���X
	class TextWrite
	{
	public:

		//enum class TextAlignment
		//{
		//	LEFT_JUSTIFIED,//������
		//	RIGHT_JUSTIFIED,//�E����
		//	POINT//���S����
		//};
		struct FontName
		{
			const std::wstring ARIAL = L"Arial";
			const std::wstring HGP_GOSIC_E = L"HGP�޼��E";
		};


	private:
		static std::vector<std::wstring>tests;
		static std::vector<std::string>fontNames;
		static std::vector<std::tuple<Vector2, Color,/* TextAlignment,*/std::wstring, std::string>>drawTextDatas;

		static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);


		//DirectWrite���g�p���邽�߂̂���
		static ComPtr<IDWriteFactory> dWriteFactory;
		static std::unordered_map<std::string, ComPtr<IDWriteTextFormat>>pTextFormat;

		//11�̃f�o�C�X
		static ComPtr<ID3D11Device>d3d11Device;
		static ComPtr<ID3D11DeviceContext>d3d11context;
		static ComPtr<ID3D11On12Device>d3d11On12device;
		//�����_�����O����o�b�t�@
		static ComPtr<ID3D11Resource>wrappedBackBuffer[2];

		//D2D
		static ComPtr<ID2D1Factory3> d2dFactory;
		static ComPtr<ID2D1Device2>d2dDevice;
		static ComPtr<IDXGIDevice>dxgiDevice;
		static ComPtr<ID2D1DeviceContext> d2dContext;

		//�o�C���h����Ă���r�b�g�}�b�v��\��
		static ComPtr<ID2D1Bitmap1>d2dRenderTerget[2];

		//�����_�����O���邽�߂̂���
		static ComPtr<ID2D1HwndRenderTarget> d2dRenderTarget;
		//�̈��h��Ԃ�����(���������ăt�H���g�̓h��Ԃ��Ɏg��?)
		static ComPtr<ID2D1SolidColorBrush> d2dSolidColorBrush;

		static HWND hwnd;


	public:
		static bool Initialize
		(
			ID3D12Device* pD3D12Device,
			ID3D12CommandQueue** pPD3D12Queue,
			ID3D12Resource* pBuckBuffer[2]
		);

		static void LoopStartProcess();
		static void LoopEndProcess(const UINT rtIndex);

		//�t�H���g�̃f�[�^���Z�b�g����֐����?
		static bool CreateFontData(const std::wstring& fontName,float size, const std::string& name);


		//�����T�C�Y�͍s��ݒ�ł��邩�炻��ŕς���悤�ɂ���?

		static void Draw
		(
			const Vector2& position,
			const Color& color,
			/*	const TextAlignment allgnment,*/
			const std::wstring& text,
			const std::string& fontName
		);
	};
}


