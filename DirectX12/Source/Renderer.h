#pragma once
#include <cstdint>
#include <vector>

namespace KRender
{
	using Microsoft::WRL::ComPtr;
	class KRENDER_API Renderer
	{
	public:
		Renderer(HWND handle);
		~Renderer();
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
		uint32_t mRtvDescriptorSize;
		uint32_t mDsvDescriptorSize;
		uint32_t mCbvDescriptorSize;
		uint32_t m4xMsaaQualityLevel;
		uint32_t mViewportWidth;
		uint32_t mViewportHeight;
		bool m4xMsaaState;

		//Helper members
		HWND mWinHandle;
		bool mIsWindowed;

		// Hard-coded values
		static constexpr DXGI_FORMAT mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
		static constexpr DXGI_FORMAT mDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;;
		static constexpr uint32_t mSwapChainBufferCount = 2; //Double-buffering
		static constexpr uint32_t mViewportsCount = 1;
		static constexpr uint32_t mScissorsCount = 1;
		uint32_t mCurrBackBuffer = 0;
		uint64_t mCurrFenceValue = 0;
	private:
		// Macros
#ifdef _DEBUG
		bool mDebug = true;
#else
		bool mDebug = false;
#endif
	};

}