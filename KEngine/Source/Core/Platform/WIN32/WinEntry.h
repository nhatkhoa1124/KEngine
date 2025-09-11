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
	MSG msg = {};
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			EntryApp->Update();
		}
	}
	EntryApp->Exit();

	return 0;
}