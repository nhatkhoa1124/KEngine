#include "dx12pch.h"
#include "DX12Renderer.h"
#include "Utils/DX12Helper.h"

namespace KRender
{
	DX12Renderer::DX12Renderer(HWND handle) :
		mHandle{ handle }
	{

	}

	DX12Renderer::~DX12Renderer()
	{

	}

	void DX12Renderer::Initialize()
	{
		DXGI_FORMAT renderTargetFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
		UINT swapChainBufferCount = 2;
		bool debugEnabled = true;
		bool isWindowd = true;
		bool useDepth = true;

		mDevice = std::make_unique<DX12Device>();
		mDevice->Initialize(debugEnabled);

		mCommandContext = std::make_unique<DX12CommandContext>();
		mCommandContext->Initialize(mDevice->GetDevice());

		mRTVHeap = std::make_unique<DX12DescriptorHeap>(*mDevice, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 2, false);
		mDSVHeap = std::make_unique<DX12DescriptorHeap>(*mDevice, D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1, true);

		mSwapChain = std::make_unique<DX12SwapChain>();
		mSwapChain->Initialize(*mDevice, *mCommandContext, mHandle, swapChainBufferCount, isWindowd, CLIENT_WIDTH, CLIENT_HEIGHT);

		mRenderTarget = std::make_unique<DX12RenderTarget>
			(
				*mDevice,
				mSwapChain->GetWidth(),
				mSwapChain->GetHeight(),
				renderTargetFormat,
				useDepth,
				mSwapChain->IsMsaa4xSupported(),
				mSwapChain->GetMsaa4xQualityLevel()
			);

		mViewport = std::make_unique<DX12Viewport>(mCommandContext->GetCommandList());
		mViewport->SetToRenderTargetSize(mSwapChain->GetWidth(), mSwapChain->GetHeight());
		mViewport->Apply();

		mShader = std::make_unique<DX12Shader>(mDevice->GetDevice(),
			mSwapChain->IsMsaa4xSupported(), mSwapChain->GetMsaa4xQualityLevel());
	}

	void DX12Renderer::Draw()
	{

	}

	void DX12Renderer::Exit()
	{

	}

}