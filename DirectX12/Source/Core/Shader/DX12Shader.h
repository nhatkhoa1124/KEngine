#pragma once

#include "d3dx12_include/directx/d3dx12.h"
#include <wrl/client.h>
#include <d3dcommon.h>
#include <d3d12.h>
#include "Utils/DX12Helper.h"

namespace KRender
{
	using Microsoft::WRL::ComPtr;
	class DX12Shader
	{
	public:
		DX12Shader(ID3D12Device* device);
		~DX12Shader() = default;
		void CreateRootSignature();
		void BindRootSignature(ID3D12GraphicsCommandList* cmdList, ID3D12DescriptorHeap* cbvHeap, UINT cbvIndex = 0);
	private:
		ID3D12Device* mShaderDevice;
		ComPtr<ID3D12RootSignature> mRootSignature;
		UINT mCbvSrvUavDescriptorSize;
	};
}