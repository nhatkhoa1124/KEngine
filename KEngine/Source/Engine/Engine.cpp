#include "KEngine.h"

#include <string>
#include <exception>

namespace CoreEngine
{
	KTime::GameTimer& EngineTimer = KTime::GameTimer::GetInstance();

	Engine::Engine(Win32::IApplication* EntryApp) :
		mWindow{ nullptr },
		mRenderer{ nullptr }
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

			mRenderer = std::make_unique<KRender::Renderer>(mWindow->GetHandle());
			mRenderer->Initialize();

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
				Render();
				mApplication->Update();
			}
		}
	}

	void Engine::Shutdown()
	{
		if (mApplication)
		{
			mApplication->Exit();
		}
		mRenderer->Exit();
		mWindow->CloseWindow();
	}

	void Engine::Render()
	{
		mWindow->RenderWindow();
		mRenderer->Draw();
	}
}