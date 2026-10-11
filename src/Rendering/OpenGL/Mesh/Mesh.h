#pragma once

#include "Mesh.h"
#include "glm/vec3.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "GPUMeshPool.h"

struct AABB
{
    // TODO
};

// TODO:
// The renderer will store the MeshPool object
// There will be a standalone mesh import function which adds to the meshpool, and returns the mesh object
// The mesh can be stored somewhere else (i.e. game layer or in the future a scene manager)
// Wherever the mesh is stored will submit render commands to the renderer with a pointer to the mesh

class Mesh
{
public:
	Mesh();

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
    mutable glm::mat4 m_CachedMatrix;
    mutable bool m_CacheDirty;

    GPUMeshLocation m_bufferLocation;

    glm::vec3 m_Position;
    glm::quat m_Rotation;
    glm::vec3 m_Scale;

    uint32_t m_AlbedoTextureIndex;
    uint32_t m_NormalTextureIndex;
    uint32_t m_ORMTextureIndex;
    uint32_t m_EmissiveTextureIndex;

    AABB m_BoundingBox;
};
