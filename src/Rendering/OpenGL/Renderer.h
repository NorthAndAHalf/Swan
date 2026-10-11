#pragma once

#include "RendererCore.h"
#include "ShaderProgram.h"
#include "Memory/LinearBuffer.h"
#include <vector>
#include <memory>
#include "Texture.h"
#include "Framebuffer.h"
#include "Mesh/GPUMeshPool.h"

class Renderer
{
public:
	Renderer(uint32_t viewportWidth, uint32_t viewportHeight);
	void Init();
	void Update();
	bool CompileShaders();

	uint32_t GetViewportWidth();
	void SetViewportWidth(uint32_t w);
	uint32_t GetViewportHeight();
	void SetViewportHeight(uint32_t h);

	GPUMeshPool* GetMeshPool() { return m_GPUMeshPool; }
	uint32_t AddTextureToPool(uint32_t width, uint32_t height, uint32_t channels, const uint8_t* data, bool srgb);

private:
	ScreenQuad m_quad;
	Framebuffer m_framebuffer1;
	Framebuffer m_framebuffer2;
	Framebuffer* m_readFramebuffer;
	Framebuffer* m_writeFramebuffer;

	void SwapFramebuffers();

	std::vector<std::unique_ptr<ShaderProgram>> m_shaders; // Might need to change to shared pointers to use in render passes? Not sure yet
	std::vector<Texture> m_texturePool;
	GPUMeshPool* m_GPUMeshPool;

	uint32_t m_viewportWidth;
	uint32_t m_viewportHeight;
};