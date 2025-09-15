#include "KEngine.h"
#include <fstream>
#include <ShlObj.h>
#include <mutex>

std::atomic<Logger*> Logger::instance{ nullptr };
std::mutex Logger::instanceMutex;

Logger::Logger() : logMutex{}
{
}

Logger::~Logger()
{
}

Logger& Logger::GetInstance()
{
	Logger* tmp = instance.load(std::memory_order_acquire);
	if (tmp == nullptr)
	{
		std::lock_guard<std::mutex> lock(instanceMutex);
		tmp = instance.load(std::memory_order_relaxed);
		if (tmp == nullptr)
		{
			tmp = new Logger();
			instance.store(tmp, std::memory_order_release);
		}
	}
	return *tmp;
}

void Logger::PrintLog(const WCHAR* fmt, ...)
{
	std::lock_guard<std::mutex> lock(logMutex);
	WCHAR buf[4096];
	va_list args;
	va_start(args, fmt);
	vswprintf_s(buf, _countof(buf), fmt, args);
	va_end(args);

	OutputDebugString(buf);

	std::wfstream outfile;
	outfile.open(LogDirectory() + L"\\" + LogFile(), std::ios_base::app);

	if (outfile.is_open())
	{
		std::wstring s = buf;
		outfile << L'[' << KTime::GetDateTimeString() << L']' << s;
		outfile.close();
		OutputDebugString(s.c_str());
	}
	else
	{
		MessageBox(nullptr, L"Unable to open log file.", L"Log Error", 0);
	}

}

std::wstring Logger::LogDirectory()
{
	PWSTR appDataLocal = nullptr;
	HRESULT result = SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &appDataLocal);
	if (FAILED(result) || appDataLocal == nullptr)
	{
		CoTaskMemFree(appDataLocal);
		return L"";
	}

	std::wstring basePath = appDataLocal;
	std::wstring gamePath = basePath + L"\\" + PerGameSettings::GetInstance().GetGameName();
	std::wstring logPath = gamePath + L"\\Log";

	CreateDirectoryW(gamePath.c_str(), nullptr);
	CreateDirectoryW(logPath.c_str(), nullptr);
	CoTaskMemFree(appDataLocal);

	return logPath;
}

std::wstring Logger::LogFile()
{
	WCHAR file[1024];
	wcscpy_s(file, _countof(file), PerGameSettings::GetInstance().GetGameName());
	wcscat_s(file, _countof(file), PerGameSettings::GetInstance().GetBootTime());
	wcscat_s(file, _countof(file), L".log");
	return file;
}



