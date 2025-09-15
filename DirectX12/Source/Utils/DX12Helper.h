#pragma once

#include <string>
#include <comdef.h>

namespace KRender
{
	class KRENDER_API DxException
	{
	public:
		DxException() = default;
		DxException(HRESULT hr, const std::wstring& functionName,
			const std::wstring& filename, int lineNumber);

		std::wstring ToString() const;

		HRESULT ErrorCode = S_OK;
		std::wstring FunctionName;
		std::wstring Filename;
		int LineNumber = -1;
	};

	// Conversion function
	inline std::wstring AnsiToWString(const std::string& str)
	{
		if (str.empty()) return L"";

		int size_needed = MultiByteToWideChar(CP_ACP, 0, str.c_str(), (int)str.size(), nullptr, 0);
		std::wstring wstr(size_needed, 0);
		MultiByteToWideChar(CP_ACP, 0, str.c_str(), (int)str.size(), &wstr[0], size_needed);
		return wstr;
	};

	// Macro for throwing exceptions
#ifndef ThrowIfFailed
#define ThrowIfFailed(x)                                              \
{                                                                     \
    HRESULT hr__ = (x);                                               \
    std::wstring wfn = AnsiToWString(__FILE__);                       \
    if(FAILED(hr__)) { throw DxException(hr__, L#x, wfn, __LINE__); } \
}
#endif

}