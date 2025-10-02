#pragma once

#include "DX12Core.h"
#include <d3d12.h>

namespace KRender
{
	class KRENDER_API DX12Viewport
	{
	public:
		DX12Viewport(ID3D12GraphicsCommandList* cmdList);
		DX12Viewport(ID3D12GraphicsCommandList* cmdList, float topLeftX, float topLeftY, float width, float height,
			float minDepth = 0.0f, float maxDepth = 1.0f);
		~DX12Viewport() = default;

		void SetViewport(float topLeftX, float topLeftY, float width, float height,
			float minDepth = 0.0f, float maxDepth = 1.0f);
		void SetScissorRectangle(int left, int top, int right, int bottom);
		void SetDimensions(float width, float height);
		void SetPosition(float topLeftX, float topLeftY);
		void SetDepthRange(float minDepth, float maxDepth);

		inline const D3D12_VIEWPORT& GetViewport() const { return mViewport; }
		inline const D3D12_RECT& GetScissorRect() const { return mScissorRect; }
		inline D3D12_VIEWPORT& GetViewport() { return mViewport; } // Get modifiable viewport
		inline D3D12_RECT& GetScissorRect() { return mScissorRect; } // Get modifiable scissor rectangle
		inline float GetWidth() const { return mViewport.Width; }
		inline float GetHeight() const { return mViewport.Height; }
		inline float GetTopLeftX() const { return mViewport.TopLeftX; }
		inline float GetTopLeftY() const { return mViewport.TopLeftY; }

		void Apply();
		void SetToRenderTargetSize(float width, float height);
		void SetAspectRatio(float aspectRatio);
	private:
		D3D12_VIEWPORT mViewport;
		D3D12_RECT mScissorRect;
		ID3D12GraphicsCommandList* mCmdList;
	};
}