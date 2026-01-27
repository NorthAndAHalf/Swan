#include "Framebuffer.h"
#include "glad/glad.h"

#include "spdlog/spdlog.h"

Framebuffer::Framebuffer(const char* name, uint32_t width, uint32_t height, bool useDepth, bool useStencil)
	: m_name(name), m_width(width), m_height(height), m_useDepthBuffer(useDepth), m_useStencilBuffer(useStencil)
{
	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

	glGenTextures(1, &m_colorBuffer);
	glBindTexture(GL_TEXTURE_2D, m_colorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorBuffer, 0);

	uint32_t internal_format;
	uint32_t attachment;
	bool use_depth_or_stencil = true;

	if (m_useDepthBuffer && m_useStencilBuffer)
	{
		internal_format = GL_DEPTH24_STENCIL8;
		attachment = GL_DEPTH_STENCIL_ATTACHMENT;
	}
	else if (m_useDepthBuffer)
	{
		internal_format = GL_DEPTH_COMPONENT24;
		attachment = GL_DEPTH_ATTACHMENT;
	}
	else if (m_useStencilBuffer)
	{
		internal_format = GL_STENCIL_INDEX8;
		attachment = GL_STENCIL_ATTACHMENT;
	}
	else
	{
		use_depth_or_stencil = false;
	}

	if (use_depth_or_stencil) 
	{
		glGenRenderbuffers(1, &m_rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, internal_format, m_width, m_height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, m_rbo);
	}

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		spdlog::error("Framebuffer is not complete: 0x{1:x}", m_name, status);
	}
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
}

void Framebuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Clear()
{
	GLbitfield mask = GL_COLOR_BUFFER_BIT;

	if (m_useDepthBuffer)   mask |= GL_DEPTH_BUFFER_BIT;
	if (m_useStencilBuffer) mask |= GL_STENCIL_BUFFER_BIT;

	glClear(mask);
}

uint32_t Framebuffer::GetWidth()
{
	return m_width;
}

uint32_t Framebuffer::GetHeight()
{
	return m_height;
}

uint32_t Framebuffer::GetColorTexture()
{
	return m_colorBuffer;
}

uint32_t Framebuffer::GetRBO()
{
	return m_rbo;
}
