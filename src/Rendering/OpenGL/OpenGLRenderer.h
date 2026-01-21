#pragma once

#include "OpenGLRendererCore.h"
#include "ShaderProgram.h"
#include "Memory/LinearBuffer.h"

class OpenGLRenderer
{
public:
	OpenGLRenderer();
	void Init();
	void Update();
private:
	LinearBuffer m_vertexPool;
	LinearBuffer m_indexPool;

	ScreenQuad m_quad;

	ShaderProgram m_shaderProgram;
};