local ROOT = ""
local SourceDir = ROOT .. "src/"

workspace "Snowfall"
    configurations { "Debug", "Release" }
    platforms { "x64" }

project "Snowfall"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++23"
    staticruntime "off"
    
    targetdir ("bin/%{cfg.buildcfg}")
    objdir ("bin-int/%{cfg.buildcfg}")
    targetname "Snowfall"
    architecture "x64"

    buildoptions { "/utf-8" }

    defines 
    { 
        "GLFW_INCLUDE_NONE",
        "IMGUI_IMPL_OPENGL_LOADER_GLAD",
        "WIN32_LEAN_AND_MEAN"
    }

    files 
    { 
        SourceDir .. "**.cpp", 
        SourceDir .. "**.h", 
        SourceDir .. "*.c", 
        "vendor/glad/src/glad.c", 
        "vendor/imgui/imgui/*.cpp", 
        "vendor/imgui/imgui/*.h", 
        "vendor/imgui/imgui/backends/imgui_impl_opengl3.cpp",
        "vendor/imgui/imgui/backends/imgui_impl_opengl3.h", 
        "vendor/imgui/imgui/misc/debuggers/imgui.natvis",
        "vendor/imgui/imgui/misc/debuggers/imgui.natstepfilter",
        "vendor/imgui/imgui/misc/cpp/imgui_stdlib.*"
    }

    includedirs 
    { 
        SourceDir,
        "vendor/glfw/include",
        "vendor/glad/include",
        "vendor/glm",
        "vendor/spdlog/include",
        "vendor/stbimage",
        "vendor/assimp/include",
        "vendor/imgui",
        "vendor/imgui/imgui"
    }

    links 
    { 
        "opengl32.lib",
        "vendor/glfw/lib-vc2022/glfw3.lib"
    }

    filter "configurations:Debug"
        defines { "SF_DEBUG" }
        runtime "Debug"
        symbols "On"

        links { "libs/assimp/Debug/assimp-vc145-mtd.lib" }

        postbuildcommands {
            "{COPY} \"libs/assimp/Debug/*.dll\" \"%{cfg.targetdir}\"",
            "{COPYDIR} \"assets\" \"%{cfg.targetdir}/assets\""
        }

    filter "configurations:Release"
        defines { "SF_NDEBUG" }
        runtime "Release"
        optimize "On"

        links { "libs/assimp/Release/assimp-vc145-mt.lib" }

        postbuildcommands {
            "{COPY} \"libs/assimp/Release/*.dll\" \"%{cfg.targetdir}\"",
            "{COPYDIR} \"assets\" \"%{cfg.targetdir}/assets\""
        }

    filter "{}"