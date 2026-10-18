#include "Renderer.h"
#include "Rendering/OpenGL/Mesh/Mesh.h"
#include "spdlog/spdlog.h"
#include "Passes/TestPasses.h"

Renderer::Renderer(uint32_t viewportWidth, uint32_t viewportHeight)
	: m_viewportWidth(viewportWidth), m_viewportHeight(viewportHeight),
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

	m_GPUMeshPool = new GPUMeshPool();
	m_submeshBuffer = new LinearBuffer(50000 * sizeof(Submesh));

	// One texel default textures
	const uint8_t white[4] = { 255, 255, 255, 255 };
	const uint8_t flat[4] = { 128, 128, 255, 255 };
	const uint8_t orm[4] = { 255, 255,   0, 255 };
	const uint8_t black[4] = { 0,   0,   0, 255 };

	m_defaultAlbedo = AddTextureToPool(1, 1, 4, white, true);    // sRGB
	m_defaultNormal = AddTextureToPool(1, 1, 4, flat, false);   // linear
	m_defaultOrm = AddTextureToPool(1, 1, 4, orm, false);   // linear
	m_defaultEmissive = AddTextureToPool(1, 1, 4, black, true);    // sRGB
}

Renderer::~Renderer()
{
	delete m_submeshBuffer;
	delete m_GPUMeshPool;
}

void Renderer::Init()
{
	CompileShaders();

	glClearColor(1.0, 0.0, 1.0, 1.0);
	glEnable(GL_CULL_FACE);
	m_quad.Init();
}

void Renderer::Update()
{
	m_quad.BindVAO();
	TestPass(m_quad, *m_writeFramebuffer, *m_shaders[0]);
	SwapFramebuffers();
	TestInversePass(m_quad, *m_readFramebuffer, *m_writeFramebuffer, *m_shaders[1]);
	SwapFramebuffers();
	m_quad.DrawToScreen(*m_shaders[2], (*m_readFramebuffer).GetColorTexture());
	glBindVertexArray(0);
}

bool Renderer::CompileShaders()
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

void Renderer::SwapFramebuffers()
{
	Framebuffer* new_read = m_writeFramebuffer;
	Framebuffer* new_write = m_readFramebuffer;

	m_writeFramebuffer = new_write;
	m_readFramebuffer = new_read;
}

uint32_t Renderer::GetViewportWidth()
{
	return m_viewportWidth;
}

void Renderer::SetViewportWidth(uint32_t w)
{
	m_viewportWidth = w;
}

uint32_t Renderer::GetViewportHeight()
{
	return m_viewportHeight;
}

void Renderer::SetViewportHeight(uint32_t h)
{
	m_viewportHeight = h;
}

void* Renderer::StoreSubmesh(const Submesh submesh)
{
	Submesh* ptr = (Submesh*) m_submeshBuffer->allocate(sizeof(Submesh), alignof(Submesh));
	*ptr = submesh;
	return ptr;
}

uint32_t Renderer::AddTextureToPool(uint32_t width, uint32_t height, uint32_t channels, const uint8_t* data, bool srgb)
{
	m_texturePool.push_back(Texture(width, height, channels, data, srgb));
	return m_texturePool.size() - 1;
}
