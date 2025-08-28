#pragma once

#include <string>

namespace KTime
{
	std::wstring KENGINE_API GetTime(bool stripped = false);
	std::wstring KENGINE_API GetDate(bool stripped = false);
	std::wstring KENGINE_API GetDateTimeString(bool stripped = false);
}