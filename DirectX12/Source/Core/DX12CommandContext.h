#pragma once

#include "DX12Core.h"
#include <wrl/client.h>
#include <d3d12.h>
#include "DX12Device.h"

namespace KRender
{
	using Microsoft::WRL::ComPtr;
	class KRENDER_API DX12CommandContext
	{
	public:
		DX12CommandContext() = default;
		~DX12CommandContext();

		void Initialize(ID3D12Device* device);
		void Reset();
		void Close();
		void Execute();
		void Flush();
		inline ID3D12GraphicsCommandList* GetCommandList() const { return mCmdList.Get(); }
		inline ID3D12CommandQueue* GetCommandQueue() const { return mCmdQueue.Get(); }
		inline ID3D12CommandAllocator* GetCommandAllocator() const { return mCmdAllocator.Get(); }
	private:
		void CreateCommandList();
		void CreateCommandQueue();
		void CreateCommandAllocator();
		void CreateFence();
		void WaitForFenceValue(UINT64 fenceValue);

		ComPtr<ID3D12GraphicsCommandList> mCmdList;
		ComPtr<ID3D12CommandQueue> mCmdQueue;
		ComPtr<ID3D12CommandAllocator> mCmdAllocator;
		ComPtr<ID3D12Fence> mFence;
		UINT64 mCurrentFenceValue = 0;
		HANDLE mFenceEvent = nullptr;
		ID3D12Device* mDevice = nullptr;
	};
}