#pragma once

#include "KEngine.h"
#include "IApplication.h"

extern Win32::IApplication* EntryApplication();

INT CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	try
	{
		CoreEngine::Engine engine{ EntryApplication() };
		if (engine.Initialize())
		{
			engine.Run();
		}
		engine.Shutdown();
		return 0;
	}
	catch (KRender::DxException& e)
	{
		MessageBox(nullptr, e.ToString().c_str(), L"HR Failed", MB_OK);
		return -1;
	}

}