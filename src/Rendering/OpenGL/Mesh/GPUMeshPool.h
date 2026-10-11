#pragma once

#include <vector>
#include "glad/glad.h"
#include "MeshData.h"

struct GPUMeshLocation
{
	uint32_t vertexLocation;
	uint32_t indexLocation;
	uint32_t indexCount;
};

class GPUMeshPool
{
public:
	GPUMeshPool();
	~GPUMeshPool();

	GPUMeshPool(const GPUMeshPool&) = delete;
	GPUMeshPool& operator=(const GPUMeshPool&) = delete;

	GPUMeshPool(GPUMeshPool&& other) noexcept
		: m_vao(other.m_vao), m_vbo(other.m_vbo), m_ebo(other.m_ebo), m_vertexHead(other.m_vertexHead), m_indexHead(other.m_indexHead) {
	}

	GPUMeshPool& operator=(GPUMeshPool&& other) noexcept {
		if (this != &other) {
			m_vao = other.m_vao;
			m_vbo = other.m_vbo;
			m_ebo = other.m_ebo;
			m_vertexHead = other.m_vertexHead;
			m_indexHead = other.m_indexHead;
		}
		return *this;
	}

	GPUMeshLocation StoreMeshData(const MeshData& meshData);
	void Clear();

private:
	uint32_t m_vao, m_vbo, m_ebo;
	uint32_t m_vertexHead, m_indexHead;
};