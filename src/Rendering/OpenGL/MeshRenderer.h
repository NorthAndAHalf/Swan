#pragma once

#include "Mesh.h"
#include "glm/vec3.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"

class MeshRenderer
{
public:
	MeshRenderer(Mesh mesh);

	const glm::mat4 get_model_matrix();

    void set_position(const glm::vec3& pos);
    const glm::vec3& get_position() { return m_Position; }

    void rotate(float angle, const glm::vec3& axis);
    void set_rotation(float angle, const glm::vec3& axis);
    void set_euler_angles(float pitch, float yaw, float roll);
    const glm::quat& get_rotation() { return m_Rotation; }

    void set_scale(const glm::vec3& scale);
    const glm::vec3& get_scale() { return m_Scale; }

private:
	Mesh m_Mesh;
    mutable glm::mat4 m_CachedMatrix;
    mutable bool m_CacheDirty;

    glm::vec3 m_Position;
    glm::quat m_Rotation;
    glm::vec3 m_Scale;
};
