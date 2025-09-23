#pragma once

#include <vector>

namespace KRender
{
	using Microsoft::WRL::ComPtr;
	class KRENDER_API DX12Renderer : public IRenderer
	{
	public:
		DX12Renderer(HWND handle);
		~DX12Renderer();
		void Initialize();
		void Draw();
		void Exit();
	private:
		void CreateDevice();
		void CreateFence();
		void CreateCommandObjects();
		void CreateSwapChain();
		void SetupDescriptorSize();
		void CreateFactory();
		void CreateRtvAndDsvHeap();
		void CheckMSAASupport4X();
		void CreateRenderTargetView();
		void CreateDepthStencilBufferAndView();
		void SetViewport();
		void SetScissorRectangle();
		D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView() const;
		D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView() const;
		void EnableDebugLayer() const;
		void FlushCommandQueue();
		inline void SetWindowed(bool isWindowed)
		{
			mIsWindowed = isWindowed;
		}
		inline void SetMsaaState(bool state)
		{
			m4xMsaaState = state;
		}
		inline void SetViewportWidth(uint32_t width)
		{
			mViewportWidth = width;
		}
		inline void SetViewportHeight(uint32_t height)
		{
			mViewportHeight = height;
		}
		inline ID3D12Resource* CurrentBackBuffer() const
		{
			return mSwapChainBuffers[mCurrBackBuffer].Get();
		}
	private:
		// Framework members
		ComPtr<ID3D12Device> mDevice;
		ComPtr<IDXGIFactory6> mDxgiFactory;
		ComPtr<ID3D12Fence> mFence;
		ComPtr<ID3D12CommandQueue> mCommandQueue;
		ComPtr<ID3D12CommandAllocator> mCommandAllocator;
		ComPtr<ID3D12GraphicsCommandList> mCommandList;
		ComPtr<IDXGISwapChain> mSwapChain;
		std::vector<ComPtr<ID3D12Resource>> mSwapChainBuffers;
		std::vector<tagRECT> mScissorRects;
		std::vector<D3D12_VIEWPORT> mScreenViewports;
		ComPtr<ID3D12Resource> mDepthStencilBuffer;
		ComPtr<ID3D12DescriptorHeap> mRtvHeap;
		ComPtr<ID3D12DescriptorHeap> mDsvHeap;
		UINT32 mRtvDescriptorSize;
		UINT32 mDsvDescriptorSize;
		UINT32 mCbvDescriptorSize;
		UINT32 m4xMsaaQualityLevel;
		UINT32 mViewportWidth;
		UINT32 mViewportHeight;
		bool m4xMsaaState;

		//Helper members
		HWND mWinHandle;
		bool mIsWindowed;

		// Hard-coded values
		static constexpr DXGI_FORMAT mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
		static constexpr DXGI_FORMAT mDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;;
		static constexpr UINT32 mSwapChainBufferCount = 2; //Double-buffering
		static constexpr UINT32 mViewportsCount = 1;
		static constexpr UINT32 mScissorsCount = 1;
		UINT32 mCurrBackBuffer = 0;
		UINT64 mCurrFenceValue = 0;
	private:
		// Macros
#ifdef _DEBUG
		bool mDebug = true;
#else
		bool mDebug = false;
#endif
	};

}