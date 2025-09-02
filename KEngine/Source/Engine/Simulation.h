#pragma once

#include "Core/Platform/WIN32/IApplication.h"

namespace KEngine
{
	class KENGINE_API Simulation : public Win32::IApplication
	{
	public:
		Simulation();
		~Simulation();
		void SetupPerGameSettings();
		void Initialize();
		void Update();
		void Exit();
	};
}