#include "pch.h"
#include "Application.h"
#include "Core/Platform/WIN32/WinEntry.h"

ENTRYAPP(Application)

Application::Application() :
	mAppWindow{}
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
	mAppWindow.InitializeWindow(L"Title", L"Class name", 1280, 720);
}

void Application::Update()
{
	mAppWindow.UpdateWindow(mAppWindow.GetHandle());
}

void Application::Exit()
{

}