#pragma once

#include "DX12Core.h"
#include "d3dx12_include/directx/d3dx12.h"
#include <wrl/client.h>
#include <d3d12.h>
#include "DX12Device.h"

namespace KRender
{
	using Microsoft::WRL::ComPtr;
	class KRENDER_API DX12DescriptorHeap
	{
	public:
		DX12DescriptorHeap(DX12Device& device, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT count, bool shaderVisible);
		~DX12DescriptorHeap() = default;
		D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle(UINT index) const;
		D3D12_GPU_DESCRIPTOR_HANDLE GetGpuHandle(UINT index) const;
		D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandleOffset(UINT baseIndex, UINT offset) const;
		D3D12_GPU_DESCRIPTOR_HANDLE GetGpuHandleOffset(UINT baseIndex, UINT offset) const;

		inline ID3D12DescriptorHeap* GetHeap() const { return mHeap.Get(); }
		inline UINT GetDescriptorSize() const { return mDescriptorSize; }
		inline UINT GetMaxDescriptors() const { return mMaxDescriptors; }
		inline UINT GetUsedDescriptors() const { return mUsedDescriptors; }
		inline D3D12_DESCRIPTOR_HEAP_TYPE GetType() const { return mHeapType; }
		inline bool IsShaderVisible() const { return mShaderVisible; }
		inline bool IsFull() const { return mUsedDescriptors >= mMaxDescriptors; }
		inline void Reset() { mUsedDescriptors = 0; }

		UINT AllocateDescriptors(UINT count = 1);
		void FreeDescriptors(UINT index, UINT count = 1);
	private:
		void CreateHeap(DX12Device& device);

		ComPtr<ID3D12DescriptorHeap> mHeap;
		D3D12_CPU_DESCRIPTOR_HANDLE mCpuHeapStart;
		D3D12_GPU_DESCRIPTOR_HANDLE mGpuHeapStart;
		UINT mDescriptorSize;
		UINT mMaxDescriptors;
		UINT mUsedDescriptors;
		D3D12_DESCRIPTOR_HEAP_TYPE mHeapType;
		bool mShaderVisible;
	};
}