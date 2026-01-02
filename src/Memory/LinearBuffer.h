#pragma once

#include <cstdint>

class LinearBuffer
{
public:
	LinearBuffer(size_t size);
	~LinearBuffer();

	void* allocate(size_t bytes, size_t alignment);
	void reset();

private:
	uint8_t* m_Data;
	size_t m_Size;
	size_t m_Offset;
};
