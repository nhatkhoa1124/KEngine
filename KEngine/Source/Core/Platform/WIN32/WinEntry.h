#pragma once

#include "KEngine.h"
#include "IApplication.h"

extern Win32::IApplication* EntryApplication();
INT CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	auto EntryApp = EntryApplication();

	PerGameSettings& settings = PerGameSettings::GetInstance();
	Win32::EngineWindow window = {};
	window.InitializeWindow(settings.GetGameName(), settings.GetShortName(), 1280, 720);

	KTime::GameTimer& timer = KTime::GameTimer::GetInstance();
	timer.SetFpsUpdateCallback
	(
		[&window](float fps, float mspf)
		{
			window.SetFrameStatsText(fps, mspf);
		}
	);
	EntryApp->SetupPerGameSettings();
	EntryApp->Initialize();
	timer.Reset();

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
			timer.Tick();
			timer.CalculateFrameStats();
			window.RenderWindow();
			EntryApp->Update();
		}
	}
	EntryApp->Exit();

	return 0;
}