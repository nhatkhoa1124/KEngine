#pragma once

#include "Platform/WIN32/IApplication.h"

class Application : public KEngine::Simulation
{
public:
	Application();
	~Application();
	void SetupPerGameSettings();
	void Init();
	void Update();
};