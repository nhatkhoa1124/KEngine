#pragma once

#include <vector>
#include <memory>
#include "Graphics/Mesh.h"
#include "Graphics/Renderer.h"

namespace KEngine
{
	class Scene
	{
	public:
		Scene(HWND handle);
		~Scene() = default;
		void Initialize();
		void AddObject(std::vector<Vertex>& vertices, std::vector<UINT32> indices);
		void Draw();
		void Exit();
	private:
		std::vector<std::unique_ptr<IMesh>> mMeshes;
		std::unique_ptr<IRenderer> mRenderer;

		static constexpr int MAX_OBJECTS = 10;
	};
}