#include "OpenGLRenderer.h"
#include "Rendering/OpenGL/Mesh.h"
#include "spdlog/spdlog.h"
#include "Passes/TestPasses.h"

// Allocate space for ten million vertices, 50 million indices
OpenGLRenderer::OpenGLRenderer(uint32_t viewportWidth, uint32_t viewportHeight)
	: m_viewportWidth(viewportWidth), m_viewportHeight(viewportHeight),
	m_vertexPool(LinearBuffer(sizeof(Vertex) * 10000000)),
	m_indexPool(LinearBuffer(sizeof(uint32_t) * 50000000)),
	m_framebuffer1(Framebuffer("FB1", viewportWidth, viewportHeight, true, false)),
	m_framebuffer2(Framebuffer("FB1", viewportWidth, viewportHeight, true, false))
{
	m_writeFramebuffer = &m_framebuffer1;
	m_readFramebuffer = &m_framebuffer2;

	m_shaders.emplace_back(std::make_unique<ShaderProgram>(
		"Screen Quad",
		"assets/shaders/glsl/vert_Quad.glsl",
		"assets/shaders/glsl/frag_QuadSolid.glsl"));

	m_shaders.emplace_back(std::make_unique<ShaderProgram>(
		"Screen Quad",
		"assets/shaders/glsl/vert_Quad.glsl",
		"assets/shaders/glsl/frag_QuadInverse.glsl"));

	m_shaders.emplace_back(std::make_unique<ShaderProgram>(
		"Textured Quad",
		"assets/shaders/glsl/vert_Quad.glsl",
		"assets/shaders/glsl/frag_QuadTexture.glsl"));
}

void OpenGLRenderer::Init()
{
	CompileShaders();

	glClearColor(1.0, 0.0, 1.0, 1.0);
	glEnable(GL_CULL_FACE);
	m_quad.Init();
}

void OpenGLRenderer::Update()
{
	m_quad.BindVAO();
	TestPass(m_quad, *m_writeFramebuffer, *m_shaders[0]);
	SwapFramebuffers();
	TestInversePass(m_quad, *m_readFramebuffer, *m_writeFramebuffer, *m_shaders[1]);
	SwapFramebuffers();
	m_quad.DrawToScreen(*m_shaders[2], (*m_readFramebuffer).GetColorTexture());
	glBindVertexArray(0);
}

bool OpenGLRenderer::CompileShaders()
{
	spdlog::info("Compiling Shaders");
	for (auto&& shader : m_shaders)
	{
		bool success = shader->Compile();
		if (!success)
			return false;
	}
	return true;
}

void OpenGLRenderer::SwapFramebuffers()
{
	Framebuffer* new_read = m_writeFramebuffer;
	Framebuffer* new_write = m_readFramebuffer;

	m_writeFramebuffer = new_write;
	m_readFramebuffer = new_read;
}

uint32_t OpenGLRenderer::GetViewportWidth()
{
	return m_viewportWidth;
}

void OpenGLRenderer::SetViewportWidth(uint32_t w)
{
	m_viewportWidth = w;
}

uint32_t OpenGLRenderer::GetViewportHeight()
{
	return m_viewportHeight;
}

void OpenGLRenderer::SetViewportHeight(uint32_t h)
{
	m_viewportHeight = h;
}

