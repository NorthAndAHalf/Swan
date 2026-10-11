#pragma once

#include <cstdint>

class Texture
{
public:
	Texture(uint32_t width, uint32_t height, uint32_t channels, const uint8_t* data, bool srgb);
	~Texture();

	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

    Texture(Texture&& other) noexcept
        : m_id(other.m_id), m_width(other.m_width), m_height(other.m_height) {}

    Texture& operator=(Texture&& other) noexcept {
        if (this != &other) { 
            m_id = other.m_id; 
            m_width = other.m_width;
            m_height = other.m_height;
        }
        return *this;
    }

private:
	uint32_t m_id;
	uint32_t m_width;
	uint32_t m_height;
};