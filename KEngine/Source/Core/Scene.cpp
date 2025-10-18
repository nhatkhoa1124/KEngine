#include "KEngine.h"
#include "Scene.h"
#include "Math/Matrix.h"

namespace KEngine
{
	Scene::Scene(HWND handle)
	{
#ifdef WIN32
		mRenderer = CreateDX12Renderer(handle);
#endif
	}

	void Scene::Initialize()
	{
		mRenderer->Initialize();
		mMeshes.reserve(MAX_OBJECTS);
	}

	void Scene::AddObject(std::vector<Vertex>& vertices, std::vector<UINT32> indices)
	{
		std::unique_ptr<IMeshFactory> meshFactory = mRenderer->GetMeshFactory();
		std::unique_ptr<IMesh> mesh = meshFactory->CreateMesh();
		mRenderer->ResetCommandList();
		mesh->SetMesh(vertices, indices);
		mesh->CreateVertexBuffer();
		mesh->CreateIndexBuffer();
		mRenderer->CloseCommandList();
		mRenderer->ExecuteCommandList();
		mRenderer->FlushCommandList();
		mMeshes.push_back(std::move(mesh));
	}

	void Scene::Draw()
	{
		mRenderer->BeginFrame();
		Matrix matrix = {};
		matrix.Translation(1.0f, 2.0f, 0.0f);
		for (auto& m : mMeshes)
		{
			mRenderer->DrawMesh(m.get(), matrix);
		}
		mRenderer->EndFrame();
	}

	void Scene::Exit()
	{
		mRenderer->Exit();
	}
}