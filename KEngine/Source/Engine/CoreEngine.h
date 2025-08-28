#pragma once

#include "Platform/WIN32/IApplication.h"

namespace KEngine
{
	enum EngineMode : INT
	{
		NONE,
		DEBUG,
		RELEASE,
		EDTIOR,
		SERVER
	};

	class KENGINE_API Simulation : public Win32::IApplication
	{
	public:
		Simulation();
		~Simulation();
	private:
		EngineMode mEngineMode;
	};
}