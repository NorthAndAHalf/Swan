#pragma once

#include "Memory/LinearBuffer.h"

class OpenGLRenderer
{
public:
	OpenGLRenderer();
	void Init();

	void SwapBuffers();
	void Update();
private:
	LinearBuffer m_VertexPool;
	LinearBuffer m_IndexPool;
};