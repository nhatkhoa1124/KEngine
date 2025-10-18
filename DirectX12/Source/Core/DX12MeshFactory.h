#pragma once
#include "DX12Core.h"
#include <d3d12.h>
#include "Core/Graphics/Mesh.h"
#include "DX12CommandContext.h"

namespace KRender
{
	class KRENDER_API DX12MeshFactory : public KEngine::IMeshFactory
	{
	public:
		DX12MeshFactory(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList);
		std::unique_ptr<KEngine::IMesh> CreateMesh() override;
	private:
		ID3D12Device* mDevice;
		ID3D12GraphicsCommandList* mCmdList;
	};
}