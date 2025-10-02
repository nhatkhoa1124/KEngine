#pragma once

#include "DX12Core.h"
#include "d3dx12_include/directx/d3dx12.h"
#include <DirectXMath.h>
#include <wrl/client.h>
#include <d3d12.h>
#include "Core/Graphics/Renderer.h"
#include "DX12VertexBuffer.h"
#include "DirectXColors.h"
#include "Utils/DX12Helper.h"

namespace KRender
{
	using Microsoft::WRL::ComPtr;

	template <typename T>
	class KRENDER_API DX12VertexBuffer : public IBuffer
	{
	public:
		DX12VertexBuffer(ID3D12Device* device, std::vector<T>& data) :
			mBufferDevice{ std::move(data) },
			mData{ data },
			mVertexBufferView{}
		{
			mByteSize = mData.size() * sizeof(T);
		}
		~DX12VertexBuffer() = default;
		DX12VertexBuffer(DX12VertexBuffer&&) = default;
		DX12VertexBuffer& operator=(DX12VertexBuffer&&) = default;

		DX12VertexBuffer(const DX12VertexBuffer&) = delete;
		DX12VertexBuffer& operator=(const DX12VertexBuffer&) = delete;
		void InitBuffer(ID3D12GraphicsCommandList* cmdList)
		{
			mVertexBufferGPU = CreateDefaultBuffer
			(
				mBufferDevice,
				cmdList,
				mData.data(),
				mByteSize,
				mVertexBufferUploader
			);
			CreateBufferView();
		}
		void CreateBufferView() override
		{
			mVertexBufferView.BufferLocation = mVertexBufferGPU->GetGPUVirtualAddress();
			mVertexBufferView.SizeInBytes = mByteSize;
			mVertexBufferView.StrideInBytes = sizeof(T);
		}
		void Bind(ID3D12GraphicsCommandList* cmdList) const
		{
			cmdList->IASetVertexBuffers(0, 1, &mVertexBufferView);
		}
	private:
		std::vector<T> mData;
		UINT64 mByteSize;
		ID3D12Device* mBufferDevice;
		ComPtr<ID3D12Resource> mVertexBufferGPU = nullptr;
		ComPtr<ID3D12Resource> mVertexBufferUploader = nullptr;
		D3D12_VERTEX_BUFFER_VIEW mVertexBufferView;
	};
}