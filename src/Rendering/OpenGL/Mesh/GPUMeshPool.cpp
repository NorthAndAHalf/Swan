#include "GPUMeshPool.h"
#include "glad/glad.h"
#include "../../../Core.h"

#define MAX_VERTS (4 * 1024 * 1024) // 4,194,304
#define MAX_INDICES (4 * 4 * 1024 * 1024) // 16,777,216

GPUMeshPool::GPUMeshPool()
	: m_vertexHead(0), m_indexHead(0)
{
	glCreateBuffers(1, &m_vbo);
	glCreateBuffers(1, &m_ebo);
	glNamedBufferStorage(m_vbo, MAX_VERTS * sizeof(Vertex), nullptr, GL_DYNAMIC_STORAGE_BIT);
	glNamedBufferStorage(m_ebo, MAX_INDICES * sizeof(uint32_t), nullptr, GL_DYNAMIC_STORAGE_BIT);

	glCreateVertexArrays(1, &m_vao);
	glVertexArrayVertexBuffer(m_vao, 0, m_vbo, 0, sizeof(Vertex));
	glVertexArrayElementBuffer(m_vao, m_ebo);

	glVertexArrayAttribFormat(m_vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, position));
	glVertexArrayAttribBinding(m_vao, 0, 0);
	glEnableVertexArrayAttrib(m_vao, 0);

	glVertexArrayAttribFormat(m_vao, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, normal));
	glVertexArrayAttribBinding(m_vao, 1, 0);
	glEnableVertexArrayAttrib(m_vao, 1);

	glVertexArrayAttribFormat(m_vao, 2, 4, GL_FLOAT, GL_FALSE, offsetof(Vertex, tangent));
	glVertexArrayAttribBinding(m_vao, 2, 0);
	glEnableVertexArrayAttrib(m_vao, 2);

	glVertexArrayAttribFormat(m_vao, 3, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, uv));
	glVertexArrayAttribBinding(m_vao, 3, 0);
	glEnableVertexArrayAttrib(m_vao, 3);
}

GPUMeshPool::~GPUMeshPool()
{
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ebo);
}

GPUMeshLocation GPUMeshPool::StoreMeshData(const MeshData& meshData)
{
	SW_ASSERT(m_vertexHead + meshData.vertices.size() <= MAX_VERTS, "GPU vertex pool exceeded");
	SW_ASSERT(m_indexHead + meshData.indices.size() <= MAX_INDICES, "GPU index pool exceeded");

	GPUMeshLocation handler{(uint32_t)m_vertexHead, (uint32_t)m_indexHead, (uint32_t)meshData.indices.size()};

	glNamedBufferSubData(m_vbo, m_vertexHead * sizeof(Vertex), meshData.vertices.size() * sizeof(Vertex), meshData.vertices.data());
	glNamedBufferSubData(m_ebo, m_indexHead * sizeof(uint32_t), meshData.indices.size() * sizeof(uint32_t), meshData.indices.data());

	m_vertexHead += meshData.vertices.size();
	m_indexHead += meshData.indices.size();

	return handler;
}

void GPUMeshPool::Clear()
{
	m_vertexHead = 0;
	m_indexHead = 0;
}
