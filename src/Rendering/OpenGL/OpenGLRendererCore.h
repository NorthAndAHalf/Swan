#pragma once

#include <cstdint>

struct ScreenQuad
{
	ScreenQuad() {}

	uint32_t vao = 0;
	uint32_t vbo = 0;
	uint32_t ibo = 0;

	float vertices[16] =
	{
		// X		Y	   U	  V
		  -1.0f, -1.0f,  0.0f,  1.0f, // Bottom Left
		  -1.0f,  1.0f,  0.0f,  0.0f, // Top Left
		   1.0f,  1.0f,  1.0f,  0.0f, // Top Right
		   1.0f, -1.0f,  1.0f,  1.0f  // Bottom Right 
	};

	uint32_t indices[6] =
	{
		0, 3, 2,
		2, 1, 0
	};
};