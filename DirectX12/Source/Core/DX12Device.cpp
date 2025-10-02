#include "dx12pch.h"
#include "DX12Device.h"
#include "Utils/DX12Helper.h"

namespace KRender
{
	DX12Device::DX12Device(bool debugEnabled) :
		mDebugEnabled{ debugEnabled }
	{
	}

	void DX12Device::Initialize(bool debugEnabled)
	{
		mDebugEnabled = debugEnabled;
		if (mDebugEnabled)
		{
			EnableDebugLayer();
		}
		CreateFactory();
		CreateDevice();
	}

	void DX12Device::EnableDebugLayer() const
	{
		if (mDebugEnabled)
		{
			ComPtr<ID3D12Debug> debugController;
			ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
			debugController->EnableDebugLayer();
		}
	}

	void DX12Device::CreateDevice()
	{
		HRESULT hr = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&mDevice));

		if (FAILED(hr))
		{
			ComPtr<IDXGIAdapter> warpAdapter;
			ThrowIfFailed(mDxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter)));
			ThrowIfFailed(D3D12CreateDevice(warpAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&mDevice)));
		}
	}

	void DX12Device::CreateFactory()
	{
		ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&mDxgiFactory)));
	}
}