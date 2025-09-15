#pragma once

#include <string>
#include <exception>

namespace KException
{
	inline std::wstring ExceptionToWString(const std::exception& e)
	{
		// Convert narrow string to wide string
		std::string narrowStr = e.what();
		int size_needed = MultiByteToWideChar(CP_UTF8, 0, narrowStr.c_str(), (int)narrowStr.size(), nullptr, 0);
		std::wstring wideStr(size_needed, 0);
		MultiByteToWideChar(CP_UTF8, 0, narrowStr.c_str(), (int)narrowStr.size(), &wideStr[0], size_needed);
		return wideStr;
	}
}
