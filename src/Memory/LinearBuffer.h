#pragma once

#include <cstdint>

class LinearBuffer
{
public:
	LinearBuffer(size_t _size);
	~LinearBuffer();

	void* allocate(size_t bytes);
	void reset();

private:
	uint8_t* data;
	size_t size;
	size_t offset;
};
