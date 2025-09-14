#pragma once

#include <string>
#include <functional>

namespace KTime
{
	std::wstring KENGINE_API GetTime(bool stripped = false);
	std::wstring KENGINE_API GetDate(bool stripped = false);
	std::wstring KENGINE_API GetDateTimeString(bool stripped = false);

	class KENGINE_API GameTimer
	{
	public:
		using FPSUpdateCallback = std::function<void(float fps, float mspf)>;

		GameTimer(const GameTimer&) = delete;
		GameTimer& operator=(const GameTimer&) = delete;
		static GameTimer& GetInstance();

		inline float GetGameTime() const
		{
			return static_cast<float>(mBaseTime);
		}
		inline float GetDeltaTime() const
		{
			return static_cast<float>(mDeltaTime);
		}
		inline float GetTotalTime() const
		{
			if (mStopped)
			{
				return static_cast<float>(((mStopTime - mPausedTime) - mBaseTime) * mSecondsPerCount);
			}
			return static_cast<float>(((mCurrTime - mPausedTime) - mBaseTime) * mSecondsPerCount);
		}
		inline void SetFpsUpdateCallback(FPSUpdateCallback callback)
		{
			mFpsCallback = callback;
		}
		void CalculateFrameStats();
		void Reset();
		void Start();
		void Stop();
		void Tick();
	public:
		inline float GetFPS() const
		{
			return mFps;
		}
		inline float GetMSPF() const
		{
			return mMspf;
		}
	private:
		GameTimer();
		~GameTimer() = default;

		FPSUpdateCallback mFpsCallback;

		double mSecondsPerCount;
		double mDeltaTime;
		__int64 mBaseTime;
		__int64 mPausedTime;
		__int64 mStopTime;
		__int64 mPrevTime;
		__int64 mCurrTime;
		float mFps;
		float mMspf;
		bool mStopped;
	};
}