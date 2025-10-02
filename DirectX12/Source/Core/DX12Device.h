#pragma once

#include "DX12Core.h"
#include <wrl/client.h>
#include <d3d12.h>
#include <dxgi1_6.h>

namespace KRender
{
	using Microsoft::WRL::ComPtr;
	class KRENDER_API DX12Device
	{
	public:
		DX12Device(bool debugEnabled = false);
		~DX12Device() = default;
		void Initialize(bool debugEnabled);
		inline ID3D12Device* GetDevice() const { return mDevice.Get(); }
		inline IDXGIFactory6* GetFactory() const { return mDxgiFactory.Get(); }
		inline void EnableDebugMode(bool debugEnabled) { mDebugEnabled = debugEnabled; }
	private:
		void EnableDebugLayer() const;
		void CreateFactory();
		void CreateDevice();

		ComPtr<ID3D12Device> mDevice;
		ComPtr<IDXGIFactory6> mDxgiFactory;
		bool mDebugEnabled;
	};
}