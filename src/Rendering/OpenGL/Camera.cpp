#include "Camera.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

Camera::Camera(float fov, float aspect, float near, float far)
	: m_Fov(fov), m_Aspect(aspect), m_NearPlane(near), m_FarPlane(far), m_CacheDirty(false)
{
	CalculateViewMatrix();
	CalculateProjectionMatrix();
}

const glm::mat4& Camera::GetVPMatrix()
{
	if (m_CacheDirty)
	{
		CalculateProjectionMatrix();
	}
	CalculateProjectionMatrix();

	return m_ProjectionMatrix * m_ViewMatrix;
}

glm::vec3 Camera::GetUp()
{
	return glm::normalize(m_Rotation * glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::vec3 Camera::GetForward()
{
	return glm::normalize(m_Rotation * glm::vec3(0.0f, 0.0f, -1.0f));
}

glm::vec3 Camera::GetRight()
{
	return glm::normalize(m_Rotation * glm::vec3(1.0f, 0.0f, 0.0f));
}

void Camera::CalculateViewMatrix()
{
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position);
	transform *= glm::mat4_cast(m_Rotation);
	m_ViewMatrix = glm::inverse(transform);
}

void Camera::CalculateProjectionMatrix()
{
	m_ProjectionMatrix = glm::perspective(m_Fov, m_Aspect, m_NearPlane, m_FarPlane);
	m_CacheDirty = false;
}

void Camera::SetPosition(const glm::vec3& pos)
{
	m_Position = pos;
	m_CacheDirty = true;
}

void Camera::Translate(const glm::vec3& offset)
{
	m_Position += offset;
}

void Camera::Rotate(float angle, const glm::vec3& axis)
{
	m_Rotation = glm::rotate(m_Rotation, angle, axis);
}

void Camera::SetRotation(float angle, const glm::vec3& axis)
{
	m_Rotation = glm::angleAxis(angle, axis);
}

void Camera::SetEulerAngles(float pitch, float yaw, float roll)
{
	m_Rotation = glm::quat(glm::vec3(pitch, yaw, roll));
}

