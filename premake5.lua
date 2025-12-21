local ROOT = ""

workspace "Snowfall"
    configurations { "Debug", "Release" }

project "Snowfall"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++23"
    targetdir "bin/%{cfg.buildcfg}"
    targetname "Snowfall"
    architecture "x64"
    local SourceDir = ROOT .. "src/";

    buildoptions  { "/utf-8" }

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
        "vendor/imgui/imgui/backends/imgui_impl_glfw.cpp", 
        "vendor/imgui/imgui/backends/imgui_impl_glfw.h",
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
        "vendor/assimp/lib/Debug/assimp-vc145-mtd.lib"
    }

    postbuildcommands {
        -- Copy Assimp DLLs to the output directory after build
        "{COPY} vendor/assimp/%{cfg.targetdir}/*.dll %{cfg.targetdir}"
    }
    

    filter "configurations:Debug"
        defines { "SF_DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "SF_NDEBUG" }
        optimize "On"