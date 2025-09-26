#pragma once

#include "d3dx12_include/directx/d3dx12.h"
#include <wrl/client.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <vector>
#include "Core/Graphics/IRenderer.h"

namespace KRender
{
	using Microsoft::WRL::ComPtr;
	class IndexBuffer : public IBuffer
	{
	public:
		IndexBuffer(ID3D12Device* device, std::vector<UINT32> indices);
		~IndexBuffer() = default;
		void InitBuffer(ID3D12GraphicsCommandList* cmdList);
		void CreateBufferView() override;
		void Bind(ID3D12GraphicsCommandList* cmdList);
	private:
		std::vector<UINT32> mIndices;
		UINT64 mByteSize;
		ID3D12Device* mBufferDevice;
		ComPtr<ID3D12Resource> mIndexBufferGPU = nullptr;
		ComPtr<ID3D12Resource> mIndexBufferUploader = nullptr;
		D3D12_INDEX_BUFFER_VIEW mIndexBufferView;
	};
}