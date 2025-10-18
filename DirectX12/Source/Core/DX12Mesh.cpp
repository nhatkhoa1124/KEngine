#include "dx12pch.h"
#include "DX12Mesh.h"

namespace KRender
{
	DX12Mesh::DX12Mesh(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList) :
		mVertexBuffer{ nullptr },
		mIndexBuffer{ nullptr },
		mDevice{ device },
		mCmdList{ cmdList }
	{
	}
	void DX12Mesh::SetMesh(std::vector<Vertex>& vertices, std::vector<UINT32> indices)
	{
		mVertices = vertices;
		mIndices = indices;
		SetPosition(1.0f, 1.0f, 0.0f);
		SetRotation(1.0f, 1.0f, 1.0f);
		SetScale(1.0f, 1.0f, 1.0f);
	}
	void DX12Mesh::DrawMesh()
	{
		mVertexBuffer->Bind(mCmdList);

		if (mIndexBuffer && !mIndices.empty())
		{
			mIndexBuffer->Bind(mCmdList);
			mCmdList->DrawIndexedInstanced(static_cast<UINT>(mIndices.size()), 1, 0, 0, 0);
		}
		else
		{
			mCmdList->DrawInstanced(static_cast<UINT>(mVertices.size()), 1, 0, 0);
		}
	}
	void DX12Mesh::Shutdown()
	{
	}
	void DX12Mesh::SetPosition(float x, float y, float z)
	{
		mPosition = DirectX::XMFLOAT3(x, y, z);
	}
	void DX12Mesh::SetRotation(float x, float y, float z)
	{
		mRotation = DirectX::XMFLOAT3(x, y, z);
	}
	void DX12Mesh::SetScale(float x, float y, float z)
	{
		mScale = DirectX::XMFLOAT3(x, y, z);
	}
	void DX12Mesh::CreateVertexBuffer()
	{
		mVertexBuffer = std::make_unique<DX12VertexBuffer<Vertex>>(mDevice, mVertices);
		mVertexBuffer->InitBuffer(mCmdList);
		mVertexBuffer->CreateBufferView();
	}
	void DX12Mesh::CreateIndexBuffer()
	{
		mIndexBuffer = std::make_unique<DX12IndexBuffer>(mDevice, mIndices);
		mIndexBuffer->InitBuffer(mCmdList);
		mIndexBuffer->CreateBufferView();
	}
}

