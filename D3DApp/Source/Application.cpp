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
}

void Application::Update()
{

}

void Application::Exit()
{

}