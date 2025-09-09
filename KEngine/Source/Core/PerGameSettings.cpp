#include "KEngine.h"

#include "cassert"

std::atomic<PerGameSettings*> PerGameSettings::mInstance{ nullptr };
std::mutex PerGameSettings::creationMutex;

PerGameSettings& PerGameSettings::GetInstance()
{
	PerGameSettings* tmp = mInstance.load(std::memory_order_acquire);
	if (tmp == nullptr)
	{
		std::lock_guard<std::mutex> lock(creationMutex);
		tmp = mInstance.load(std::memory_order_relaxed);
		if (tmp == nullptr)
		{
			tmp = new PerGameSettings();
			mInstance.store(tmp, std::memory_order_release);
		}
	}
	return *tmp;
}

PerGameSettings::PerGameSettings() : mMainIcon{ nullptr }
{
	wcscpy_s(mGameName, _countof(mGameName), L"GameName");
	wcscpy_s(mShortName, _countof(mShortName), L"ShortName");
	wcscpy_s(mBootTime, _countof(mBootTime), KTime::GetDateTimeString(true).c_str());
}

PerGameSettings::~PerGameSettings()
{
	if (mMainIcon)
	{
		DestroyIcon(mMainIcon);
	}
}

void PerGameSettings::SetGameName(UINT id)
{
	LoadString(HInstance(), id, mGameName, MAX_NAME_STRING);
}

void PerGameSettings::SetShortName(UINT id)
{
	LoadString(HInstance(), id, mShortName, MAX_NAME_STRING);
}

void PerGameSettings::SetMainIcon(UINT id)
{
	if (mMainIcon) {
		DestroyIcon(mMainIcon);
	}
	mMainIcon = LoadIcon(HInstance(), MAKEINTRESOURCE(id));
}
