#include "LinearBuffer.h"

LinearBuffer::LinearBuffer(size_t _size)
	: size(_size), offset(0)
{
	data = new uint8_t[size];
}

LinearBuffer::~LinearBuffer()
{
	delete[] data;
}

void* LinearBuffer::allocate(size_t bytes)
{
	if (offset + bytes > size) return nullptr; // Out of memory
	void* ptr = data + offset;
	offset += bytes;
	return ptr;
}

void LinearBuffer::reset()
{
	offset = 0;
}
