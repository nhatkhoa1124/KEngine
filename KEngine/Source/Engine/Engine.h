#pragma once

#include <memory>
#include <vector>
#include "Platform/WIN32/IApplication.h"
#include "Core/Scene.h"

namespace KEngine
{
	class KENGINE_API Engine
	{
	public:
		Engine(Win32::IApplication* EntryApp);
		~Engine() = default;
		bool Initialize();
		void Run();
		void Shutdown();
		void CreateScene();
	private:
		void Render();
	private:
		std::unique_ptr<Win32::EngineWindow> mWindow;
		std::vector < std::unique_ptr<Scene>> mScenes;
		Win32::IApplication* mApplication = nullptr;
		bool mIsRunning = false;
	};

}