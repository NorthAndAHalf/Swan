#include "Mesh.h"

Mesh::Mesh()
	:  m_CacheDirty(false)
{
}

const glm::mat4 Mesh::GetModelMatrix()
{
	if (m_CacheDirty) {
		m_CachedMatrix = glm::translate(glm::mat4(1.0f), m_Position)
			* glm::mat4_cast(m_Rotation)
			* glm::scale(glm::mat4(1.0f), m_Scale);
		m_CacheDirty = false;
	}
	return m_CachedMatrix;
}

void Mesh::SetPosition(const glm::vec3& pos)
{
	m_Position = pos;
	m_CacheDirty = true;
}

void Mesh::Translate(const glm::vec3& offset)
{
	m_Position += offset;
	m_CacheDirty = true;
}

void Mesh::Rotate(float angle, const glm::vec3& axis)
{
	m_Rotation = glm::rotate(m_Rotation, angle, axis);
	m_CacheDirty = true;
}

void Mesh::SetRotation(float angle, const glm::vec3& axis)
{
	m_Rotation = glm::angleAxis(angle, axis);
	m_CacheDirty = true;
}

void Mesh::SetEulerAngles(float pitch, float yaw, float roll)
{
	m_Rotation = glm::quat(glm::vec3(pitch, yaw, roll));
	m_CacheDirty = true;
}

void Mesh::SetScale(const glm::vec3& scale)
{
	m_Scale = scale;
	m_CacheDirty = true;
}
