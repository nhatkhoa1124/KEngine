#include "KEngine.h"

#include <ctime>
#include <sstream>
#include <iomanip>

std::wstring KENGINE_API KTime::GetTime(bool stripped)
{
	std::time_t now = time(nullptr);
	std::tm ltm;
	localtime_s(&ltm, &now);
	std::wstringstream wss;
	wss << std::put_time(&ltm, L"%T");
	std::wstring timeString = wss.str();

	if (stripped)
	{
		WCHAR c = L':';
		timeString.erase(std::remove(timeString.begin(), timeString.end(), c), timeString.end());
	}

	return timeString;
}

std::wstring KENGINE_API KTime::GetDate(bool stripped)
{
	std::time_t now = time(nullptr);
	std::tm ltm;
	localtime_s(&ltm, &now);
	std::wstringstream wss;
	wss << std::put_time(&ltm, L"%d/%m/%y");
	std::wstring timeString = wss.str();

	if (stripped)
	{
		WCHAR c = L'/';
		timeString.erase(std::remove(timeString.begin(), timeString.end(), c), timeString.end());
	}

	return timeString;
}

std::wstring KENGINE_API KTime::GetDateTimeString(bool stripped)
{
	std::wstring timeString = GetDate(stripped) + L" " + GetTime(stripped);

	if (stripped)
	{
		WCHAR c = L' ';
		timeString.erase(std::remove(timeString.begin(), timeString.end(), c), timeString.end());
	}
	return timeString;
}

namespace KTime
{
	GameTimer::GameTimer() :
		mSecondsPerCount{ 0.0 },
		mDeltaTime{ -1.0 },
		mBaseTime{ 0 },
		mPausedTime{ 0 },
		mStopTime{ 0 },
		mPrevTime{ 0 },
		mCurrTime{ 0 },
		mStopped{ false }
	{
		__int64 countsPerSec;
		QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&countsPerSec));
		mSecondsPerCount = 1.0 / static_cast<double>(countsPerSec);
	}
}