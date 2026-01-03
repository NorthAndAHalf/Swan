#include "OpenGLRenderer.h"
#include "glad/glad.h"

OpenGLRenderer::OpenGLRenderer()
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
