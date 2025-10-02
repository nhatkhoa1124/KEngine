#pragma once

#include "DX12Core.h"
#include "Core/Graphics/Renderer.h"

#include "DX12Shader.h"
#include "DX12Device.h"
#include "DX12CommandContext.h"
#include "DX12SwapChain.h"
#include "DX12RenderTarget.h"
#include "DX12DescriptorHeap.h"
#include "DX12Viewport.h"

namespace KRender
{
	class KRENDER_API DX12Renderer : public IRenderer
	{
	public:
		DX12Renderer(HWND handle);
		~DX12Renderer();
		void Initialize() override;
		void Draw() override;
		void Exit() override;
	private:
		std::unique_ptr<DX12Device> mDevice;
		std::unique_ptr<DX12DescriptorHeap> mRTVHeap;
		std::unique_ptr<DX12DescriptorHeap> mDSVHeap;
		std::unique_ptr<DX12CommandContext> mCommandContext;
		std::unique_ptr<DX12RenderTarget> mRenderTarget;
		std::unique_ptr<DX12SwapChain> mSwapChain;
		std::unique_ptr<DX12Viewport> mViewport;
		std::unique_ptr<DX12Shader> mShader;
		HWND mHandle;
	};

}