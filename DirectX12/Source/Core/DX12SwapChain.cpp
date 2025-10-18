#include "dx12pch.h"
#include "DX12SwapChain.h"
#include "Utils/DX12Helper.h"

namespace KRender
{
	void DX12SwapChain::Initialize(DX12Device& device, DX12CommandContext& commandContext,
		HWND windowHandle, UINT bufferCount, bool isWindowed, UINT width, UINT height)
	{
		mDevice = &device;
		mCmdContext = &commandContext;
		mWindowHandle = windowHandle;
		mIsWindowed = isWindowed;
		mBufferCount = bufferCount;
		mWidth = width;
		mHeight = height;
		mSwapChainBuffers.resize(mBufferCount);
		mBufferStates.resize(mBufferCount, D3D12_RESOURCE_STATE_PRESENT);

		CheckMsaa4xSupport();
		CreateSwapChain();

		RECT clientRect;
		GetClientRect(mWindowHandle, &clientRect);
		UINT rectWidth = clientRect.right - clientRect.left;
		UINT rectHeight = clientRect.bottom - clientRect.top;
		Resize(rectWidth, rectHeight);
	}

	void DX12SwapChain::Resize(UINT32 width, UINT32 height)
	{
		if (width == 0 || height == 0)
		{
			return;
		}
		mCmdContext->Flush();
		for (UINT i = 0; i < mBufferCount; i++)
		{
			mSwapChainBuffers[i].Reset();
		}
		ThrowIfFailed
		(
			mSwapChain->ResizeBuffers(
				mBufferCount, width, height, mBackBufferFormat, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
			)
		);
		mCurrentBackBuffer = 0;
		CreateBackBuffers();
	}

	void DX12SwapChain::Present(bool vsync)
	{
		ThrowIfFailed(mSwapChain->Present(vsync ? 1 : 0, 0));
		MoveToNextFrame();
	}

	void DX12SwapChain::MoveToNextFrame()
	{
		mCurrentBackBuffer = (mCurrentBackBuffer + 1) % mBufferCount;
	}

	void DX12SwapChain::TransitionToRenderTarget(ID3D12GraphicsCommandList* cmdList)
	{
		auto PresentToTarget = CD3DX12_RESOURCE_BARRIER::Transition
		(
			GetCurrentBackBuffer(),
			GetCurrentBufferState(),
			D3D12_RESOURCE_STATE_RENDER_TARGET
		);
		cmdList->ResourceBarrier(1, &PresentToTarget);
		mBufferStates[mCurrentBackBuffer] = D3D12_RESOURCE_STATE_RENDER_TARGET;
	}

	void DX12SwapChain::TransitionToPresent(ID3D12GraphicsCommandList* cmdList)
	{
		auto TargetToPresent = CD3DX12_RESOURCE_BARRIER::Transition
		(
			GetCurrentBackBuffer(),
			GetCurrentBufferState(),
			D3D12_RESOURCE_STATE_PRESENT
		);
		cmdList->ResourceBarrier(1, &TargetToPresent);
		mBufferStates[mCurrentBackBuffer] = D3D12_RESOURCE_STATE_PRESENT;
	}

	void DX12SwapChain::CreateSwapChain()
	{
		ComPtr<IDXGIFactory6> dxgiFactory = mDevice->GetFactory();
		ID3D12CommandQueue* commandQueue = mCmdContext->GetCommandQueue();

		DXGI_SWAP_CHAIN_DESC1 scDesc = {};
		scDesc.Width = mWidth;
		scDesc.Height = mHeight;
		scDesc.Format = mBackBufferFormat;
		scDesc.Stereo = false;
		scDesc.SampleDesc.Count = m4xMsaaState ? 4 : 1;
		scDesc.SampleDesc.Quality = m4xMsaaState ? (m4xMsaaQualityLevel - 1) : 0;
		scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scDesc.BufferCount = mBufferCount;
		scDesc.Scaling = DXGI_SCALING_STRETCH;
		scDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		scDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
		scDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsDesc = {};
		fsDesc.RefreshRate.Numerator = 60;
		fsDesc.RefreshRate.Denominator = 1;
		fsDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		fsDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		fsDesc.Windowed = mIsWindowed;

		ComPtr<IDXGISwapChain1> swapChain1;
		ThrowIfFailed
		(
			dxgiFactory->CreateSwapChainForHwnd
			(
				commandQueue,
				mWindowHandle,
				&scDesc,
				&fsDesc,
				nullptr,
				&swapChain1
			)
		);
		ThrowIfFailed(swapChain1.As(&mSwapChain));
	}

	void DX12SwapChain::CheckMsaa4xSupport()
	{
		D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevel = {};
		msQualityLevel.Format = mBackBufferFormat;
		msQualityLevel.SampleCount = 4;
		msQualityLevel.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
		msQualityLevel.NumQualityLevels = 0;
		ThrowIfFailed(mDevice->GetDevice()->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &msQualityLevel, sizeof(msQualityLevel)));

		m4xMsaaQualityLevel = msQualityLevel.NumQualityLevels;
		assert(m4xMsaaQualityLevel > 0);
	}

	void DX12SwapChain::CreateBackBuffers()
	{
		mRTVHeap = std::make_unique<DX12DescriptorHeap>
			(*mDevice, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, mBufferCount, false);

		for (UINT i = 0; i < mBufferCount; i++)
		{
			ThrowIfFailed(mSwapChain->GetBuffer(i, IID_PPV_ARGS(&mSwapChainBuffers[i])));
			mBufferStates[i] = D3D12_RESOURCE_STATE_PRESENT;
			mDevice->GetDevice()->CreateRenderTargetView
			(
				mSwapChainBuffers[i].Get(),
				nullptr,
				mRTVHeap->GetCpuHandle(i)
			);
		}
	}
}