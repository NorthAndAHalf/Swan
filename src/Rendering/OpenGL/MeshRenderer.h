#pragma once

#include "Mesh.h"
#include "glm/vec3.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"

class MeshRenderer
{
public:
	MeshRenderer(Mesh mesh);

	const glm::mat4 GetModelMatrix();

    void SetPosition(const glm::vec3& pos);
    const glm::vec3& GetPosition() { return m_Position; }

    void Translate(const glm::vec3& offset);
    void Rotate(float angle, const glm::vec3& axis);
    void SetRotation(float angle, const glm::vec3& axis);
    void SetEulerAngles(float pitch, float yaw, float roll);
    const glm::quat& GetRotation() { return m_Rotation; }

    void SetScale(const glm::vec3& scale);
    const glm::vec3& GetScale() { return m_Scale; }

private:
	Mesh m_Mesh;
    mutable glm::mat4 m_CachedMatrix;
    mutable bool m_CacheDirty;

    glm::vec3 m_Position;
    glm::quat m_Rotation;
    glm::vec3 m_Scale;
};
