#pragma once

#include <vector>

namespace KRender
{
	using Microsoft::WRL::ComPtr;
	using namespace DirectX;
	struct Vertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT4 color;
	};
	class DX12VertexBuffer : public IBuffer
	{
	public:
		DX12VertexBuffer(ID3D12Device* device, std::vector<Vertex>& vertices);
		~DX12VertexBuffer() = default;
		void InitBuffer(ID3D12GraphicsCommandList* cmdList);
		void CreateBufferView() override;
		void Bind(ID3D12GraphicsCommandList* cmdList);
	private:
		std::vector<Vertex> mVertices;
		UINT64 mByteSize;
		ID3D12Device* mBufferDevice;
		ComPtr<ID3D12Resource> mVertexBufferGPU = nullptr;
		ComPtr<ID3D12Resource> mVertexBufferUploader = nullptr;
		D3D12_VERTEX_BUFFER_VIEW mVertexBufferView;
	};
}