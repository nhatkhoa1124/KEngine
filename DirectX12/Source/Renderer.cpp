#include "dx12pch.h"
#include <cassert>

namespace KRender
{
	Renderer::Renderer() :
		mRtvDescriptorSize{ 0 },
		mDsvDescriptorSize{ 0 },
		mCbvDescriptorSize{ 0 },
		m4xMsaaQualityLevel{ 0 }
	{

	}

	Renderer::~Renderer()
	{

	}

	void Renderer::CreateDevice()
	{
		HRESULT hr = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&mDevice));

		if (FAILED(hr))
		{
			ComPtr<IDXGIAdapter> warpAdapter;
			ThrowIfFailed(mDxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter)));
			ThrowIfFailed(D3D12CreateDevice(warpAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&mDevice)));
		}

	}

	void Renderer::CreateFence()
	{
		ThrowIfFailed(mDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mFence)));
	}

	void Renderer::CreateCommandObjects()
	{
		D3D12_COMMAND_QUEUE_DESC queueDesc;
		queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		ThrowIfFailed(mDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&mCommandQueue)));
		ThrowIfFailed(mDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&mCommandAllocator)));
		ThrowIfFailed(mDevice->CreateCommandList(
			0,
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			mCommandAllocator.Get(),
			nullptr,
			IID_PPV_ARGS(&mCommandList)));

		mCommandList->Close();
	}

	void Renderer::CreateSwapChain()
	{
		DXGI_SWAP_CHAIN_DESC scDesc;

		scDesc.BufferDesc.Width = mClientWidth;

	}

	void Renderer::SetupDescriptorSize()
	{
		mRtvDescriptorSize = mDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		mDsvDescriptorSize = mDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
		mCbvDescriptorSize = mDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	}

	void Renderer::CreateFactory()
	{
		ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&mDxgiFactory)));
	}

	void Renderer::CheckMSAASupport4X()
	{
		D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevel;
		msQualityLevel.Format = mBackBufferFormat;
		msQualityLevel.SampleCount = 4;
		msQualityLevel.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
		msQualityLevel.NumQualityLevels = 0;
		ThrowIfFailed(mDevice->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &msQualityLevel, sizeof(msQualityLevel)));

		m4xMsaaQualityLevel = msQualityLevel.NumQualityLevels;
		assert(m4xMsaaQualityLevel > 0);
	}
}