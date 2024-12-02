workspace "pear"
    architecture "x64"

    configurations {
        "Debug",
        "Release"
    }

outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

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
		"pear/include/vendor"
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
        "pear/include"
    }
    
    links ("pear")

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
        postbuildcommands {
            "cp -R assets/* ../bin/" .. outputDir .. "/%{prj.name}/",
            "cp -R ../pear/assets/* ../bin/" .. outputDir .. "/%{prj.name}/"
        }

    filter "configurations:Debug"
        defines
        {
            "PEAR_DEBUG",
        }
        symbols "On"

    filter "configurations:Release"
        optimize "On"
