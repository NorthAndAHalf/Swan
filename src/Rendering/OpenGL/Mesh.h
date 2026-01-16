#pragma once

#include "glm/vec3.hpp"
#include "glm/vec2.hpp"

struct Vertex
{
	const glm::vec3 pos;
	const glm::vec2 uv;
};

class Mesh
{
public:
	Mesh(Vertex* vertexPtr, uint32_t vertexCount, uint32_t* indexPtr, uint32_t indexCount);

	void set_vertices(Vertex* ptr);
	Vertex* get_vertices();

	void set_vertex_count(uint32_t count);
	uint32_t get_vertex_count();

	void set_indices(uint32_t* ptr); // All indices are 32 bit for now, but could change in the future to be adaptive based on Mesh size
	uint32_t* get_indices();

	void set_index_count(uint32_t count);
	uint32_t get_index_count();

private:
	Vertex* m_Vertices;
	uint32_t* m_Indices;

	uint32_t m_VertexCount;
	uint32_t m_IndexCount;
};