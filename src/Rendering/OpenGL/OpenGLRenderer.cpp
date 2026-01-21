#include "OpenGLRenderer.h"
#include "Rendering/OpenGL/Mesh.h"

// Allocate space for ten million vertices, 50 million indices
OpenGLRenderer::OpenGLRenderer()
	: m_vertexPool(LinearBuffer(sizeof(Vertex) * 10000000)), 
	  m_indexPool(LinearBuffer(sizeof(uint32_t) * 50000000)),

	  m_shaderProgram(ShaderProgram(
		"Screen Quad",
		"assets/shaders/glsl/vert_Quad.glsl",
		"assets/shaders/glsl/frag_QuadSolid.glsl"))
{
}

void OpenGLRenderer::Init()
{
	glClearColor(1.0, 0.0, 1.0, 1.0);
	glEnable(GL_CULL_FACE);

	glGenVertexArrays(1, &m_quad.vao);
	glBindVertexArray(m_quad.vao);

	glGenBuffers(1, &m_quad.vbo);
	glGenBuffers(1, &m_quad.ibo);

	glBindBuffer(GL_ARRAY_BUFFER, m_quad.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_quad.vertices), m_quad.vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_quad.ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_quad.indices), m_quad.indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0); // Position
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))); // UV
	glEnableVertexAttribArray(1);
}

void OpenGLRenderer::Update()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_shaderProgram.Use();
	glBindVertexArray(m_quad.vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

