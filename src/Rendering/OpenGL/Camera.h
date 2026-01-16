#pragma once

#include "glm/vec3.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"

class Camera
{
public:
	Camera(float fov, float aspect, float near, float far);

	const glm::mat4& GetVPMatrix();

	void SetPosition(const glm::vec3& pos);
	const glm::vec3& GetPosition() { return m_Position; }

	void Translate(const glm::vec3& offset);
	void Rotate(float angle, const glm::vec3& axis);
	void SetRotation(float angle, const glm::vec3& axis);
	void SetEulerAngles(float pitch, float yaw, float roll);
	const glm::quat& GetRotation() { return m_Rotation; }

private:
	void CalculateViewMatrix();
	void CalculateProjectionMatrix();

	float m_Fov;
	float m_Aspect;
	float m_NearPlane;
	float m_FarPlane;

	glm::vec3 m_Position;
	glm::quat m_Rotation;

	glm::mat4 m_ViewMatrix;
	glm::mat4 m_ProjectionMatrix;
	
	bool m_CacheDirty;

	glm::mat4 m_CachedVP;
};