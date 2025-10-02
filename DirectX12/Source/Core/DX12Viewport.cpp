#include "dx12pch.h"
#include "DX12Viewport.h"
#include <algorithm>
#include <cmath>

namespace KRender
{
	DX12Viewport::DX12Viewport(ID3D12GraphicsCommandList* cmdList) :
		mViewport{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f },
		mScissorRect{ 0, 0, 0, 0 },
		mCmdList{ cmdList }
	{

	}
	DX12Viewport::DX12Viewport(ID3D12GraphicsCommandList* cmdList, float topLeftX, float topLeftY, float width, float height,
		float minDepth, float maxDepth) :
		mCmdList{ cmdList }
	{
		SetViewport(topLeftX, topLeftY, width, height, minDepth, maxDepth);
		SetScissorRectangle(topLeftX, topLeftY, topLeftX + width, topLeftY + height);
	}

	void DX12Viewport::SetViewport(float topLeftX, float topLeftY, float width, float height,
		float minDepth, float maxDepth)
	{
		mViewport.TopLeftX = topLeftX;
		mViewport.TopLeftY = topLeftY;
		mViewport.Width = std::fmax(width, 1.0f);
		mViewport.Height = std::fmax(height, 1.0f);
		mViewport.MinDepth = std::clamp(minDepth, 0.0f, 1.0f);
		mViewport.MaxDepth = std::clamp(maxDepth, 0.0f, 1.0f);
	}

	void DX12Viewport::SetScissorRectangle(int left, int top, int right, int bottom)
	{
		mScissorRect.left = std::fmin(left, right);
		mScissorRect.right = std::fmax(left, right);
		mScissorRect.top = std::fmin(top, bottom);
		mScissorRect.bottom = std::fmax(top, bottom);
	}

	void DX12Viewport::SetDimensions(float width, float height)
	{
		SetViewport(mViewport.TopLeftX, mViewport.TopLeftY, width, height, mViewport.MinDepth, mViewport.MaxDepth);
		SetScissorRectangle(mViewport.TopLeftX, mViewport.TopLeftY, mViewport.TopLeftX + width, mViewport.TopLeftY + height);
	}

	void DX12Viewport::SetPosition(float topLeftX, float topLeftY)
	{
		float width = mViewport.Width;
		float height = mViewport.Height;
		SetViewport(topLeftX, topLeftY, width, height, mViewport.MinDepth, mViewport.MaxDepth);
		SetScissorRectangle(topLeftX, topLeftY, topLeftX + width, topLeftY + height);
	}

	void DX12Viewport::SetDepthRange(float minDepth, float maxDepth)
	{
		SetViewport(mViewport.TopLeftX, mViewport.TopLeftY, mViewport.Width, mViewport.Height, minDepth, maxDepth);
	}

	void DX12Viewport::Apply()
	{
		mCmdList->RSSetViewports(1, &mViewport);
		mCmdList->RSSetScissorRects(1, &mScissorRect);
	}

	void DX12Viewport::SetToRenderTargetSize(float width, float height)
	{
		SetViewport(0.0f, 0.0f, width, height, 0.0f, 1.0f);
		SetScissorRectangle(0, 0, width, height);
	}

	void DX12Viewport::SetAspectRatio(float aspectRatio)
	{
	}

}