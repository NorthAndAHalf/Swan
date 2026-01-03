#pragma once

class OpenGLRenderer
{
public:
	OpenGLRenderer();
	void init();

	void swap_buffers();
	void update();
};