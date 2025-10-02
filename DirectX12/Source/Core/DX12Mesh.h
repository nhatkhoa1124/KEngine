#pragma once

#include <d3d12.h>
#include "DX12Core.h"
#include "Core/Graphics/Mesh.h"
#include "DX12VertexBuffer.h"
#include "DX12IndexBuffer.h"

namespace KRender
{
	using namespace KEngine;
	class KRENDER_API DX12Mesh : public IMesh
	{
	public:
		DX12Mesh(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList);
		~DX12Mesh() = default;
		void Initialize() override;
		void Render() override;
		void Shutdown() override;
		inline size_t GetVertexCount() const override { return mVertices.size(); }
		inline size_t GetIndexCount() const override { return mIndices.size(); }
		void SetPosition(float x, float y, float z) override;
		void SetRotation(float x, float y, float z) override;
		void SetScale(float x, float y, float z) override;

	private:
		std::vector<Vertex> mVertices;
		std::vector<UINT32> mIndices;
		std::unique_ptr<DX12VertexBuffer<Vertex>> mVertexBuffer;
		std::unique_ptr<DX12IndexBuffer> mIndexBuffer;
		ID3D12Device* mDevice;
		ID3D12GraphicsCommandList* mCmdList;
	};
}
