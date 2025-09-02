#pragma once

#include "Platform/WIN32/IApplication.h"

class KENGINE_API CoreEngine;

namespace Engine
{
	extern CoreEngine gKEngine;

	enum EngineMode : INT
	{
		NONE,
		DEBUG,
		RELEASE,
		EDITOR,
		SERVER
	};
}

class KENGINE_API CoreEngine
{
public:
	CoreEngine();
	~CoreEngine();
private:
	Engine::EngineMode mEngineMode;
};
