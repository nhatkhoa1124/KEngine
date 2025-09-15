#pragma once

#include <memory>
#include "Platform/WIN32/IApplication.h"

namespace CoreEngine
{
	class KENGINE_API Engine
	{
	public:
		Engine(Win32::IApplication* EntryApp);
		~Engine() = default;
		bool Initialize();
		void Run();
		void Shutdown();
	private:
		void Render();
	private:
		std::unique_ptr<Win32::EngineWindow> mWindow;
		std::unique_ptr<KRender::Renderer> mRenderer;
		Win32::IApplication* mApplication = nullptr;
		bool mIsRunning = false;
	};
}