#include "dx12pch.h"
#include "DX12DescriptorHeap.h"
#include "Utils/DX12Helper.h"
#include <stdexcept>

namespace KRender
{
	DX12DescriptorHeap::DX12DescriptorHeap
	(DX12Device& device, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT count, bool shaderVisible) :
		mMaxDescriptors{ count },
		mUsedDescriptors{ 0 },
		mHeapType{ type },
		mShaderVisible{ shaderVisible }
	{
		CreateHeap(device);
		mCpuHeapStart = mHeap->GetCPUDescriptorHandleForHeapStart();
		if (mShaderVisible)
		{
			mGpuHeapStart = mHeap->GetGPUDescriptorHandleForHeapStart();
		}
		mDescriptorSize = device.GetDevice()->GetDescriptorHandleIncrementSize(type);
	}

	D3D12_CPU_DESCRIPTOR_HANDLE DX12DescriptorHeap::GetCpuHandle(UINT index) const
	{
		CD3DX12_CPU_DESCRIPTOR_HANDLE handle(mCpuHeapStart);
		handle.Offset(index, mDescriptorSize);
		return handle;
	}

	D3D12_GPU_DESCRIPTOR_HANDLE DX12DescriptorHeap::GetGpuHandle(UINT index) const
	{
		CD3DX12_GPU_DESCRIPTOR_HANDLE handle(mGpuHeapStart);
		handle.Offset(index, mDescriptorSize);
		return handle;
	}

	D3D12_CPU_DESCRIPTOR_HANDLE DX12DescriptorHeap::GetCpuHandleOffset(UINT baseIndex, UINT offset) const
	{
		return GetCpuHandle(baseIndex + offset);
	}

	D3D12_GPU_DESCRIPTOR_HANDLE DX12DescriptorHeap::GetGpuHandleOffset(UINT baseIndex, UINT offset) const
	{
		return GetGpuHandle(baseIndex + offset);
	}

	UINT DX12DescriptorHeap::AllocateDescriptors(UINT count)
	{
		// Return allocated descriptor's index
		UINT startIndex = mUsedDescriptors;
		mUsedDescriptors += count;
		return startIndex;
	}

	void DX12DescriptorHeap::FreeDescriptors(UINT index, UINT count)
	{
		// Simple implementation with no allocation strategy, only freeing from the end
		if (index + count > mUsedDescriptors)
		{
			throw std::out_of_range("Descriptor range out of bounds.");
		}
		if (index + count == mUsedDescriptors)
		{
			mUsedDescriptors = index;
		}
		else
		{
			throw std::exception("Can only free descriptors from the end.");
		}
	}

	void DX12DescriptorHeap::CreateHeap(DX12Device& device)
	{
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
		heapDesc.Type = mHeapType;
		heapDesc.NumDescriptors = mMaxDescriptors;
		heapDesc.Flags = mShaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		heapDesc.NodeMask = 0;
		ThrowIfFailed(device.GetDevice()->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(mHeap.GetAddressOf())));
	}
}