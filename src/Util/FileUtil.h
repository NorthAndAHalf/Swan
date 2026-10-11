#pragma once

#include <string>
#include "../Rendering/OpenGL/Mesh/Mesh.h"

namespace FileUtil 
{
	std::string ReadFileToString(const char* path);

	Mesh LoadMesh(std::string path);
}