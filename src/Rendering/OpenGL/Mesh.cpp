#include "Mesh.h"

Mesh::Mesh(Vertex* vertexPtr, uint32_t vertexCount, uint32_t* indexPtr, uint32_t indexCount)
    : m_Vertices(vertexPtr), m_VertexCount(vertexCount), m_Indices(indexPtr), m_IndexCount(indexCount)
{
}

void Mesh::SetVertices(Vertex* ptr)
{
    m_Vertices = ptr;
}

Vertex* Mesh::GetVertices()
{
    return m_Vertices;
}

void Mesh::SetVertexCount(uint32_t count)
{
    m_VertexCount = count;
}

uint32_t Mesh::GetVertexCount()
{
    return m_VertexCount;
}

void Mesh::SetIndices(uint32_t* ptr)
{
    m_Indices = ptr;
}

uint32_t* Mesh::GetIndices()
{
    return m_Indices;
}

void Mesh::SetIndexCount(uint32_t count)
{
    m_IndexCount = count;
}

uint32_t Mesh::GetIndexCount()
{
    return m_IndexCount;
}
