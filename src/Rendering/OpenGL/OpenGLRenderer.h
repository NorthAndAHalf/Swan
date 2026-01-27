#pragma once

#include "OpenGLRendererCore.h"
#include "ShaderProgram.h"
#include "Memory/LinearBuffer.h"
#include <vector>
#include <memory>
#include "Texture.h"
#include "Framebuffer.h"

class OpenGLRenderer
{
public:
	OpenGLRenderer(uint32_t viewportWidth, uint32_t viewportHeight);
	void Init();
	void Update();
	bool CompileShaders();

	uint32_t GetViewportWidth();
	void SetViewportWidth(uint32_t w);
	uint32_t GetViewportHeight();
	void SetViewportHeight(uint32_t h);

private:
	LinearBuffer m_vertexPool;
	LinearBuffer m_indexPool;

	ScreenQuad m_quad;
	Framebuffer m_framebuffer1;
	Framebuffer m_framebuffer2;
	Framebuffer* m_readFramebuffer;
	Framebuffer* m_writeFramebuffer;

	void SwapFramebuffers();

	uint32_t m_viewportWidth;
	uint32_t m_viewportHeight;

	std::vector<std::unique_ptr<ShaderProgram>> m_shaders; // Might need to change to shared pointers to use in render passes? Not sure yet
};