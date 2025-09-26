#pragma once

#include <memory>

class IBuffer {
private:
	enum class BufferType
	{
		INDEX_BUFFER,
		VERTEX_BUFFER
	};
public:
	virtual ~IBuffer() = default;
	virtual void CreateBufferView() = 0;
};

class IRenderer {
public:
	virtual ~IRenderer() = default;
	virtual void Initialize() = 0;
	virtual void Draw() = 0;
	virtual void Exit() = 0;
};

class IRendererFactory
{
public:
	virtual ~IRendererFactory() = default;
	virtual std::unique_ptr<IRenderer> CreateRenderer(HWND handle) = 0;
};