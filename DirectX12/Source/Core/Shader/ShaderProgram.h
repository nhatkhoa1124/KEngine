#pragma once

#include "DX12Core.h"
#include <d3dcommon.h>
#include <wrl/client.h>

namespace KRender
{
	using Microsoft::WRL::ComPtr;
	class KRENDER_API ShaderProgram
	{
	protected:
		enum class ShaderType
		{
			VERTEX_SHADER,
			PIXEL_SHADER,
			GEOMETRY_SHADER,
			HULL_SHADER,
			DOMAIN_SHADER,
			COMPUTE_SHADER
		};
		UINT mCompileFlags;
		ComPtr<ID3DBlob> mByteCode = nullptr;
		ShaderType mShaderType;

	public:
		ShaderProgram();
		virtual ~ShaderProgram() = default;
		void CompileShader
		(
			const std::wstring& filename,
			const D3D_SHADER_MACRO* defines,
			const std::string& entryPoint,
			const std::string& target
		);
		void LoadShaderBinary(const std::wstring& filename);

		inline ShaderType GetShaderType() const { return mShaderType; }
		inline ID3DBlob* GetByteCode() const { return mByteCode.Get(); }
		inline bool IsCompiled() const { return mByteCode != nullptr; }

	};
}