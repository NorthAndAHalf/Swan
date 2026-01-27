#include "Texture.h"
#include "glad/glad.h"

Texture::Texture(uint32_t width, uint32_t height)
	: m_data(NULL), m_width(width), m_height(height)
{
	glGenTextures(1, &m_id);
	glBindTexture(GL_TEXTURE_2D, m_id);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

const void* Texture::GetData()
{
	return m_data;
}

uint32_t Texture::GetViewportWidth()
{
	return m_width;
}

void Texture::SetViewportWidth(uint32_t w)
{
	m_width = w;
}

uint32_t Texture::GetViewportHeight()
{
	return m_height;
}

void Texture::SetViewportHeight(uint32_t h)
{
	m_height = h;
}