#pragma once

#include <DirectXMath.h>

namespace DX12Math
{
	using namespace DirectX;

	inline XMFLOAT4X4 Identity4x4()
	{
		XMFLOAT4X4 result;
		XMStoreFloat4x4(&result, XMMatrixIdentity());
		return result;
	}
}