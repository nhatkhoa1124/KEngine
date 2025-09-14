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
		mStopped{ false },
		mFps{ 0 },
		mMspf{ 0 }
	{
		__int64 countsPerSec;
		QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&countsPerSec));
		mSecondsPerCount = 1.0 / static_cast<double>(countsPerSec);
	}

	GameTimer& GameTimer::GetInstance()
	{
		static GameTimer instance;
		return instance;
	}

	void GameTimer::CalculateFrameStats()
	{
		static int frameCount = 0;
		static float timeElapsed = 0.0f;
		frameCount++;

		if ((GetTotalTime() - timeElapsed) >= 1.0f)
		{
			mFps = static_cast<float>(frameCount);
			mMspf = 1000.0f / mFps;


			if (mFpsCallback)
			{
				mFpsCallback(mFps, mMspf);
			}

			frameCount = 0;
			timeElapsed += 1.0f;
		}
	}

	void GameTimer::Reset()
	{
		__int64 currTime;
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currTime));

		mBaseTime = currTime;
		mPrevTime = currTime;
		mStopTime = 0;
		mStopped = false;
	}

	void GameTimer::Start()
	{
		__int64 startTime;
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&startTime));
		if (mStopped)
		{
			mPausedTime += (startTime - mStopTime);
			mPrevTime = startTime;
			mStopTime = 0;
			mStopped = false;
		}

	}

	void GameTimer::Stop()
	{
		if (!mStopped)
		{
			__int64 currTime;
			QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currTime));
			mStopTime = currTime;
			mStopped = true;
		}
	}

	void GameTimer::Tick()
	{
		if (mStopped)
		{
			mDeltaTime = 0.0;
			return;
		}

		__int64 currTime;
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currTime));
		mCurrTime = currTime;

		mDeltaTime = (mCurrTime - mPrevTime) * mSecondsPerCount;
		mPrevTime = mCurrTime;
		if (mDeltaTime < 0.0)
		{
			mDeltaTime = 0.0;
		}
	}
}