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
	void DX12Mesh::Initialize()
	{
	}
	void DX12Mesh::Render()
	{
	}
	void DX12Mesh::Shutdown()
	{
	}
	void DX12Mesh::SetPosition(float x, float y, float z)
	{
	}
	void DX12Mesh::SetRotation(float x, float y, float z)
	{
	}
	void DX12Mesh::SetScale(float x, float y, float z)
	{
	}
}

