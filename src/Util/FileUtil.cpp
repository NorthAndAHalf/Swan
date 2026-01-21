#include "FileUtil.h"

#include <iostream>
#include <fstream>

#include "spdlog/spdlog.h"

namespace FileUtil 
{

    std::string ReadFileToString(const char* path) 
    {
        std::ifstream file(path, std::ios::binary | std::ios::ate);

        if (!file)
        {
            spdlog::warn("File not found: {0}", path);
            return "";
        }

        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        std::string buffer;
        buffer.reserve(size);

        buffer.assign((std::istreambuf_iterator<char>(file)),
            std::istreambuf_iterator<char>());

        return buffer;
    }

}