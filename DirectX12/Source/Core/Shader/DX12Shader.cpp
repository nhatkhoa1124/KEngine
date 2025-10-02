#include "dx12pch.h"
#include "DX12Shader.h"
#include "Utils/DX12Helper.h"

namespace KRender
{
	DX12Shader::DX12Shader(ID3D12Device* device, bool MsaaState4x, UINT32 MsaaQualityLevel4x) :
		mShaderDevice{ device },
		m4xMsaaState{ MsaaState4x },
		m4xMsaaQualityLevel{ MsaaQualityLevel4x },
		mVertexShader{},
		mPixelShader{}
	{
		mCbvSrvUavDescriptorSize = mShaderDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		CreateDefaultDesc();
		CreateRootSignature();
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

		if (errorBlob)
			OutputDebugString((WCHAR*)errorBlob->GetBufferPointer());
		ThrowIfFailed(hr);
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

	void DX12Shader::Bind(ID3D12GraphicsCommandList* cmdList, ID3D12DescriptorHeap* cbvHeap, UINT cbvIndex)
	{
		cmdList->SetPipelineState(mPSO.Get());
		cmdList->SetGraphicsRootSignature(mRootSignature.Get());

		if (cbvHeap)
		{
			ID3D12DescriptorHeap* descHeaps[] = { cbvHeap };
			cmdList->SetDescriptorHeaps(_countof(descHeaps), descHeaps);

			CD3DX12_GPU_DESCRIPTOR_HANDLE cbv{ cbvHeap->GetGPUDescriptorHandleForHeapStart() };
			cbv.Offset(cbvIndex, mCbvSrvUavDescriptorSize);
			cmdList->SetGraphicsRootDescriptorTable(0, cbv);
		}
	}

	void DX12Shader::BindPSO(ID3D12GraphicsCommandList* cmdList)
	{
		cmdList->SetPipelineState(mPSO.Get());
	}

	void DX12Shader::BindRootSignature(ID3D12GraphicsCommandList* cmdList)
	{
		cmdList->SetGraphicsRootSignature(mRootSignature.Get());
	}

	bool DX12Shader::LoadVertexShader(const std::wstring& filename, const D3D_SHADER_MACRO* defines,
		const std::string& entryPoint)
	{
		mVertexShader.CompileShader(filename, defines, entryPoint, VERTEX_SHADER_TARGET);
		return mVertexShader.IsCompiled();
	}

	bool DX12Shader::LoadPixelShader(const std::wstring& filename, const D3D_SHADER_MACRO* defines,
		const std::string& entryPoint)
	{
		mPixelShader.CompileShader(filename, defines, entryPoint, PIXEL_SHADER_TARGET);
		return mPixelShader.IsCompiled();
	}

	bool DX12Shader::LoadVertexShaderBinary(const std::wstring& filename)
	{
		mVertexShader.LoadShaderBinary(filename);
		return mVertexShader.IsCompiled();

	}

	bool DX12Shader::LoadPixelShaderBinary(const std::wstring& filename)
	{
		mPixelShader.LoadShaderBinary(filename);
		return mPixelShader.IsCompiled();
	}

	void DX12Shader::CreateDefaultDesc()
	{
		mRasDesc = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		mBlendDesc = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		mDepthStencilDesc = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	}

	void DX12Shader::CreatePipelineStateObject()
	{
		ID3DBlob* vsByteCode = mVertexShader.GetByteCode();
		ID3DBlob* psByteCode = mPixelShader.GetByteCode();

		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
		ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
		psoDesc.pRootSignature = mRootSignature.Get();
		if (HasVertexShader())
		{
			psoDesc.VS = { reinterpret_cast<BYTE*>(vsByteCode->GetBufferPointer()), vsByteCode->GetBufferSize() };
		}
		if (HasPixelShader())
		{
			psoDesc.PS = { reinterpret_cast<BYTE*>(psByteCode->GetBufferPointer()), psByteCode->GetBufferSize() };
		}
		psoDesc.BlendState = mBlendDesc;
		psoDesc.RasterizerState = mRasDesc;
		psoDesc.DepthStencilState = mDepthStencilDesc;
		psoDesc.SampleMask = UINT_MAX;
		psoDesc.InputLayout = { mInputLayout.data(), (UINT)mInputLayout.size() };
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		psoDesc.NumRenderTargets = 1;
		psoDesc.RTVFormats[0] = mBackBufferFormat;
		psoDesc.DSVFormat = mDepthStencilFormat;
		psoDesc.SampleDesc.Count = m4xMsaaState ? 4 : 1;
		psoDesc.SampleDesc.Quality = m4xMsaaState ? (m4xMsaaQualityLevel - 1) : 0;

		ThrowIfFailed(mShaderDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(mPSO.GetAddressOf())));
	}
}