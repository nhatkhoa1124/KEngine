#include "dx12pch.h"
#include "IndexBuffer.h"

namespace KRender
{
	IndexBuffer::IndexBuffer(ID3D12Device* device, std::vector<UINT32> indices) :
		mBufferDevice{ device },
		mIndices{ indices },
		mIndexBufferView{}
	{
		mByteSize = mIndices.size() * sizeof(UINT32);
	}
	void IndexBuffer::InitBuffer(ID3D12GraphicsCommandList* cmdList)
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
	void IndexBuffer::CreateBufferView()
	{
		mIndexBufferView.BufferLocation = mIndexBufferGPU->GetGPUVirtualAddress();
		mIndexBufferView.Format = DXGI_FORMAT_R32_UINT;
		mIndexBufferView.SizeInBytes = mByteSize;
	}
	void IndexBuffer::Bind(ID3D12GraphicsCommandList* cmdList)
	{
		cmdList->IASetIndexBuffer(&mIndexBufferView);
	}
}