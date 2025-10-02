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

	void DX12SwapChain::CreateSwapChain()
	{
		IDXGIFactory6* dxgiFactory = mDevice->GetFactory();
		ID3D12CommandQueue* commandQueue = mCmdContext->GetCommandQueue();

		DXGI_SWAP_CHAIN_DESC scDesc = {};
		scDesc.BufferDesc.Width = mWidth;
		scDesc.BufferDesc.Height = mHeight;
		scDesc.BufferDesc.Format = mBackBufferFormat;
		scDesc.BufferDesc.RefreshRate.Numerator = 60;
		scDesc.BufferDesc.RefreshRate.Denominator = 1;
		scDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		scDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		scDesc.SampleDesc.Count = m4xMsaaState ? 4 : 1;
		scDesc.SampleDesc.Quality = m4xMsaaState ? (m4xMsaaQualityLevel - 1) : 0;
		scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scDesc.BufferCount = mBufferCount;
		scDesc.OutputWindow = mWindowHandle;
		scDesc.Windowed = mIsWindowed;
		scDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		scDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		ThrowIfFailed(dxgiFactory->CreateSwapChain(commandQueue, &scDesc, mSwapChain.GetAddressOf()));
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
		mSwapChainBuffers.resize(mBufferCount);
		for (UINT i = 0; i < mBufferCount; i++)
		{
			ThrowIfFailed(mSwapChain->GetBuffer(i, IID_PPV_ARGS(&mSwapChainBuffers[i])));
		}
	}
}