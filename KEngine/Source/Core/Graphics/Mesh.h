#pragma once

#include <DirectXMath.h>

namespace KEngine
{
	struct Vertex
	{
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT3 Color;
	};

	struct ObjectConstants
	{
		DirectX::XMFLOAT4X4 WorldViewProj;
	};

	class IMesh
	{
	public:
		virtual ~IMesh() = default;
		virtual void Initialize() = 0;
		virtual void Render() = 0;
		virtual void Shutdown() = 0;

		virtual size_t GetVertexCount() const = 0;
		virtual size_t GetIndexCount() const = 0;

		virtual void SetPosition(float x, float y, float z) = 0;
		virtual void SetRotation(float x, float y, float z) = 0;
		virtual void SetScale(float x, float y, float z) = 0;
	};
}
