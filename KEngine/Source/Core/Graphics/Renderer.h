#pragma once

#include "Core/CoreDefines.h"

namespace KEngine
{
	class IMesh;
	class IMeshFactory;
	class Matrix;
	class Color;
}

namespace KEngine
{
	class IRenderer {
	public:
		virtual ~IRenderer() = default;
		virtual void Initialize() = 0;
		virtual void Exit() = 0;
		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;
		virtual void Clear(const Color& color) = 0;
		virtual void DrawMesh(IMesh* mesh, const Matrix& transform) = 0;
		virtual std::unique_ptr<IMeshFactory> GetMeshFactory() const = 0;

		virtual void ResetCommandList() = 0;
		virtual void CloseCommandList() = 0;
		virtual void ExecuteCommandList() = 0;
		virtual void FlushCommandList() = 0;
	};

	extern KENGINE_API std::unique_ptr<IRenderer> CreateDX12Renderer(HWND handle);
}