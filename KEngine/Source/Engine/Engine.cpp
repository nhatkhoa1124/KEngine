#include "KEngine.h"

#include <string>
#include <exception>
#include "Core/Math/Vector.h"

namespace KEngine
{
	KTime::GameTimer& EngineTimer = KTime::GameTimer::GetInstance();

	Engine::Engine(Win32::IApplication* EntryApp) :
		mWindow{ nullptr },
		mScenes{}
	{
		mApplication = EntryApp;
	}

	bool Engine::Initialize()
	{
		try
		{
			PerGameSettings& settings = PerGameSettings::GetInstance();
			mWindow = std::make_unique<Win32::EngineWindow>();
			mWindow->InitializeWindow(settings.GetGameName(), settings.GetShortName(), 1280, 720);

			EngineTimer.SetFpsUpdateCallback
			(
				[this](float fps, float mspf)
				{
					mWindow->SetFrameStatsText(fps, mspf);
				}
			);
			mApplication->SetupPerGameSettings();
			mApplication->Initialize();
			EngineTimer.Reset();
			CreateScene();

			return true;
		}
		catch (const std::exception& e)
		{
			std::wstring errorString = KException::ExceptionToWString(e);
			OutputDebugStringW(errorString.c_str());
			return false;
		}
	}

	void Engine::Run()
	{
		mIsRunning = true;
		MSG msg = {};
		static bool firstFrame = true;
		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				EngineTimer.Tick();
				EngineTimer.CalculateFrameStats();
				mWindow->RenderWindow();
				mApplication->Update();
				Render();
			}
		}
	}

	void Engine::Shutdown()
	{
		if (mApplication)
		{
			mApplication->Exit();
		}
		mWindow->CloseWindow();
	}

	void Engine::CreateScene()
	{
		std::unique_ptr<Scene> newScene = std::make_unique<Scene>(mWindow->GetHandle());
		newScene->Initialize();
		// Default cube data
		std::vector<Vertex> defaultCubeVertices = {
			// Front face
			{ Vector3(-0.5f, -0.5f, -0.5f), Vector3(1.0f, 0.0f, 0.0f) }, // 0: bottom-left-front (red)
			{ Vector3(0.5f, -0.5f, -0.5f), Vector3(0.0f, 1.0f, 0.0f) }, // 1: bottom-right-front (green)
			{ Vector3(0.5f,  0.5f, -0.5f), Vector3(0.0f, 0.0f, 1.0f) }, // 2: top-right-front (blue)
			{ Vector3(-0.5f,  0.5f, -0.5f), Vector3(1.0f, 1.0f, 0.0f) }, // 3: top-left-front (yellow)

			// Back face  
			{ Vector3(-0.5f, -0.5f,  0.5f), Vector3(1.0f, 0.0f, 1.0f) }, // 4: bottom-left-back (magenta)
			{ Vector3(0.5f, -0.5f,  0.5f), Vector3(0.0f, 1.0f, 1.0f) }, // 5: bottom-right-back (cyan)
			{ Vector3(0.5f,  0.5f,  0.5f), Vector3(1.0f, 1.0f, 1.0f) }, // 6: top-right-back (white)
			{ Vector3(-0.5f,  0.5f,  0.5f), Vector3(0.5f, 0.5f, 0.5f) }  // 7: top-left-back (gray)
		};
		std::vector<UINT32> defaultCubeIndices = {
			// Front face (2 triangles)
			0, 1, 2,  // triangle 1
			0, 2, 3,  // triangle 2

			// Back face (2 triangles)
			5, 4, 7,  // triangle 1
			5, 7, 6,  // triangle 2

			// Top face (2 triangles)
			3, 2, 6,  // triangle 1
			3, 6, 7,  // triangle 2

			// Bottom face (2 triangles)
			4, 5, 1,  // triangle 1
			4, 1, 0,  // triangle 2

			// Left face (2 triangles)
			4, 0, 3,  // triangle 1
			4, 3, 7,  // triangle 2

			// Right face (2 triangles)
			1, 5, 6,  // triangle 1
			1, 6, 2   // triangle 2
		};
		newScene->AddObject(defaultCubeVertices, defaultCubeIndices);
		mScenes.push_back(std::move(newScene));
	}

	void Engine::Render()
	{
		// Temporary hard-coded draw logic
		mScenes[0]->Draw();
	}
}