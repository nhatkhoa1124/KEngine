#include "dx12pch.h"
#include "VertexBuffer.h"
#include "DirectXColors.h"
#include "Utils/DX12Helper.h"

namespace KRender
{
	DX12VertexBuffer::DX12VertexBuffer(ID3D12Device* device, std::vector<Vertex>& vertices) :
		mBufferDevice{ device },
		mVertices{ vertices },
		mVertexBufferView{}
	{
		mByteSize = mVertices.size() * sizeof(Vertex);
	}

	void DX12VertexBuffer::InitBuffer(ID3D12GraphicsCommandList* cmdList)
	{
		mVertexBufferGPU = CreateDefaultBuffer
		(
			mBufferDevice,
			cmdList,
			mVertices.data(),
			mByteSize,
			mVertexBufferUploader
		);
	}

	void DX12VertexBuffer::CreateBufferView()
	{
		mVertexBufferView.BufferLocation = mVertexBufferGPU->GetGPUVirtualAddress();
		mVertexBufferView.SizeInBytes = mByteSize;
		mVertexBufferView.StrideInBytes = sizeof(Vertex);
	}

	void DX12VertexBuffer::Bind(ID3D12GraphicsCommandList* cmdList)
	{
		cmdList->IASetVertexBuffers(0, 1, &mVertexBufferView);
	}
}
