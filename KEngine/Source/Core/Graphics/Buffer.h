#pragma once

namespace KEngine
{
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
}