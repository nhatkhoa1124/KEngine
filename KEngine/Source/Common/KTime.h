#pragma once

#include <string>

namespace KTime
{
	std::wstring KENGINE_API GetTime(bool stripped = false);
	std::wstring KENGINE_API GetDate(bool stripped = false);
	std::wstring KENGINE_API GetDateTimeString(bool stripped = false);

	class GameTimer
	{
	public:
		GameTimer();
		~GameTimer();
		float GetGameTime() const;
		float GetDeltaTime() const;

		void Reset();
		void Start();
		void Stop();
		void Tick();
	private:
		double mSecondsPerCount;
		double mDeltaTime;
		__int64 mBaseTime;
		__int64 mPausedTime;
		__int64 mStopTime;
		__int64 mPrevTime;
		__int64 mCurrTime;
		bool mStopped;
	};
}