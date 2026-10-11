#pragma once

#include "../../../../vendor/glm/glm/vec3.hpp"
#include "../../../../vendor/glm/glm/vec2.hpp"

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec4 tangent;
	glm::vec2 uv;
};

struct MeshData
{
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
};