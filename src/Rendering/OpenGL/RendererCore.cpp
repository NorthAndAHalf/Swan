#include "RendererCore.h"
#include "glad/glad.h"

void ScreenQuad::Init()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0); // Position
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))); // UV
	glEnableVertexAttribArray(1);
}

void ScreenQuad::BindVAO()
{
	glBindVertexArray(vao);
}

void ScreenQuad::DrawToScreen(ShaderProgram& shader, uint32_t textureId)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glBindTexture(GL_TEXTURE_2D, textureId);
	shader.Use();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void ScreenQuad::DrawToFramebuffer(ShaderProgram& shader, Framebuffer& fb)
{
	fb.Bind();
	fb.Clear();
	shader.Use();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	fb.Unbind();
}
