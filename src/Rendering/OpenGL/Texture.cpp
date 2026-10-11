#include "Texture.h"
#include "glad/glad.h"

Texture::Texture(uint32_t width, uint32_t height, uint32_t channels,
    const uint8_t* data, bool srgb)
    : m_width(width), m_height(height)
{
    GLenum format = GL_RGBA;
    GLenum internalFormat = srgb ? GL_SRGB8_ALPHA8 : GL_RGBA8;
    switch (channels) {
    case 1: format = GL_RED;  internalFormat = GL_R8;  break;
    case 2: format = GL_RG;   internalFormat = GL_RG8; break;
    case 3: format = GL_RGB;  internalFormat = srgb ? GL_SRGB8 : GL_RGB8; break;
    case 4: break;
    }

    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_width, m_height, 0,
        format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_id);
}


