#pragma once

#include <cstdint>
#include <string>

class Framebuffer
{
public:
	Framebuffer(const char* name, uint32_t width, uint32_t height, bool useDepth, bool useStencil);

	void Bind();
	void Unbind();
	void Clear();

	uint32_t GetWidth();
	uint32_t GetHeight();

	uint32_t GetColorTexture();
	uint32_t GetRBO();

private:
	std::string m_name;
	uint32_t m_width;
	uint32_t m_height;

	bool m_useDepthBuffer;
	bool m_useStencilBuffer;

	uint32_t m_fbo;
	uint32_t m_colorBuffer;
	uint32_t m_rbo;
};