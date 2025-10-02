#pragma once

#include "DX12Core.h"
#include "Core/Graphics/Renderer.h"
#include "d3dx12_include/directx/d3dx12.h"
#include <wrl/client.h>
#include <d3d12.h>
#include <vector>

namespace KRender
{
	using Microsoft::WRL::ComPtr;
	class KRENDER_API DX12IndexBuffer : public IBuffer
	{
	public:
		DX12IndexBuffer(ID3D12Device* device, std::vector<UINT32> indices);
		~DX12IndexBuffer() = default;
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