#include "dx12pch.h"
#include "ShaderProgram.h"
#include <d3dcompiler.h>

namespace KRender
{
	ShaderProgram::ShaderProgram() :
		mCompileFlags{ 0 }
	{
#if defined(DEBUG) || defined(_DEBUG)
		mCompileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
	}
	void ShaderProgram::CompileShader
	(
		const std::wstring& filename,
		const D3D_SHADER_MACRO* defines,
		const std::string& entryPoint,
		const std::string& target
	)
	{
		HRESULT hr = S_OK;

		ComPtr<ID3DBlob> errors = nullptr;
		hr = D3DCompileFromFile
		(
			filename.c_str(),
			defines,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			entryPoint.c_str(),
			target.c_str(),
			mCompileFlags,
			0,
			&mByteCode,
			&errors
		);

		if (FAILED(hr)) {
			if (errors != nullptr) {
				OutputDebugStringA((char*)errors->GetBufferPointer());
			}
			throw std::exception("Shader compilation failed");
		}
	}
}