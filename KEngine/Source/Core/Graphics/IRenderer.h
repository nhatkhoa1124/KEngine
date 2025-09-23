#pragma once

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
};