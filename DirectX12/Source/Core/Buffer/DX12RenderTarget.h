#pragma once

#include "DX12Core.h"
#include <wrl/client.h>
#include <d3d12.h>
#include <memory>
#include "DX12Device.h"
#include "DX12DescriptorHeap.h"

namespace KRender
{
	using Microsoft::WRL::ComPtr;
	class KRENDER_API DX12RenderTarget
	{
	public:
		DX12RenderTarget
		(DX12Device& device, UINT width,
			UINT height, DXGI_FORMAT renderFormat = DXGI_FORMAT_R8G8B8A8_UNORM,
			bool useDepth = true,
			bool enableMsaa4x = false,
			UINT msaa4xQuality = 0
		);
		~DX12RenderTarget();
		void Resize(UINT width, UINT height);
		void BeginRender
		(
			ID3D12GraphicsCommandList* cmdList,
			const FLOAT* clearColor = nullptr,
			FLOAT clearDepth = 1.0f,
			UINT8 clearStencil = 0
		);
		void EndRender(ID3D12GraphicsCommandList* cmdList);
		D3D12_CPU_DESCRIPTOR_HANDLE GetRTV() const { return mRTVHeap->GetCpuHandle(0); }
		D3D12_CPU_DESCRIPTOR_HANDLE GetDSV() const { return mDSVHeap->GetCpuHandle(0); }

	private:
		void CreateResources();
		void CreateRenderTarget();
		void CreateDepthStencil();
		void ReleaseResources();
		void TransitionToRenderTarget(ID3D12GraphicsCommandList* cmdList);
		void TransitionToShaderResource(ID3D12GraphicsCommandList* cmdList);

		ComPtr<ID3D12Resource> mRenderTarget;
		ComPtr<ID3D12Resource> mDepthStencil;
		std::unique_ptr<DX12DescriptorHeap> mRTVHeap;
		std::unique_ptr<DX12DescriptorHeap> mDSVHeap;
		DX12Device* mDevice;
		UINT mWidth, mHeight;
		DXGI_FORMAT mRTFormat;
		DXGI_FORMAT mDSFormat;
		bool mUseDepth;
		bool m4xMsaaState;
		UINT m4xMsaaQuality;
	};
}