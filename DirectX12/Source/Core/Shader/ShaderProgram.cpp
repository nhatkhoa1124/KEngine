#include "dx12pch.h"
#include "ShaderProgram.h"
#include <d3dcompiler.h>
#include <fstream>
#include "Utils/DX12Helper.h"

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
	void ShaderProgram::LoadShaderBinary(const std::wstring& filename)
	{
		std::fstream fin(filename, std::ios::binary);
		fin.seekg(0, std::ios_base::end);
		std::ifstream::pos_type size = (size_t)(fin.tellg());
		fin.seekg(0, std::ios_base::beg);

		ThrowIfFailed(D3DCreateBlob(size, mByteCode.GetAddressOf()));
		fin.read((char*)mByteCode->GetBufferPointer(), size);
		fin.close();
	}
}