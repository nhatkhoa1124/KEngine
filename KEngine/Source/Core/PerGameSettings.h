#pragma once

#include <atomic>
#include <mutex>

class KENGINE_API PerGameSettings
{
public:
	PerGameSettings(const PerGameSettings&) = delete;
	PerGameSettings& operator=(const PerGameSettings&) = delete;

	static PerGameSettings& GetInstance();

	const WCHAR* GetGameName() const { return mGameName; }
	void SetGameName(UINT id);

	const WCHAR* GetShortName() const { return mShortName; };
	void SetShortName(UINT id);

	HICON GetMainIcon() const { return mMainIcon; }
	void SetMainIcon(UINT id);

	const WCHAR* GetBootTime() const { return mBootTime; }

private:
	PerGameSettings();
	~PerGameSettings();

	static std::atomic<PerGameSettings*> instance;
	static std::mutex creationMutex;

	WCHAR mGameName[MAX_NAME_STRING];
	WCHAR mShortName[MAX_NAME_STRING];
	HICON mMainIcon;
	WCHAR mBootTime[MAX_NAME_STRING];
};