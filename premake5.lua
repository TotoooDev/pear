workspace "pear"
    architecture "x64"

    configurations {
        "Debug",
        "Release"
    }

outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "pear-3d"
    location "pear-3d"
    kind "StaticLib"
    language "C"

    targetdir ("bin/" .. outputDir .. "/%{prj.name}")
    objdir ("bin-intermediate/" .. outputDir .. "/%{prj.name}")

    debugdir ("bin/" .. outputDir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.c",
    }

    includedirs
    {
        "pear-3d/include",
        "pear-3d/include/vendor"
    }
    
    filter "system:linux"
        defines {
            "PEAR_PLATFORM_LINUX"
        }

    filter "configurations:Debug"
        defines
        {
            "PEAR_DEBUG",
        }
        symbols "On"

    filter "configurations:Release"
        optimize "On"

project "pear"
    location "pear"
    kind "StaticLib"
    language "C"

	targetdir ("bin/" .. outputDir .. "/%{prj.name}")
	objdir ("bin-intermediate/" .. outputDir .. "/%{prj.name}")

	debugdir ("bin/" .. outputDir .. "/%{prj.name}")

    files
	{
		"pear/include/**.h",
		"pear/src/**.c",
		"pear/src/vendor/**.c",
	}

    includedirs
	{
		"pear/include",
		"pear/include/vendor",
		"pear-3d/include"
	}

    filter "system:linux"
        defines {
            "PEAR_PLATFORM_LINUX",
            "PEAR_PLATFORM_OPENGL"
        }

    filter "configurations:Debug"
		defines
		{
			"PEAR_DEBUG",
		}
		symbols "On"

	filter "configurations:Release"
		optimize "On"

project "pear-project"
    location "pear-project"
    kind "ConsoleApp"
    language "C"

    targetdir ("bin/" .. outputDir .. "/%{prj.name}")
    objdir ("bin-intermediate/" .. outputDir .. "/%{prj.name}")

    debugdir ("bin/" .. outputDir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.c",
    }

    includedirs
    {
        "pear-project/include",
        "pear-project/include/vendor",
        "pear/include/vendor",
        "pear/include",
        "pear-3d/include"
    }
    
    links {
        "pear",
        "pear-3d"
    }

    filter "system:linux"
        defines {
            "PEAR_PLATFORM_LINUX",
            "PEAR_PLATFORM_OPENGL"
        }
        links {
            "m",
            "glfw",
            "GL",
            "GLEW"
        }
        postbuildcommands ("cp -R assets/* ../bin/" .. outputDir .. "/%{prj.name}/")

    filter "configurations:Debug"
        defines
        {
            "PEAR_DEBUG",
        }
        symbols "On"

    filter "configurations:Release"
        optimize "On"

project "pear-3d-converter"
    location "pear-3d-converter"
    kind "ConsoleApp"
    language "C++"

    targetdir ("bin/" .. outputDir .. "/%{prj.name}")
    objdir ("bin-intermediate/" .. outputDir .. "/%{prj.name}")

    debugdir ("bin/" .. outputDir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
    }

    includedirs
    {
        "pear-3d-converter/include",
        "pear-3d-converter/include/vendor",
        "pear-3d/include"
    }
    
    links {
        "pear-3d"
    }

    filter "system:linux"
        defines {
            "PEAR_PLATFORM_LINUX",
            "PEAR_PLATFORM_OPENGL"
        }
        links {
            "assimp"
        }
        postbuildcommands ("cp -R assets/* ../bin/" .. outputDir .. "/%{prj.name}/")

    filter "configurations:Debug"
        defines
        {
            "PEAR_DEBUG",
        }
        symbols "On"

    filter "configurations:Release"
        optimize "On"
