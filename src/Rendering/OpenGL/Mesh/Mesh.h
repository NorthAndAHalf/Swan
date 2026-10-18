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

struct Submesh
{
    GPUMeshLocation m_bufferLocation;

    uint32_t m_albedoTextureIndex;
    uint32_t m_normalTextureIndex;
    uint32_t m_ormTextureIndex;
    uint32_t m_emissiveTextureIndex;

    AABB m_boundingBox;
};

class Mesh
{
public:
	Mesh();
    Mesh(Submesh* _submeshStart, uint32_t _submeshCount);

	const glm::mat4 GetModelMatrix();
    const Submesh* GetSubmeshArray() { return m_submeshArrayStart; }
    uint32_t GetSubmeshCount() { return m_submeshCount; }

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
    Submesh* m_submeshArrayStart = nullptr;
    uint32_t m_submeshCount = 0;

    glm::vec3 m_Position = glm::vec3(0.0f);
    glm::quat m_Rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    glm::vec3 m_Scale = glm::vec3(0.0f);

    AABB m_boundingBox;
};
