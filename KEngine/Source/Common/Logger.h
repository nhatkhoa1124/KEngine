#pragma once

#include <string>
#include <mutex>
#include <atomic>
#include <exception>

class KENGINE_API Logger
{
public:
	Logger(const Logger&) = delete;
	Logger& operator = (const Logger&) = delete;
	static Logger& GetInstance();
	void PrintLog(const WCHAR* fmt, ...);
	std::wstring LogDirectory();
	std::wstring LogFile();
private:
	Logger();
	~Logger();
	static std::atomic<Logger*> instance;
	static std::mutex instanceMutex;
	std::mutex logMutex;
};