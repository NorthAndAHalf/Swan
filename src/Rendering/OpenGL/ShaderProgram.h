#pragma once
#include <string>
#include <cstdint>
#include "glad/glad.h"

class ShaderProgram
{
public:
	ShaderProgram(std::string name, const char* vertPath, const char* fragPath);

	bool Compile();
	bool Link();
	void Use();

private:
	std::string m_name;
	uint32_t m_vertexShader;
	uint32_t m_fragmentShader;
	uint32_t m_shaderProgram;
};