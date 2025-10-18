#include "dx12pch.h"
#include "DX12Renderer.h"
#include "Math/DX12Math.h"
#include "Utils/DX12Helper.h"
#include "DX12Mesh.h"

namespace KRender
{
	DX12Renderer::DX12Renderer(HWND handle) :
		mHandle{ handle }
	{
	}

	DX12Renderer::~DX12Renderer()
	{

	}

	void DX12Renderer::Initialize()
	{
		DXGI_FORMAT renderTargetFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
		UINT swapChainBufferCount = 2;
		bool debugEnabled = true;
		bool isWindowed = true;
		bool useDepth = true;

		InitializeMatrices();

		mDevice = std::make_unique<DX12Device>();
		mDevice->Initialize(debugEnabled);

		mCommandContext = std::make_unique<DX12CommandContext>(mDevice->GetDevice());
		mCommandContext->Initialize();

		mSwapChain = std::make_unique<DX12SwapChain>();
		mSwapChain->Initialize(*mDevice, *mCommandContext, mHandle, swapChainBufferCount, isWindowed, CLIENT_WIDTH, CLIENT_HEIGHT);

		mDSVHeap = std::make_unique<DX12DescriptorHeap>(*mDevice, D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1, false);
		CreateDSV();

		mViewport = std::make_unique<DX12Viewport>(
			mCommandContext->GetCommandList(), 0.0f, 0.0f, mSwapChain->GetWidth(), mSwapChain->GetHeight(), 0.0f, 1.0f
		);

		mShader = std::make_unique<DX12Shader>(mDevice->GetDevice(),
			mSwapChain->IsMsaa4xSupported(), mSwapChain->GetMsaa4xQualityLevel());
		mShader->LoadVertexShaderBinary
		(
			L"C:\\Tin\\Personal\\Cpp_Projects\\DirectX\\DX12\\DX12_Renderer\\Resource\\Shader\\HLSL\\Compiled\\base_vs.cso"
		);
		mShader->LoadPixelShaderBinary
		(
			L"C:\\Tin\\Personal\\Cpp_Projects\\DirectX\\DX12\\DX12_Renderer\\Resource\\Shader\\HLSL\\Compiled\\base_ps.cso"
		);
		mShader->CreatePipelineStateObject();

		CreateConstantBuffer();
	}

	void DX12Renderer::Exit()
	{

	}

	void DX12Renderer::BeginFrame()
	{
		mCommandContext->Reset();
		auto cmdList = mCommandContext->GetCommandList();
		mSwapChain->TransitionToRenderTarget(cmdList);
		D3D12_CPU_DESCRIPTOR_HANDLE rtv = mSwapChain->GetCurrentBackBufferRTV();
		D3D12_CPU_DESCRIPTOR_HANDLE dsv = mDSVHeap->GetCpuHandle(0);

		cmdList->OMSetRenderTargets(1, &rtv, true, &dsv);
		cmdList->ClearRenderTargetView(rtv, backgroundColor, 0, nullptr);
		cmdList->ClearDepthStencilView(dsv, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

		mViewport->SetToRenderTargetSize(mSwapChain->GetWidth(), mSwapChain->GetHeight());
		mViewport->Apply();

		cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	void DX12Renderer::EndFrame()
	{
		mSwapChain->TransitionToPresent(mCommandContext->GetCommandList());
		mCommandContext->Close();
		mCommandContext->Execute();
		mSwapChain->Present();
	}

	void DX12Renderer::Clear(const KEngine::Color& color)
	{
	}

	void DX12Renderer::DrawMesh(KEngine::IMesh* mesh, const KEngine::Matrix& transform)
	{
		auto dx12Mesh = dynamic_cast<DX12Mesh*>(mesh);
		UpdateObjectConstants(transform);
		mShader->Bind(mCommandContext->GetCommandList(), mObjectConstants->GetCBVHeap(), 0);
		dx12Mesh->DrawMesh();
	}

	void DX12Renderer::CreateConstantBuffer()
	{
		mObjectConstants = std::make_unique<DX12ConstantBuffer<ObjectConstants>>(mDevice->GetDevice(), 1);
		mObjectConstants->InitBuffer(mCommandContext->GetCommandList());
		mObjectConstants->CreateCbvHeap();
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = mObjectConstants->GetResource()->GetGPUVirtualAddress();
		cbvDesc.SizeInBytes = mObjectConstants->GetByteSize();
		mObjectConstants->SetBufferViewDesc(cbvDesc);
		mObjectConstants->CreateBufferView();
	}

	void DX12Renderer::CreateDSV()
	{
		bool depthSampleState = mSwapChain->IsMsaa4xSupported();
		UINT depthSampleQuality = mSwapChain->GetMsaa4xQualityLevel();
		DXGI_FORMAT depthFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

		D3D12_RESOURCE_DESC depthDesc = CD3DX12_RESOURCE_DESC::Tex2D
		(
			depthFormat,
			CLIENT_WIDTH,
			CLIENT_HEIGHT,
			1,
			0,
			depthSampleState ? 4 : 1,
			depthSampleState ? (depthSampleQuality - 1) : 0,
			D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
		);

		D3D12_CLEAR_VALUE clearValue = {};
		clearValue.Format = depthFormat;
		clearValue.DepthStencil = { 1.0f, 0 };
		auto heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		ThrowIfFailed(mDevice->GetDevice()->CreateCommittedResource
		(
			&heapProperties,
			D3D12_HEAP_FLAG_NONE,
			&depthDesc,
			D3D12_RESOURCE_STATE_DEPTH_WRITE,
			&clearValue,
			IID_PPV_ARGS(mDepthStencilBuffer.GetAddressOf())
		));

		D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
		dsvDesc.Format = depthFormat;
		dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Flags = D3D12_DSV_FLAG_NONE;

		mDevice->GetDevice()->CreateDepthStencilView
		(
			mDepthStencilBuffer.Get(),
			&dsvDesc,
			mDSVHeap->GetCpuHandle(0)
		);
	}

	void DX12Renderer::UpdateObjectConstants(const KEngine::Matrix& worldMatrix)
	{
		ObjectConstants objConstants = {};
		DirectX::XMMATRIX world = worldMatrix.ToXMMatrix();
		DirectX::XMStoreFloat4x4(&objConstants.WorldViewProj, DirectX::XMMatrixTranspose(world));
		mObjectConstants->CopyData(0, objConstants);
	}

	void DX12Renderer::InitializeMatrices()
	{
		mWorldMatrix = DirectX::XMMatrixIdentity();

		DirectX::XMVECTOR eyePosition = DirectX::XMVectorSet(0.0f, 2.0f, -5.0f, 1.0f);
		DirectX::XMVECTOR focusPoint = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
		DirectX::XMVECTOR upDirection = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		mViewMatrix = DirectX::XMMatrixLookAtLH(eyePosition, focusPoint, upDirection);

		// Projection matrix - perspective
		float aspectRatio = static_cast<float>(CLIENT_WIDTH) / CLIENT_HEIGHT;
		mProjectionMatrix = DirectX::XMMatrixPerspectiveFovLH(
			DirectX::XM_PIDIV4, // 45 degree FOV
			aspectRatio,
			1.0f,   // Near plane
			100.0f  // Far plane
		);
	}

	// Factory implementation code
	std::unique_ptr<KEngine::IMeshFactory> DX12Renderer::GetMeshFactory() const
	{
		return std::make_unique<DX12MeshFactory>(mDevice->GetDevice(), mCommandContext->GetCommandList());
	}

	void DX12Renderer::ResetCommandList()
	{
		if (!mCommandContext->IsRecording())
		{
			mCommandContext->Reset();
		}
	}

	void DX12Renderer::CloseCommandList()
	{
		if (mCommandContext->IsRecording())
		{
			mCommandContext->Close();
		}
	}

	void DX12Renderer::ExecuteCommandList()
	{
		mCommandContext->Execute();
	}

	void DX12Renderer::FlushCommandList()
	{
		mCommandContext->Flush();
	}

}

namespace KEngine
{
	KENGINE_API std::unique_ptr<IRenderer> CreateDX12Renderer(HWND handle)
	{
		return std::make_unique<KRender::DX12Renderer>(handle);
	}
}