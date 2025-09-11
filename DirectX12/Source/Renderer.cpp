#include "dx12pch.h"
#include <cassert>

namespace KRender
{
	Renderer::Renderer(HWND handle) :
		mRtvDescriptorSize{ 0 },
		mDsvDescriptorSize{ 0 },
		mCbvDescriptorSize{ 0 },
		m4xMsaaQualityLevel{ 0 },
		mWinHandle{ handle },
		mIsWindowed{ true },
		m4xMsaaState{ false }
	{
		mSwapChainBuffers.resize(mSwapChainBufferCount);
		mScissorRects.resize(1);
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
		scDesc.BufferDesc.Width = CLIENT_WIDTH;
		scDesc.BufferDesc.Height = CLIENT_HEIGHT;
		scDesc.BufferDesc.Format = mBackBufferFormat;
		scDesc.BufferDesc.RefreshRate.Numerator = 72;
		scDesc.BufferDesc.RefreshRate.Denominator = 1;
		scDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		scDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		scDesc.SampleDesc.Count = m4xMsaaState ? 4 : 1;
		scDesc.SampleDesc.Quality = m4xMsaaState ? (m4xMsaaQualityLevel - 1) : 0;
		scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scDesc.BufferCount = mSwapChainBufferCount;
		scDesc.OutputWindow = mWinHandle;
		scDesc.Windowed = mIsWindowed;
		scDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		scDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		ThrowIfFailed(mDxgiFactory->CreateSwapChain(mDevice.Get(), &scDesc, mSwapChain.GetAddressOf()));

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

	void Renderer::CreateRtvAndDsvHeap()
	{
		D3D12_DESCRIPTOR_HEAP_DESC rtvDesc;
		rtvDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvDesc.NumDescriptors = mSwapChainBufferCount;
		rtvDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		rtvDesc.NodeMask = 0;
		ThrowIfFailed(mDevice->CreateDescriptorHeap(&rtvDesc, IID_PPV_ARGS(mRtvHeap.GetAddressOf())));

		D3D12_DESCRIPTOR_HEAP_DESC dsvDesc;
		dsvDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		dsvDesc.NumDescriptors = 1;
		dsvDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		dsvDesc.NodeMask = 0;
		ThrowIfFailed(mDevice->CreateDescriptorHeap(&dsvDesc, IID_PPV_ARGS(mDsvHeap.GetAddressOf())));
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

	void Renderer::SetViewport()
	{
		D3D12_VIEWPORT vp;
		vp.TopLeftX = 0.0f;
		vp.TopLeftY = 0.0f;
		vp.Width = static_cast<float>(CLIENT_WIDTH);
		vp.Height = static_cast<float>(CLIENT_HEIGHT);
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		mCommandList->RSSetViewports(1, &vp);
	}

	void Renderer::SetScissorRectangle()
	{
		mScissorRects[0] = { 0, 0, CLIENT_WIDTH / 2, CLIENT_HEIGHT / 2 };
		mCommandList->RSSetScissorRects(1, &mScissorRects[0]);
	}

	D3D12_CPU_DESCRIPTOR_HANDLE Renderer::CurrentBackBufferView() const
	{
		return CD3DX12_CPU_DESCRIPTOR_HANDLE
		(
			mRtvHeap->GetCPUDescriptorHandleForHeapStart(),
			mCurrBackBuffer,
			mRtvDescriptorSize
		);
	}

	D3D12_CPU_DESCRIPTOR_HANDLE Renderer::DepthStencilView() const
	{
		return mDsvHeap->GetCPUDescriptorHandleForHeapStart();
	}

	void Renderer::CreateRenderTargetView()
	{
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(mRtvHeap->GetCPUDescriptorHandleForHeapStart());
		for (uint32_t i = 0; i < mSwapChainBufferCount; i++)
		{
			ThrowIfFailed(mSwapChain->GetBuffer(i, IID_PPV_ARGS(mSwapChainBuffers[i].GetAddressOf())));
			mDevice->CreateRenderTargetView(mSwapChainBuffers[i].Get(), nullptr, rtvHeapHandle);
			rtvHeapHandle.Offset(1, mRtvDescriptorSize);
		}
	}

	void Renderer::CreateDepthStencilBufferAndView()
	{
		auto heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

		D3D12_RESOURCE_DESC dsvDesc;
		dsvDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		dsvDesc.Alignment = 0;
		dsvDesc.Width = CLIENT_WIDTH;
		dsvDesc.Height = CLIENT_HEIGHT;
		dsvDesc.DepthOrArraySize = 1;
		dsvDesc.MipLevels = 1;
		dsvDesc.Format = mDepthStencilFormat;
		dsvDesc.SampleDesc.Count = m4xMsaaState ? 4 : 1;
		dsvDesc.SampleDesc.Quality = m4xMsaaState ? (m4xMsaaQualityLevel - 1) : 0;
		dsvDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		dsvDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

		D3D12_CLEAR_VALUE clearValue;
		clearValue.Format = mDepthStencilFormat;
		clearValue.DepthStencil.Depth = 1.0f;
		clearValue.DepthStencil.Stencil = 0;

		ThrowIfFailed
		(
			mDevice->CreateCommittedResource
			(
				&heapProperty,
				D3D12_HEAP_FLAG_NONE,
				&dsvDesc,
				D3D12_RESOURCE_STATE_COMMON,
				&clearValue,
				IID_PPV_ARGS(mDepthStencilBuffer.GetAddressOf())
			)
		);

		mDevice->CreateDepthStencilView(mDepthStencilBuffer.Get(), nullptr, DepthStencilView());
		mCommandList->ResourceBarrier
		(
			1,
			&CD3DX12_RESOURCE_BARRIER::Transition
			(
				mDepthStencilBuffer.Get(),
				D3D12_RESOURCE_STATE_COMMON,
				D3D12_RESOURCE_STATE_DEPTH_WRITE
			)
		);

	}

	void Renderer::EnableDebugLayer() const
	{
		if (mDebug)
		{
			ComPtr<ID3D12Debug> debugController;
			ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
			debugController->EnableDebugLayer();
		}
	}

}