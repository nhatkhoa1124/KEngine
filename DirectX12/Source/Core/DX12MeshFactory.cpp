#include "dx12pch.h"
#include "DX12MeshFactory.h"
#include "DX12Mesh.h"

namespace KRender
{
	DX12MeshFactory::DX12MeshFactory(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList) :
		mDevice{ device },
		mCmdList{ cmdList }
	{
	}

	std::unique_ptr<KEngine::IMesh> DX12MeshFactory::CreateMesh()
	{
		return std::make_unique<DX12Mesh>(mDevice, mCmdList);
	}
}