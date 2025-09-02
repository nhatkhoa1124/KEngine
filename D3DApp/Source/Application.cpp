#include "pch.h"
#include "Application.h"
#include "Core/Platform/WIN32/WinEntry.h"

ENTRYAPP(Application)

Application::Application()
{

}

Application::~Application()
{

}

void Application::SetupPerGameSettings()
{
	PerGameSettings& settings = PerGameSettings::GetInstance();
	settings.SetGameName(IDS_PERGAMENAME);
	settings.SetShortName(IDS_SHORTNAME);
	settings.SetMainIcon(IDI_MAINICON);
}

void Application::Initialize()
{
	Logger::GetInstance().PrintLog(L"hello %s", L"dx12");
	MessageBox(0, L"Loaded up", 0, 0);
}

void Application::Update()
{
	MessageBox(0, L"Updating", 0, 0);
}

void Application::Exit()
{

}