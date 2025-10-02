#pragma once

#include "DX12Core.h"
#include "d3dx12_include/directx/d3dx12.h"
#include <wrl/client.h>
#include <DirectXMath.h>
#include <d3d12.h>
#include <DX12Math.h>
#include "Core/Graphics/Renderer.h"
#include "Utils/DX12Helper.h"

namespace KRender
{
	using namespace DirectX;
	using Microsoft::WRL::ComPtr;

	template <typename T>
	class KRENDER_API DX12ConstantBuffer : public IBuffer
	{
	public:
		DX12ConstantBuffer(ID3D12Device* device, UINT32 numElements = 1) :
			mBufferDevice{ device },
			mNumElements{ numElements },
			mMappedData{ nullptr },
			mCbvDesc{}
		{
			mByteSize = CalcConstantBufferByteSize(sizeof(T));
		}
		~DX12ConstantBuffer()
		{
			if (mMappedData != nullptr && mConstantBufferUploader != nullptr)
			{
				mConstantBufferUploader->Unmap(0, nullptr);
			}
			mMappedData = nullptr;
		}
		void InitBuffer(ID3D12GraphicsCommandList* cmdList)
		{
			CD3DX12_HEAP_PROPERTIES heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
			CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(mByteSize * mNumElements);
			ThrowIfFailed(mBufferDevice->CreateCommittedResource
			(
				&heapProperties,
				D3D12_HEAP_FLAG_NONE,
				&resourceDesc,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&mConstantBufferUploader)
			));
			ThrowIfFailed(mConstantBufferUploader->Map(0, nullptr, reinterpret_cast<void**>(&mMappedData)));
		}
		void CreateBufferView() override
		{
			mBufferDevice->CreateConstantBufferView(&mCbvDesc, mCbvHeap->GetCPUDescriptorHandleForHeapStart());
		}
		void CopyData(int elementIndex, const T& data)
		{
			memcpy(mMappedData[elementIndex * mByteSize], &data, sizeof(T);
		}
		ID3D12Resource* GetResource() const
		{
			return mConstantBufferUploader.Get();
		}
		void CreateCbvHeap()
		{
			D3D12_DESCRIPTOR_HEAP_DESC cbvDesc = {};
			cbvDesc.NumDescriptors = 1;
			cbvDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			cbvDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
			cbvDesc.NodeMask = 0;
			mBufferDevice->CreateDescriptorHeap(&cbvDesc, IID_PPV_ARGS(mCbvHeap.GetAddressOf()));
		}
		void SetBufferViewDesc(D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc)
		{
			mCbvDesc = cbvDesc;
		}
		ID3D12DescriptorHeap* GetCBVHeap() const { return mCbvHeap.Get(); }
	private:
		UINT32 mByteSize;
		ID3D12Device* mBufferDevice;
		ComPtr<ID3D12Resource> mConstantBufferUploader = nullptr;
		ComPtr<ID3D12DescriptorHeap> mCbvHeap = nullptr;
		UINT32 mNumElements;
		BYTE* mMappedData;
		D3D12_CONSTANT_BUFFER_VIEW_DESC mCbvDesc;
	};
}