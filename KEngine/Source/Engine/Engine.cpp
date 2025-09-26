#include "KEngine.h"

#include <string>
#include <exception>

namespace KEngine
{
	KTime::GameTimer& EngineTimer = KTime::GameTimer::GetInstance();

	template <typename RendererType>
	Engine<RendererType>::Engine(Win32::IApplication* EntryApp) :
		mWindow{ nullptr },
		mRenderer{ nullptr }
	{
		mApplication = EntryApp;
	}

	template<typename RendererType>
	bool Engine<RendererType>::Initialize()
	{
		try
		{
			PerGameSettings& settings = PerGameSettings::GetInstance();
			mWindow = std::make_unique<Win32::EngineWindow>();
			mWindow->InitializeWindow(settings.GetGameName(), settings.GetShortName(), 1280, 720);

			mRenderer = std::make_unique<RendererType>(mWindow->GetHandle());
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

	template<typename RendererType>
	void Engine<RendererType>::Run()
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

	template<typename RendererType>
	void Engine<RendererType>::Shutdown()
	{
		if (mApplication)
		{
			mApplication->Exit();
		}
		mRenderer->Exit();
		mWindow->CloseWindow();
	}

	template<typename RendererType>
	void Engine<RendererType>::Render()
	{
		mWindow->RenderWindow();
		mRenderer->Draw();
	}
	// Concrete instantiation
	template class KENGINE_API Engine<KRender::DX12Renderer>;
}