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
        "GLFW_INCLUDE_NONE",            -- Fixes GLFW conflicts
        "IMGUI_IMPL_OPENGL_LOADER_GLAD", -- Fixes ImGui conflicts
        "WIN32_LEAN_AND_MEAN"           -- Fixes Windows.h conflicts
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

    -- Common Libraries
    links 
    { 
        "opengl32.lib",
        "vendor/glfw/lib-vc2022/glfw3.lib"
    }

    -- Configuration specific logic
    filter "configurations:Debug"
        defines { "SF_DEBUG" }
        runtime "Debug"
        symbols "On"

        -- Link Debug version of Assimp
        links { "vendor/assimp/lib/Debug/assimp-vc145-mtd.lib" }

        -- Copy Debug DLLs and Assets
        postbuildcommands {
            "{COPY} \"vendor/assimp/lib/Debug/*.dll\" \"%{cfg.targetdir}\"",
            "{COPYDIR} \"assets\" \"%{cfg.targetdir}/assets\""
        }

    filter "configurations:Release"
        defines { "SF_NDEBUG" }
        runtime "Release"
        optimize "On"

        -- Link Release version of Assimp
        links { "vendor/assimp/lib/Release/assimp-vc145.lib" }

        -- Copy Release DLLs and Assets
        postbuildcommands {
            "{COPY} \"vendor/assimp/lib/Release/*.dll\" \"%{cfg.targetdir}\"",
            "{COPYDIR} \"assets\" \"%{cfg.targetdir}/assets\""
        }

    filter "{}" -- Reset filter