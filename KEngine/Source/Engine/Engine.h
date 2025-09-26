#pragma once

#include <memory>
#include "Platform/WIN32/IApplication.h"
#include "DX12Renderer.h"
#include "Core/Graphics/IRenderer.h"

namespace KEngine
{
	template <typename RendererType>
	class KENGINE_API Engine
	{
		// Ensure RendererType is derived from IRenderer (Assert at compile time)
		static_assert(std::is_base_of_v<IRenderer, RendererType>,
			"RendererType must derive from IRenderer");
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
		std::unique_ptr<RendererType> mRenderer;
		Win32::IApplication* mApplication = nullptr;
		bool mIsRunning = false;
	};

}