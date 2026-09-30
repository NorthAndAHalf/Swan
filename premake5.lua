local ROOT = ""
local SourceDir = ROOT .. "src/"

workspace "Swan"
    configurations { "Debug", "Release" }
    platforms { "x64" }

project "Swan"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++23"
    staticruntime "off"
    
    targetdir ("bin/%{cfg.buildcfg}")
    objdir ("bin-int/%{cfg.buildcfg}")
    targetname "Swan"
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
        "vendor/imgui",
        "vendor/imgui/imgui"
    }

    links 
    { 
        "opengl32.lib",
        "vendor/glfw/lib-vc2022/glfw3.lib"
    }

    filter "configurations:Debug"
        defines { "SW_DEBUG" }
        runtime "Debug"
        symbols "On"

        postbuildcommands {
            "{COPYDIR} \"assets\" \"%{cfg.targetdir}/assets\""
        }

    filter "configurations:Release"
        defines { "SW_NDEBUG" }
        runtime "Release"
        optimize "On"

        postbuildcommands {
            "{COPYDIR} \"assets\" \"%{cfg.targetdir}/assets\""
        }

    filter "{}"