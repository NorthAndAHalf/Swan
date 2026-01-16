#pragma once

#include "Memory/LinearBuffer.h"

class OpenGLRenderer
{
public:
	OpenGLRenderer();
	void init();

	void swap_buffers();
	void update();
private:
	LinearBuffer m_VertexPool;
	LinearBuffer m_IndexPool;
};