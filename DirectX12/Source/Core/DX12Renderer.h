#pragma once

#include "Core/Graphics/Mesh.h"
#include "Core/Graphics/Renderer.h"
#include "Core/Graphics/Color.h"
#include "Core/Math/Matrix.h"

#include "DX12Core.h"
#include <DirectXColors.h>

#include "Shader/DX12Shader.h"
#include "DX12Device.h"
#include "DX12CommandContext.h"
#include "DX12SwapChain.h"
#include "Memory/DX12DescriptorHeap.h"
#include "DX12Viewport.h"
#include "DX12MeshFactory.h"
#include "Buffer/DX12ConstantBuffer.h"

#include <wrl/client.h>

namespace KRender
{
	using Microsoft::WRL::ComPtr;
	class KRENDER_API DX12Renderer : public KEngine::IRenderer
	{
	public:
		DX12Renderer(HWND handle);
		~DX12Renderer();
		void Initialize() override;
		void Exit() override;
		void BeginFrame() override;
		void EndFrame() override;
		void Clear(const KEngine::Color& color) override;
		void DrawMesh(KEngine::IMesh* mesh, const KEngine::Matrix& transform) override;
		std::unique_ptr<KEngine::IMeshFactory> GetMeshFactory() const override;

		void ResetCommandList() override;
		void CloseCommandList() override;
		void ExecuteCommandList() override;
		void FlushCommandList() override;
	private:
		void CreateConstantBuffer();
		void CreateDSV();
		void UpdateObjectConstants(const KEngine::Matrix& worldMatrix);
		void InitializeMatrices();
		// API members
		HWND mHandle;
		std::unique_ptr<DX12Device> mDevice;
		std::unique_ptr<DX12DescriptorHeap> mDSVHeap;
		std::unique_ptr<DX12DescriptorHeap> mCBVHeap;
		std::unique_ptr<DX12CommandContext> mCommandContext;
		std::unique_ptr<DX12SwapChain> mSwapChain;
		std::unique_ptr<DX12Viewport> mViewport;
		std::unique_ptr<DX12Shader> mShader;

		ComPtr<ID3D12Resource> mDepthStencilBuffer;

		std::unique_ptr<DX12ConstantBuffer<KEngine::ObjectConstants>> mObjectConstants;

		// Helper members
		const DirectX::XMVECTORF32 backgroundColor = DirectX::Colors::AntiqueWhite;
		DirectX::XMMATRIX mWorldMatrix;
		DirectX::XMMATRIX mViewMatrix;
		DirectX::XMMATRIX mProjectionMatrix;

	};
}