#pragma once

#include "DX12Core.h"
#include "d3dx12_include/directx/d3dx12.h"
#include <wrl/client.h>
#include <d3dcommon.h>
#include <d3d12.h>
#include "VertexShader.h"
#include "PixelShader.h"

namespace KRender
{
	using Microsoft::WRL::ComPtr;
	class KRENDER_API DX12Shader
	{
	public:
		DX12Shader(ID3D12Device* device, bool MsaaState4x, UINT32 MsaaQualityLevel4x);
		~DX12Shader() = default;

		void CreateRootSignature();
		void CreateDefaultDesc();
		void CreatePipelineStateObject();
		void Bind(ID3D12GraphicsCommandList* cmdList, ID3D12DescriptorHeap* cbvHeap, UINT cbvIndex = 0);
		void BindPSO(ID3D12GraphicsCommandList* cmdList);
		void BindRootSignature(ID3D12GraphicsCommandList* cmdList);

		bool LoadVertexShader(const std::wstring& filename, const D3D_SHADER_MACRO* defines, const std::string& entryPoint);
		bool LoadPixelShader(const std::wstring& filename, const D3D_SHADER_MACRO* defines, const std::string& entryPoint);
		bool LoadVertexShaderBinary(const std::wstring& filename);
		bool LoadPixelShaderBinary(const std::wstring& filename);

		inline ID3D12RootSignature* GetRootSignature() const { return mRootSignature.Get(); }
		inline ID3D12PipelineState* GetPSO() const { return mPSO.Get(); }
		inline const VertexShader GetVertexShader() const { return mVertexShader; }
		inline const PixelShader GetPixelShader() const { return mPixelShader; }
		inline bool HasVertexShader() const { return mVertexShader.IsCompiled(); }
		inline bool HasPixelShader() const { return mPixelShader.IsCompiled(); }

		inline void AddInputElement(const D3D12_INPUT_ELEMENT_DESC& element) { mInputLayout.push_back(element); }
		inline void SetInputLayout(std::vector<D3D12_INPUT_ELEMENT_DESC>& inputLayout) { mInputLayout = inputLayout; }
		inline void SetRasterizerDesc(const D3D12_RASTERIZER_DESC& desc) { mRasDesc = desc; }
		inline void SetBlendDesc(const D3D12_BLEND_DESC& desc) { mBlendDesc = desc; }
		inline void SetDepthStencilDesc(const D3D12_DEPTH_STENCIL_DESC& desc) { mDepthStencilDesc = desc; }
		inline void SetFormat(DXGI_FORMAT backBufferFormat, DXGI_FORMAT depthStencilFormat)
		{
			mBackBufferFormat = backBufferFormat;
			mDepthStencilFormat = depthStencilFormat;
		}
	private:
		VertexShader mVertexShader;
		PixelShader mPixelShader;

		ComPtr<ID3D12RootSignature> mRootSignature;
		ComPtr<ID3D12PipelineState> mPSO;
		ID3D12Device* mShaderDevice;

		std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;
		D3D12_RASTERIZER_DESC mRasDesc;
		D3D12_BLEND_DESC mBlendDesc;
		D3D12_DEPTH_STENCIL_DESC mDepthStencilDesc;

		DXGI_FORMAT mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
		DXGI_FORMAT mDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
		bool m4xMsaaState;
		UINT32 m4xMsaaQualityLevel;
		UINT mCbvSrvUavDescriptorSize;
	};
}