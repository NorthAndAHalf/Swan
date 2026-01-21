#include "ShaderProgram.h"
#include "Util/FileUtil.h"
#include "spdlog/spdlog.h"

ShaderProgram::ShaderProgram(std::string name, const char* vertPath, const char* fragPath)
	: m_name(name)
{
	m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
	m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	std::string vertString = FileUtil::ReadFileToString(vertPath);
	std::string fragString = FileUtil::ReadFileToString(fragPath);

	const char* vertSrc = vertString.c_str();
	const char* fragSrc = fragString.c_str();

	glShaderSource(m_vertexShader, 1, &vertSrc, NULL);
	glShaderSource(m_fragmentShader, 1, &fragSrc, NULL);

	Compile();
	Link();
}

bool ShaderProgram::Compile()
{
	glCompileShader(m_vertexShader);
	glCompileShader(m_fragmentShader);

	bool success = true;

	int vertSuccess;
	char vertInfoLog[512];
	glGetShaderiv(m_vertexShader, GL_COMPILE_STATUS, &vertSuccess);

	int fragSuccess;
	char fragInfoLog[512];
	glGetShaderiv(m_fragmentShader, GL_COMPILE_STATUS, &fragSuccess);

	if (!vertSuccess)
	{
		glGetShaderInfoLog(m_vertexShader, 512, NULL, vertInfoLog);
		spdlog::error("Vertex shader compilation error ({0}): {1}", m_name, vertInfoLog);
		success = false;
	}
	if (!fragSuccess)
	{
		glGetShaderInfoLog(m_fragmentShader, 512, NULL, fragInfoLog);
		spdlog::error("Fragment shader compilation error ({0}): {1}", m_name, fragInfoLog);
		success = false;
	}
	return success;
}

bool ShaderProgram::Link()
{
	m_shaderProgram = glCreateProgram();

	glAttachShader(m_shaderProgram, m_vertexShader);
	glAttachShader(m_shaderProgram, m_fragmentShader);

	glLinkProgram(m_shaderProgram);

	glDetachShader(m_shaderProgram, m_vertexShader);
	glDetachShader(m_shaderProgram, m_fragmentShader);

	int success;
	char infoLog[512];
	glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(m_shaderProgram, 512, NULL, infoLog);
		spdlog::error("Shader link failed ({0}): {1}", m_name, infoLog);
		return false;
	}

	return true;
}

void ShaderProgram::Use()
{
	glUseProgram(m_shaderProgram);
}