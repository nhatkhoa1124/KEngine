#pragma once
#include <cstdint>

namespace KRender
{
	using Microsoft::WRL::ComPtr;
	class KRENDER_API Renderer
	{
	public:
		Renderer();
		~Renderer();
		void CreateDevice();
		void CreateFence();
		void CreateCommandObjects();
		void CreateSwapChain();
		void SetupDescriptorSize();
		void CreateFactory();
		void CheckMSAASupport4X();
		inline void EnableDebugLayer() const
		{
			if (mDebug)
			{
				ComPtr<ID3D12Debug> debugController;
				ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
				debugController->EnableDebugLayer();
			}
		}
	private:
		ComPtr<ID3D12Device> mDevice;
		ComPtr<IDXGIFactory6> mDxgiFactory;
		ComPtr<ID3D12Fence> mFence;
		ComPtr<ID3D12CommandQueue> mCommandQueue;
		ComPtr<ID3D12CommandAllocator> mCommandAllocator;
		ComPtr<ID3D12GraphicsCommandList> mCommandList;

		DXGI_FORMAT mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

		uint32_t mRtvDescriptorSize;
		uint32_t mDsvDescriptorSize;
		uint32_t mCbvDescriptorSize;
		uint32_t m4xMsaaQualityLevel;
		const uint32_t mClientWidth = 1280;
		const uint32_t mClientHeight = 720;
	private:
#ifdef _DEBUG
		bool mDebug = true;
#else
		bool mDebug = false;
#endif
	};

}