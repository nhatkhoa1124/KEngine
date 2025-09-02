#pragma once

#include "Core/Platform/WIN32/IApplication.h"

class Application : public Win32::IApplication
{
public:
	Application();
	~Application();
	void SetupPerGameSettings();
	void Initialize();
	void Update();
	void Exit();
};