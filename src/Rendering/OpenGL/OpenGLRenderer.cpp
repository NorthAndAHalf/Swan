#include "OpenGLRenderer.h"
#include "glad/glad.h"
#include "Rendering/OpenGL/Mesh.h"

// Allocate space for ten million vertices, 50 million indices
OpenGLRenderer::OpenGLRenderer()
	: m_VertexPool(sizeof(Vertex) * 10000000), m_IndexPool(sizeof(uint32_t) * 50000000) 
{

}

void OpenGLRenderer::init()
{
	glClearColor(0.0, 1.0, 0.0, 1.0);


}

void OpenGLRenderer::update()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderer::swap_buffers()
{
	
}
