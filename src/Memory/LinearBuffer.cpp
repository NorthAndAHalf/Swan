#include "LinearBuffer.h"
#include <memory>

LinearBuffer::LinearBuffer(size_t size)
	: m_Size(size), m_Offset(0)
{
	m_Data = static_cast<uint8_t*>(_aligned_malloc(size, 16));
}

LinearBuffer::~LinearBuffer()
{
	_aligned_free(m_Data);
}

void* LinearBuffer::allocate(size_t bytes, size_t alignment)
{
	void* ptr = m_Data + m_Offset;
	size_t space = m_Size - m_Offset;

	if (std::align(alignment, bytes, ptr, space))
	{
		m_Offset = m_Size - space + bytes;
		return ptr;
	}

	return nullptr; // Out of memory
}

void LinearBuffer::reset()
{
#ifdef SW_DEBUG
	// Clear the buffer to avoid seeing old data while debugging
	std::memset(m_Data, 0x33, m_Size);
#endif
	m_Offset = 0;
}
