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
