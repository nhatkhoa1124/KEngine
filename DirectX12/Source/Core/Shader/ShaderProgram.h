#pragma once

#include <string>
#include <d3dcommon.h>
#include <wrl/client.h>

namespace KRender
{
	using Microsoft::WRL::ComPtr;
	class ShaderProgram
	{
	public:
		ShaderProgram();
		~ShaderProgram() = default;
		void CompileShader
		(
			const std::wstring& filename,
			const D3D_SHADER_MACRO* defines,
			const std::string& entryPoint,
			const std::string& target
		);
	private:
		UINT mCompileFlags;
		ComPtr<ID3DBlob> mByteCode = nullptr;
	};
}