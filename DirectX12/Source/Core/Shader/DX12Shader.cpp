#include "dx12pch.h"
#include "DX12Shader.h"

namespace KRender
{
	DX12Shader::DX12Shader(ID3D12Device* device) :
		mShaderDevice{ device }
	{
		mCbvSrvUavDescriptorSize = mShaderDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	}
	void DX12Shader::CreateRootSignature()
	{
		CD3DX12_ROOT_PARAMETER slotRootParameter[1]{};
		CD3DX12_DESCRIPTOR_RANGE cbvTable = {};
		cbvTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
		slotRootParameter[0].InitAsDescriptorTable(1, &cbvTable);

		CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc
		{
			1,
			slotRootParameter,
			0,
			nullptr,
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
		};
		ComPtr<ID3DBlob> serializeRootSig = nullptr;
		ComPtr<ID3DBlob> errorBlob = nullptr;
		HRESULT hr = D3D12SerializeRootSignature
		(
			&rootSigDesc,
			D3D_ROOT_SIGNATURE_VERSION_1,
			serializeRootSig.GetAddressOf(),
			errorBlob.GetAddressOf()
		);

		ThrowIfFailed
		(
			mShaderDevice->CreateRootSignature
			(
				0,
				serializeRootSig->GetBufferPointer(),
				serializeRootSig->GetBufferSize(),
				IID_PPV_ARGS(mRootSignature.GetAddressOf())
			)
		);
	}

	void DX12Shader::BindRootSignature(ID3D12GraphicsCommandList* cmdList, ID3D12DescriptorHeap* cbvHeap, UINT cbvIndex)
	{
		cmdList->SetGraphicsRootSignature(mRootSignature.Get());

		ID3D12DescriptorHeap* descHeaps[] = { cbvHeap };
		cmdList->SetDescriptorHeaps(_countof(descHeaps), descHeaps);

		CD3DX12_GPU_DESCRIPTOR_HANDLE cbv{ cbvHeap->GetGPUDescriptorHandleForHeapStart() };
		cbv.Offset(cbvIndex, mCbvSrvUavDescriptorSize);
		cmdList->SetGraphicsRootDescriptorTable(0, cbv);
	}
}