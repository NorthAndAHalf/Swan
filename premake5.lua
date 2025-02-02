local ROOT = ""

workspace "Snowdrift"
    configurations { "Debug", "Release" }

project "Snowdrift"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    targetdir "bin/%{cfg.buildcfg}"
    targetname "Snowdrift"
    architecture "x64"
    local SourceDir = ROOT .. "src/";

    buildoptions  { "/utf-8" }

    files { SourceDir .. "**.cpp", SourceDir .. "**.h", SourceDir .. "*.c", "vendor/glad/src/glad.c" }

    includedirs 
    { 
        SourceDir,
        "vendor/glfw/include",
        "vendor/glad/include",
        "vendor/glm",
        "vendor/spdlog/include",
        "vendor/stbimage",
        "vendor/assimp/include"
    }
    libdirs 
    { 
        "vendor/glfw",
        "vendor/glad",
        "vendor/glm",
        "vendor/spdlog",
        "vendor/stbimage",
        "vendor/assimp/include"
    }
    links 
    { 
        "vendor/glfw/lib-vc2022/glfw3.lib",
        "opengl32.lib",
        "vendor/assimp/lib/Debug/assimp-vc143-mtd.lib"
    }

    postbuildcommands {
        -- Copy Assimp DLLs to the output directory after build
        "{COPY} vendor/assimp/%{cfg.targetdir}/*.dll %{cfg.targetdir}"
    }
    

    filter "configurations:Debug"
        defines { "SD_DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "SD_NDEBUG" }
        optimize "On"