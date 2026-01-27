#pragma once

#include <cstdint>

class Texture
{
public:
	Texture(uint32_t width, uint32_t height);

	const void* GetData();

	// In the future add a load function here, to populate the data buffer from within the texture class

	uint32_t GetViewportWidth();
	void SetViewportWidth(uint32_t w);
	uint32_t GetViewportHeight();
	void SetViewportHeight(uint32_t h);
private:
	uint32_t m_id;
	void* m_data;

	uint32_t m_width;
	uint32_t m_height;
};