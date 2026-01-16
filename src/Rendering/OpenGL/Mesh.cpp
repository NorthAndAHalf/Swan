#include "Mesh.h"

Mesh::Mesh(Vertex* vertexPtr, uint32_t vertexCount, uint32_t* indexPtr, uint32_t indexCount)
    : m_Vertices(vertexPtr), m_VertexCount(vertexCount), m_Indices(indexPtr), m_IndexCount(indexCount)
{
}

void Mesh::set_vertices(Vertex* ptr)
{
    m_Vertices = ptr;
}

Vertex* Mesh::get_vertices()
{
    return m_Vertices;
}

void Mesh::set_vertex_count(uint32_t count)
{
    m_VertexCount = count;
}

uint32_t Mesh::get_vertex_count()
{
    return m_VertexCount;
}

void Mesh::set_indices(uint32_t* ptr)
{
    m_Indices = ptr;
}

uint32_t* Mesh::get_indices()
{
    return m_Indices;
}

void Mesh::set_index_count(uint32_t count)
{
    m_IndexCount = count;
}

uint32_t Mesh::get_index_count()
{
    return m_IndexCount;
}
