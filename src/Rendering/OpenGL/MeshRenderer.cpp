#include "MeshRenderer.h"

MeshRenderer::MeshRenderer(Mesh mesh)
	: m_Mesh(mesh), m_CacheDirty(false)
{
}

const glm::mat4 MeshRenderer::GetModelMatrix()
{
	if (m_CacheDirty) {
		m_CachedMatrix = glm::translate(glm::mat4(1.0f), m_Position)
			* glm::mat4_cast(m_Rotation)
			* glm::scale(glm::mat4(1.0f), m_Scale);
		m_CacheDirty = false;
	}
	return m_CachedMatrix;
}

void MeshRenderer::SetPosition(const glm::vec3& pos)
{
	m_Position = pos;
	m_CacheDirty = true;
}

void MeshRenderer::Translate(const glm::vec3& offset)
{
	m_Position += offset;
	m_CacheDirty = true;
}

void MeshRenderer::Rotate(float angle, const glm::vec3& axis)
{
	m_Rotation = glm::rotate(m_Rotation, angle, axis);
	m_CacheDirty = true;
}

void MeshRenderer::SetRotation(float angle, const glm::vec3& axis)
{
	m_Rotation = glm::angleAxis(angle, axis);
	m_CacheDirty = true;
}

void MeshRenderer::SetEulerAngles(float pitch, float yaw, float roll)
{
	m_Rotation = glm::quat(glm::vec3(pitch, yaw, roll));
	m_CacheDirty = true;
}

void MeshRenderer::SetScale(const glm::vec3& scale)
{
	m_Scale = scale;
	m_CacheDirty = true;
}
