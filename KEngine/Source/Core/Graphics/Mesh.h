#pragma once

#include "Core/Math/Vector.h"
#include "Core/Math/Matrix.h"

namespace KEngine
{
	struct Vertex
	{
		Vector3 Position;
		Vector3 Color;
	};

	struct ObjectConstants
	{
		DirectX::XMFLOAT4X4 WorldViewProj;
	};
}

namespace KEngine
{
	class IMesh
	{
	public:
		virtual ~IMesh() = default;
		virtual void SetMesh(std::vector<Vertex>& vertices, std::vector<UINT32> indices) = 0;
		virtual void DrawMesh() = 0;
		virtual void Shutdown() = 0;
		virtual void CreateVertexBuffer() = 0;
		virtual void CreateIndexBuffer() = 0;

		virtual size_t GetVertexCount() const = 0;
		virtual size_t GetIndexCount() const = 0;

		virtual void SetPosition(float x, float y, float z) = 0;
		virtual void SetRotation(float x, float y, float z) = 0;
		virtual void SetScale(float x, float y, float z) = 0;
	};
}

namespace KEngine
{
	class IMeshFactory
	{
	public:
		virtual ~IMeshFactory() = default;
		virtual std::unique_ptr<IMesh> CreateMesh() = 0;
	};
}
