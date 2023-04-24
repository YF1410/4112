#include "DirectX12.h"

#include<fstream>

#include"ErrorProcess.h"

#include"LibMath.h"
#include"FbxLoader.h"
#include"ImguiManager.h"

#include"DirectionalLight.h"
#include"RenderTarget.h"

#include"ModelData.h"
#include"ModelObject.h"
#include"TextWrite.h"
#include"Material.h"

#include"DrawManager.h"


using namespace MelLib;


DirectX12::DirectX12()
{
}


DirectX12::~DirectX12()
{


	//#ifdef _DEBUG
	//
	//	ComPtr<ID3D12DebugDevice> debugDevice;
	//	//�f�o�b�O�f�o�C�X�̍쐬
	//	dev->QueryInterface(IID_PPV_ARGS(&debugDevice));
	//	//�o��
	//	debugDevice->ReportLiveDeviceObjects(D3D12_RLDO_SUMMARY);
	//#endif // _DEBUG


}

DirectX12* DirectX12::GetInstance()
{
	static DirectX12 dx12;
	return &dx12;
}

void DirectX12::Initialize(HWND hwnd, int windouWidth, int windowHeight)
{



#pragma region �ϐ�������

	
#pragma region Windows

	this->hwnd = hwnd;
	winWidth = windouWidth;
	winHeight = windowHeight;

#pragma endregion


	heapDesc = {};
	barrierDesc = {};


	
#pragma endregion


#pragma region �f�o�b�O���C���[
#ifdef _DEBUG
	ComPtr<ID3D12Debug> debugController;

	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
	}
	else
	{
		ErrorProcess::GetInstance()->StartErroeProcess(L"�f�o�b�O���C���[�̐����Ɏ��s�B", true);
	}

#endif // _DEBUG

#pragma endregion

#pragma region DirectX����

	result = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));
	if(result != S_OK)ErrorProcess::GetInstance()->StartErroeProcess(L"DXGIFactory�̐����Ɏ��s�B",true);
#pragma region �A�_�v�^�̗�

	std::vector<IDXGIAdapter*> adapters;
	IDXGIAdapter* tmpAdapter = nullptr;
	for (int i = 0; dxgiFactory->EnumAdapters(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND; i++)
	{
		adapters.push_back(tmpAdapter);
	}

	std::vector<std::wstring> strDescs(adapters.size());
	bool useOnboard = true;
	for (int i = 0; i < (int)adapters.size(); i++)
	{
		DXGI_ADAPTER_DESC adesc{};
		adapters[i]->GetDesc(&adesc);
		strDescs[i] = adesc.Description;

		if (strDescs[i].find(L"Microsoft") == std::wstring::npos)
		{
			useOnboard = false;
		}
	}

	for (int i = 0; i < (int)adapters.size(); i++)
	{
		if (strDescs[i].find(L"Microsoft") != std::wstring::npos && useOnboard)
		{
			tmpAdapter = adapters[i];
			break;
		}
		else
		{
			tmpAdapter = adapters[i];
			break;
		}
	}

	if(!tmpAdapter)ErrorProcess::GetInstance()->StartErroeProcess(L"�A�_�v�^�[��������܂���ł����B",true);





#pragma endregion

#pragma region �f�o�C�X�̐���

	D3D_FEATURE_LEVEL levels[] =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	D3D_FEATURE_LEVEL featureLevel;

	for (int i = 0; i < _countof(levels); i++)
	{
		result = D3D12CreateDevice(tmpAdapter, levels[i], IID_PPV_ARGS(&dev));
		if (result == S_OK)
		{
			featureLevel = levels[i];
			break;
		}
	}


	if (dev != nullptr)
	{
		CreateBuffer::GetInstance()->Initialize(dev.Get(), windouWidth, windowHeight);
	
	}
	else if (!tmpAdapter)ErrorProcess::GetInstance()->StartErroeProcess(L"�f�o�C�X�̐����Ɏ��s�B",true);

	tmpAdapter->Release();




#pragma endregion

#pragma region �A���P�[�^�[ ���X�g����
	result = dev->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&cmdAllocator));
	if(result != S_OK)ErrorProcess::GetInstance()->StartErroeProcess(L"�R�}���h�A���P�[�^�[�̐����Ɏ��s�B",true);

	result = dev->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, cmdAllocator.Get(), nullptr, IID_PPV_ARGS(&cmdList));
	if(result != S_OK)ErrorProcess::GetInstance()->StartErroeProcess(L"�R�}���h���X�g�̐����Ɏ��s�B",true);
#pragma endregion

#pragma region �R�}���h�L���[�쐬
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};
	result = dev->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&cmdQueue));
	if(result != S_OK)ErrorProcess::GetInstance()->StartErroeProcess(L"�R�}���h�L���[�̐����Ɏ��s�B",true);

#pragma endregion

#pragma region �X���b�v�`�F�[���̐���
	ComPtr<IDXGISwapChain1>swapchain1;
	DXGI_SWAP_CHAIN_DESC1 swapchainDesc{};
	swapchainDesc.Width = winWidth;
	swapchainDesc.Height = winHeight;
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapchainDesc.SampleDesc.Count = 1;
	swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
	swapchainDesc.BufferCount = 2;
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	result = dxgiFactory->CreateSwapChainForHwnd(
		cmdQueue.Get(),
		hwnd,
		&swapchainDesc,
		nullptr,
		nullptr,
		&swapchain1
	);
	swapchain1.As(&swapchain);

	if(result != S_OK)ErrorProcess::GetInstance()->StartErroeProcess(L"�X���b�v�`�F�[���̐����Ɏ��s�B",true);

#pragma endregion

#pragma region �o�b�N�o�b�t�@�֌W

	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	heapDesc.NumDescriptors = 2;
	dev->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&rtvHeaps));

	for (int i = 0; i < 2; i++)
	{
		result = swapchain->GetBuffer(i, IID_PPV_ARGS(&backBuffer[i]));
		D3D12_CPU_DESCRIPTOR_HANDLE handle = rtvHeaps->GetCPUDescriptorHandleForHeapStart();
		handle.ptr += i * dev->GetDescriptorHandleIncrementSize(heapDesc.Type);
		dev->CreateRenderTargetView
		(
			backBuffer[i].Get(),
			nullptr,
			handle
		);

		if (result != S_OK)ErrorProcess::GetInstance()->StartErroeProcess(L"�o�b�N�o�b�t�@�̐����Ɏ��s�B",true);
	}

#pragma endregion

#pragma region �[�x�o�b�t�@
	//�[�x�p�q�[�v�쐬
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	result = dev->CreateDescriptorHeap
	(
		&dsvHeapDesc,
		IID_PPV_ARGS(&depthHeap)
	);
	if (result != S_OK)ErrorProcess::GetInstance()->StartErroeProcess(L"�[�x�o�b�t�@�̐����Ɏ��s�B",true);

	D3D12_CPU_DESCRIPTOR_HANDLE depthHeapHandle = depthHeap->GetCPUDescriptorHandleForHeapStart();


	D3D12_RESOURCE_DESC depthResDesc = CD3DX12_RESOURCE_DESC::Tex2D
	(
		DXGI_FORMAT_D32_FLOAT,
		winWidth,
		winHeight,
		1, 0, 1, 0,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
	);

	CreateBuffer::GetInstance()->CreateDepthBufferSet(CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), depthResDesc, CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0), depthHeapHandle, depthBufferSet);


#pragma endregion

#pragma region �t�F���X�쐬
	fence = nullptr;
	fenceVal = 0;
	result = dev->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	if (result != S_OK)ErrorProcess::GetInstance()->StartErroeProcess(L"�t�F���X�̐����Ɏ��s�B",true);
#pragma endregion



#pragma endregion



	FbxLoader::GetInstance()->Initialize(dev.Get());
	PipelineState::Initialize(dev.Get());
	//Model::Initialize
	//(
	//	dev.Get(),
	//	cmdLists
	//);

	//
	//ObjModel::Initialize();
	//FbxModel::Initialize();
	//PrimitiveModel::Initialize();

	std::vector<ID3D12GraphicsCommandList*> cmdLists(1, cmdList.Get());

	Material::Initialize(dev.Get());
	ModelObject::Initialize(dev.Get(), cmdLists);
	ModelData::Initialize(dev.Get());

	Sprite::Initialize(dev.Get(), cmdList.Get());
	Sprite2DBase::Initialize(windouWidth, windowHeight);
	Sprite3D::Initialize();
	RenderTarget::Initialize(dev.Get(), cmdList.Get());

	Color rtColor =
		Color
		(
			Color::ParToUChar(clearColor[0] * 100.0f),
			Color::ParToUChar(clearColor[1] * 100.0f),
			Color::ParToUChar(clearColor[2] * 100.0f),
			Color::ParToUChar(clearColor[3] * 100.0f)
		);
	
	Camera::Create("main");
	RenderTarget::Create(rtColor, "main");
	RenderTarget::Get()->SetCamera(Camera::Get());
	DirectionalLight::Create("main");

	//renderTarget = std::make_unique<RenderTarget>(Color(255, 0, 255, 255));



	ID3D12Resource* pBackBuffer[] = { backBuffer[0].Get(),backBuffer[1].Get() };
	TextWrite::Initialize(dev.Get(), cmdQueue.GetAddressOf(), pBackBuffer);

	ImguiManager::GetInstance()->Initialize(dev.Get(), cmdList.Get());


#pragma region �e�X�g�p
	//PipelineState postEffectTestPipeline;

	//PipelineData data;
	//data.alphaWriteMode = ALPHA_WRITE_TRUE;
	//data.blendMode = BlendMode::ADD;
	//data.cullMode = CullMode::NONE;
	//data.depthMode = DEPTH_FALSE;
	//data.drawMode = DrawMode::SOLID;
	//auto result = postEffectTestPipeline.CreatePipeline
	//(
	//	data,
	//	{ L"../MyLibrary/PostEffectTestVertexShader.hlsl","VSmain","vs_5_0" },
	//	{ L"NULL","","" },
	//	{ L"NULL","","" },
	//	{ L"NULL","","" },
	//	{ L"../MyLibrary/PostEffectTestPixelShader.hlsl","PSmain","ps_5_0" },
	//	PipelineType::RENDER_TARGET,
	//	nullptr,
	//	typeid(RenderTarget).name(),
	//	1
	//);
	//RenderTarget::Get("main")->SetPipeline(&postEffectTestPipeline);
	//renderTarget->SetPipeline(&postEffectTestPipeline);
#pragma endregion

}

void DirectX12::LoopStartProcess()
{

#pragma region �o���A����_PSResource����RTV��

	UINT bbIndex = swapchain->GetCurrentBackBufferIndex();

	/*barrierDesc.Transition.pResource = backBuffer[bbIndex].Get();
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;*/

	//�|���̏�Ԃ��ARESOURCE����RT�ɐ؂�ւ�
	/*barrierDesc.Transition.pResource = postEffectResources[0].Get();
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

	cmdList->ResourceBarrier(1, &barrierDesc);*/
#pragma endregion

	//RenderTarget::Get("main")->PreDrawProcess();
	RenderTarget::DrawBegin();


	TextWrite::LoopStartProcess();
	ImguiManager::GetInstance()->Begin();
}

void DirectX12::LoopEndProcess()
{
	RenderTarget::AllDraw();
	//�|�����o�b�N�o�b�t�@�[�ɕ`�悷�鏀��
	UINT bbIndex = swapchain->GetCurrentBackBufferIndex();
	barrierDesc.Transition.pResource = backBuffer[bbIndex].Get();
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	cmdList->ResourceBarrier(1, &barrierDesc);

	D3D12_CPU_DESCRIPTOR_HANDLE rtvH = rtvHeaps->GetCPUDescriptorHandleForHeapStart();
	rtvH.ptr += bbIndex * dev->GetDescriptorHandleIncrementSize(heapDesc.Type);
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = depthHeap.Get()->GetCPUDescriptorHandleForHeapStart();
	cmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);

	//��ʂ̃N���A
	cmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);
	cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	
	RenderTarget::MainRTDraw();

	ImguiManager::GetInstance()->Draw();
#pragma region RTV����PRESENT��
	//TextWrite�N���X�Ŏ����I�ɕύX���邩�炢��Ȃ�

	/*barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	cmdList->ResourceBarrier(1, &barrierDesc);*/

	//barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	//barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	//cmdList->ResourceBarrier(1, &barrierDesc);
#pragma endregion



#pragma region ���s


	cmdList->Close();
	ID3D12CommandList* cmdLists[] = { cmdList.Get() };
	cmdQueue->ExecuteCommandLists(_countof(cmdLists), cmdLists);
	

	//�e�L�X�g�`��
	TextWrite::LoopEndProcess(bbIndex);

	cmdQueue->Signal(fence.Get(), ++fenceVal);
	if (fence->GetCompletedValue() != fenceVal)
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence->SetEventOnCompletion(fenceVal, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}


	cmdAllocator->Reset();
	cmdList->Reset(cmdAllocator.Get(), nullptr);

	//(VSYNC,�萔)
	swapchain->Present(1, 0);
#pragma endregion
	DrawManager::GetInstance()->Reset();
}

void DirectX12::Finalize()
{
	LoopEndProcess();
	FbxLoader::GetInstance()->Finalize();
	ImguiManager::GetInstance()->Finalize();
}


void DirectX12::SetScreenColor(Color screenColor)
{
	clearColor[0] = (float)screenColor.r / 255;
	clearColor[1] = (float)screenColor.g / 255;
	clearColor[2] = (float)screenColor.b / 255;
	clearColor[3] = (float)screenColor.a / 255;
}
