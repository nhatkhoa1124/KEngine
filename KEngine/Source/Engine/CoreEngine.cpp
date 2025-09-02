#include "KEngine.h"

namespace Engine
{
	CoreEngine gKEngine;
}

CoreEngine::CoreEngine()
{
#ifdef _DEBUG
	mEngineMode = Engine::EngineMode::DEBUG;
#else
	mEngineMode = Engine::EngineMode::RELEASE;
#endif
}

CoreEngine::~CoreEngine()
{
}
