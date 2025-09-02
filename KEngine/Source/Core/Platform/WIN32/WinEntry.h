#pragma once

#include "KEngine.h"
#include "IApplication.h"

extern Win32::IApplication* EntryApplication();

INT CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	auto EntryApp = EntryApplication();

	PerGameSettings& gameSettings = PerGameSettings::GetInstance();
	EntryApp->SetupPerGameSettings();
	Logger& logger = Logger::GetInstance();
	EntryApp->Initialize();
	EntryApp->Update();
	EntryApp->Exit();

	return 0;
}