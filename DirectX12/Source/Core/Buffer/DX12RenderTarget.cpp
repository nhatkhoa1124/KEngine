#include "dx12pch.h"
#include "DX12RenderTarget.h"
#include "Utils/DX12Helper.h"
#include <stdexcept>
#include <DirectXColors.h>

namespace KRender
{
	DX12RenderTarget::DX12RenderTarget
	(
		DX12Device& device,
		UINT width, UINT height, DXGI_FORMAT renderFormat,
		bool useDepth,
		bool enableMsaa4x,
		UINT msaa4xQuality
	) :
		mDevice{ &device },
		mWidth{ width },
		mHeight{ height },
		mRTFormat{ renderFormat },
		mDSFormat{ DXGI_FORMAT_D24_UNORM_S8_UINT },
		mUseDepth{ useDepth },
		m4xMsaaState{ enableMsaa4x },
		m4xMsaaQuality{ msaa4xQuality }
	{
		if (mDevice == nullptr)
		{
			throw std::invalid_argument("Device cannot be null.");
		}
		if (mWidth == 0 || mHeight == 0)
		{
			throw std::invalid_argument("Width and height must be greater than 0.");
		}
		CreateResources();
	}
	DX12RenderTarget::~DX12RenderTarget()
	{
		ReleaseResources();
	}
	void DX12RenderTarget::Resize(UINT width, UINT height)
	{
		if (width == mWidth && height == mHeight)
		{
			return;
		}

		if (width == 0 || height == 0)
		{
			throw std::invalid_argument("Width and height must be greater than 0.");
		}
		mWidth = width;
		mHeight = height;
		ReleaseResources();
		CreateResources();
	}
	void DX12RenderTarget::BeginRender(ID3D12GraphicsCommandList* cmdList, const FLOAT* clearColor, FLOAT clearDepth, UINT8 clearStencil)
	{
		TransitionToRenderTarget(cmdList);
		if (clearColor != nullptr)
		{
			cmdList->ClearRenderTargetView(GetRTV(), clearColor, 0, nullptr);
		}
		if (mUseDepth)
		{
			cmdList->ClearDepthStencilView
			(
				GetDSV(),
				D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,
				clearDepth,
				clearStencil,
				0,
				nullptr
			);
		}
		D3D12_CPU_DESCRIPTOR_HANDLE rtv = GetRTV();
		if (mUseDepth)
		{
			D3D12_CPU_DESCRIPTOR_HANDLE dsv = GetDSV();
			cmdList->OMSetRenderTargets(1, &rtv, true, &dsv);
		}
		else
		{
			cmdList->OMSetRenderTargets(1, &rtv, true, nullptr);
		}
	}
	void DX12RenderTarget::EndRender(ID3D12GraphicsCommandList* cmdList)
	{
		TransitionToShaderResource(cmdList);
	}
	void DX12RenderTarget::CreateResources()
	{
		mRTVHeap = std::make_unique<DX12DescriptorHeap>
			(
				*mDevice,
				D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
				1,
				false
			);
		if (mUseDepth)
		{
			mDSVHeap = std::make_unique<DX12DescriptorHeap>
				(
					*mDevice,
					D3D12_DESCRIPTOR_HEAP_TYPE_DSV,
					1,
					false
				);
		}
		CreateRenderTarget();
		if (mUseDepth)
		{
			CreateDepthStencil();
		}
	}
	void DX12RenderTarget::CreateRenderTarget()
	{
		D3D12_RESOURCE_DESC resourceDesc = {};
		resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		resourceDesc.Alignment = 0;
		resourceDesc.Width = mWidth;
		resourceDesc.Height = mHeight;
		resourceDesc.DepthOrArraySize = 1;
		resourceDesc.MipLevels = 1;
		resourceDesc.Format = mRTFormat;
		resourceDesc.SampleDesc.Count = m4xMsaaState ? 4 : 1;
		resourceDesc.SampleDesc.Quality = m4xMsaaState ? (m4xMsaaQuality - 1) : 0;
		resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

		D3D12_CLEAR_VALUE clearValue = {};
		clearValue.Format = mRTFormat;
		memcpy(clearValue.Color, DirectX::Colors::Azure, sizeof(clearValue.Color));
		auto heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		ThrowIfFailed(mDevice->GetDevice()->CreateCommittedResource
		(
			&heapProperties,
			D3D12_HEAP_FLAG_NONE,
			&resourceDesc,
			D3D12_RESOURCE_STATE_COMMON,
			&clearValue,
			IID_PPV_ARGS(mRenderTarget.GetAddressOf())
		));

		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
		rtvDesc.Format = mRTFormat;
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		rtvDesc.Texture2D.MipSlice = 0;
		rtvDesc.Texture2D.PlaneSlice = 0;
		mDevice->GetDevice()->CreateRenderTargetView
		(
			mRenderTarget.Get(),
			&rtvDesc,
			mRTVHeap->GetCpuHandle(0)
		);

	}
	void DX12RenderTarget::CreateDepthStencil()
	{
		D3D12_RESOURCE_DESC depthDesc = {};
		depthDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		depthDesc.Alignment = 0;
		depthDesc.Width = mWidth;
		depthDesc.Height = mHeight;
		depthDesc.DepthOrArraySize = 1;
		depthDesc.MipLevels = 1;
		depthDesc.Format = mDSFormat;
		depthDesc.SampleDesc.Count = m4xMsaaState ? 4 : 1;
		depthDesc.SampleDesc.Quality = m4xMsaaState ? (m4xMsaaQuality - 1) : 0;;
		depthDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		depthDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

		D3D12_CLEAR_VALUE clearValue = {};
		clearValue.Format = mDSFormat;
		clearValue.DepthStencil.Depth = 1.0f;
		clearValue.DepthStencil.Stencil = 0;

		auto heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		ThrowIfFailed
		(
			mDevice->GetDevice()->CreateCommittedResource
			(
				&heapProperty,
				D3D12_HEAP_FLAG_NONE,
				&depthDesc,
				D3D12_RESOURCE_STATE_COMMON,
				&clearValue,
				IID_PPV_ARGS(mDepthStencil.GetAddressOf())
			)
		);

		D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
		dsvDesc.Format = mDSFormat;
		dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
		dsvDesc.Texture2D.MipSlice = 0;

		mDevice->GetDevice()->CreateDepthStencilView(mDepthStencil.Get(), &dsvDesc, mDSVHeap->GetCpuHandle(0));
	}

	void DX12RenderTarget::ReleaseResources()
	{
		mRenderTarget.Reset();
		mDepthStencil.Reset();
		mRTVHeap.reset();
		mDSVHeap.reset();
	}
	void DX12RenderTarget::TransitionToRenderTarget(ID3D12GraphicsCommandList* cmdList)
	{
		auto PresentToTarget = CD3DX12_RESOURCE_BARRIER::Transition
		(
			mRenderTarget.Get(),
			D3D12_RESOURCE_STATE_COMMON,
			D3D12_RESOURCE_STATE_RENDER_TARGET
		);
		cmdList->ResourceBarrier(1, &PresentToTarget);
	}
	void DX12RenderTarget::TransitionToShaderResource(ID3D12GraphicsCommandList* cmdList)
	{
		auto TargetToShaderResource = CD3DX12_RESOURCE_BARRIER::Transition
		(
			mRenderTarget.Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_COMMON
		);
		cmdList->ResourceBarrier(1, &TargetToShaderResource);
	}
}