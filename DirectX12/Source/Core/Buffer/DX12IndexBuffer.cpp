#include "dx12pch.h"
#include "DX12IndexBuffer.h"
#include "Utils/DX12Helper.h"

namespace KRender
{
	DX12IndexBuffer::DX12IndexBuffer(ID3D12Device* device, std::vector<UINT32> indices) :
		mBufferDevice{ device },
		mIndices{ indices },
		mIndexBufferView{}
	{
		mByteSize = mIndices.size() * sizeof(UINT32);
	}
	void DX12IndexBuffer::InitBuffer(ID3D12GraphicsCommandList* cmdList)
	{
		mIndexBufferGPU = CreateDefaultBuffer
		(
			mBufferDevice,
			cmdList,
			mIndices.data(),
			mByteSize,
			mIndexBufferUploader
		);
	}
	void DX12IndexBuffer::CreateBufferView()
	{
		mIndexBufferView.BufferLocation = mIndexBufferGPU->GetGPUVirtualAddress();
		mIndexBufferView.Format = DXGI_FORMAT_R32_UINT;
		mIndexBufferView.SizeInBytes = mByteSize;
	}
	void DX12IndexBuffer::Bind(ID3D12GraphicsCommandList* cmdList)
	{
		cmdList->IASetIndexBuffer(&mIndexBufferView);
	}
}