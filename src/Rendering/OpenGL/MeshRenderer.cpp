#include "MeshRenderer.h"

MeshRenderer::MeshRenderer(Mesh mesh)
	: m_Mesh(mesh)
{
}

const glm::mat4 MeshRenderer::get_model_matrix()
{
	if (m_CacheDirty) {
		m_CachedMatrix = glm::translate(glm::mat4(1.0f), m_Position)
			* glm::mat4_cast(m_Rotation)
			* glm::scale(glm::mat4(1.0f), m_Scale);
		m_CacheDirty = false;
	}
	return m_CachedMatrix;
}

void MeshRenderer::set_position(const glm::vec3& pos)
{
	m_Position = pos;
	m_CacheDirty = true;
}

void MeshRenderer::rotate(float angle, const glm::vec3& axis)
{
	m_Rotation = glm::rotate(m_Rotation, angle, axis);
	m_CacheDirty = true;
}

void MeshRenderer::set_rotation(float angle, const glm::vec3& axis)
{
	m_Rotation = glm::angleAxis(angle, axis);
	m_CacheDirty = true;
}

void MeshRenderer::set_euler_angles(float pitch, float yaw, float roll)
{
	m_Rotation = glm::quat(glm::vec3(pitch, yaw, roll));
	m_CacheDirty = true;
}

void MeshRenderer::set_scale(const glm::vec3& scale)
{
	m_Scale = scale;
	m_CacheDirty = true;
}
