#pragma once

#include "KEngine.h"
#include "IApplication.h"
#include <stdexcept>

extern Win32::IApplication* EntryApplication();

INT CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	try
	{
		KEngine::Engine engine{ EntryApplication() };

		if (engine.Initialize())
		{
			engine.Run();
		}
		engine.Shutdown();
		return 0;
	}
	catch (std::exception e)
	{
		MessageBox(nullptr, L"Error", L"HR Failed", MB_OK);
		return -1;
	}

}