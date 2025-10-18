#pragma once

#include "DX12Core.h"
#include <wrl/client.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <vector>
#include "DX12Device.h"
#include "DX12CommandContext.h"
#include "Memory/DX12DescriptorHeap.h"

namespace KRender
{
	using Microsoft::WRL::ComPtr;
	class KRENDER_API DX12SwapChain
	{
	public:
		DX12SwapChain() = default;
		~DX12SwapChain() = default;

		void Initialize(DX12Device& device, DX12CommandContext& commandContext,
			HWND windowHandle, UINT bufferCount, bool isWindowed, UINT width, UINT height);
		void Resize(UINT32 width, UINT32 height);
		void Present(bool vsync = false);
		void MoveToNextFrame();
		void TransitionToRenderTarget(ID3D12GraphicsCommandList* cmdList);
		void TransitionToPresent(ID3D12GraphicsCommandList* cmdList);

		inline UINT GetWidth() const { return mWidth; }
		inline UINT GetHeight() const { return mHeight; }
		inline DXGI_FORMAT GetBackBufferFormat() const { return mBackBufferFormat; }
		inline ID3D12Resource* GetCurrentBackBuffer() const { return mSwapChainBuffers[mCurrentBackBuffer].Get(); }
		inline D3D12_RESOURCE_STATES GetCurrentBufferState() const { return mBufferStates[mCurrentBackBuffer]; }
		inline D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentBackBufferRTV() const { return mRTVHeap->GetCpuHandle(mCurrentBackBuffer); }
		inline UINT GetCurrentBackBufferIndex() const { return mCurrentBackBuffer; }
		inline bool IsMsaa4xSupported() const { return m4xMsaaState; }
		inline UINT GetMsaa4xQualityLevel() const { return m4xMsaaQualityLevel; }
	private:
		void CreateSwapChain();
		void CheckMsaa4xSupport();
		void CreateBackBuffers();

		ComPtr<IDXGISwapChain1> mSwapChain;
		std::vector<ComPtr<ID3D12Resource>> mSwapChainBuffers;
		std::unique_ptr<DX12DescriptorHeap> mRTVHeap;
		DX12Device* mDevice = nullptr;
		DX12CommandContext* mCmdContext = nullptr;
		HWND mWindowHandle = nullptr;
		UINT mWidth = 0;
		UINT mHeight = 0;

		UINT mCurrentBackBuffer = 0;
		UINT mBufferCount = 2;
		DXGI_FORMAT mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
		bool mIsWindowed = true;
		bool m4xMsaaState = false;
		UINT32 m4xMsaaQualityLevel = 0;
		std::vector<D3D12_RESOURCE_STATES> mBufferStates;
	};
}