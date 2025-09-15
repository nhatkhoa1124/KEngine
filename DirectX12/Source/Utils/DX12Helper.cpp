#include "dx12pch.h"
#include "DX12Helper.h"
#include <sstream>

namespace KRender
{
	DxException::DxException(HRESULT hr, const std::wstring& functionName,
		const std::wstring& filename, int lineNumber) :
		ErrorCode(hr),
		FunctionName(functionName),
		Filename(filename),
		LineNumber(lineNumber)
	{
	}

	std::wstring DxException::ToString() const
	{
		// Get the string description of the HRESULT
		_com_error err(ErrorCode);
		std::wstring msg = err.ErrorMessage();

		std::wstringstream ss;
		ss << L"Error: " << msg << std::endl;
		ss << L"Code: 0x" << std::hex << ErrorCode << std::endl;
		ss << L"Function: " << FunctionName << std::endl;
		ss << L"File: " << Filename << std::endl;
		ss << L"Line: " << LineNumber;

		return ss.str();
	}

}