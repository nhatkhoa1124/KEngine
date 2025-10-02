#include "dx12pch.h"
#include "DX12CommandContext.h"
#include "Utils/DX12Helper.h"
#include <stdexcept>

namespace KRender
{

	DX12CommandContext::~DX12CommandContext()
	{
		if (mFence && mFenceEvent)
		{
			Flush();
		}
		if (mFenceEvent)
		{
			CloseHandle(mFenceEvent);
		}
	}

	void DX12CommandContext::Initialize(ID3D12Device* device)
	{
		mDevice = device;

		mFenceEvent = CreateEventEx(nullptr, nullptr, false, EVENT_ALL_ACCESS);
		if (mFenceEvent == nullptr)
		{
			throw std::runtime_error("Failed to create fence event.");
		}
		CreateCommandQueue();
		CreateCommandAllocator();
		CreateCommandList();
		CreateFence();
		Close();
	}

	void DX12CommandContext::Reset()
	{
		ThrowIfFailed(mCmdAllocator->Reset());
		ThrowIfFailed(mCmdList->Reset(mCmdAllocator.Get(), nullptr));
	}

	void DX12CommandContext::Close()
	{
		ThrowIfFailed(mCmdList->Close());
	}

	void DX12CommandContext::Execute()
	{
		ID3D12CommandList* commandLists[] = { mCmdList.Get() };
		mCmdQueue->ExecuteCommandLists(_countof(commandLists), commandLists);
		mCurrentFenceValue++;
		ThrowIfFailed(mCmdQueue->Signal(mFence.Get(), mCurrentFenceValue));
	}

	void DX12CommandContext::Flush()
	{
		mCurrentFenceValue++;
		ThrowIfFailed(mCmdQueue->Signal(mFence.Get(), mCurrentFenceValue));
		WaitForFenceValue(mCurrentFenceValue);
	}

	void DX12CommandContext::CreateCommandList()
	{
		ThrowIfFailed(mDevice->CreateCommandList(
			0,
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			mCmdAllocator.Get(),
			nullptr,
			IID_PPV_ARGS(&mCmdList)));
	}
	void DX12CommandContext::CreateCommandQueue()
	{
		D3D12_COMMAND_QUEUE_DESC queueDesc = {};
		queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		queueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
		queueDesc.NodeMask = 0;
		ThrowIfFailed(mDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&mCmdQueue)));
	}
	void DX12CommandContext::CreateCommandAllocator()
	{
		ThrowIfFailed(mDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(mCmdAllocator.GetAddressOf())));
	}
	void DX12CommandContext::CreateFence()
	{
		ThrowIfFailed(mDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mFence)));
	}
	void DX12CommandContext::WaitForFenceValue(UINT64 fenceValue)
	{
		if (mFence->GetCompletedValue() < fenceValue)
		{
			ThrowIfFailed(mFence->SetEventOnCompletion(mCurrentFenceValue, mFenceEvent));
			WaitForSingleObject(mFenceEvent, INFINITE);
			CloseHandle(mFenceEvent);
		}
	}
}