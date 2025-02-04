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

    files {
		"pear/include/**.h",
		"pear/src/**.c"
	}

    includedirs {
		"pear/include"
	}

    filter "system:linux"
        defines {
            "PEAR_PLATFORM_LINUX",
            "PEAR_PLATFORM_GLFW",
            "PEAR_PLATFORM_OPENGL"
        }

    filter "configurations:Debug"
		defines {
			"PEAR_DEBUG",
            "PEAR_ENABLE_EDITOR"
		}
		symbols "On"

	filter "configurations:Release"
        defines {
            "PEAR_RELEASE",
            "PEAR_ENABLE_EDITOR"
        }
		optimize "On"
        symbols "On"

project "editor"
    location "editor"
    kind "ConsoleApp"
    language "C"

    targetdir ("bin/" .. outputDir .. "/%{prj.name}")
    objdir ("bin-intermediate/" .. outputDir .. "/%{prj.name}")
    libdirs ("pear/lib")

    debugdir ("bin/" .. outputDir .. "/%{prj.name}")

    files {
        "editor/src/**.h",
        "editor/src/**.c",
    }

    includedirs {
        "editor/include",
        "pear/include"
    }
    
    links {
        "cimgui_glfw",
        "lua",
        "pear",
    }

    filter "system:linux"
        linkoptions {
            "-Wl,-rpath='${ORIGIN}.'"
        }
        defines {
            "PEAR_PLATFORM_LINUX",
            "PEAR_PLATFORM_GLFW",
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
            "cp -R ../pear/assets/* ../bin/" .. outputDir .. "/%{prj.name}/",
            "cp ../pear/lib/libcimgui_glfw.so ../bin/" .. outputDir .. "/%{prj.name}/"
        }

    filter "configurations:Debug"
        defines {
            "PEAR_DEBUG",
            "PEAR_ENABLE_EDITOR"
        }
        symbols "On"

    filter "configurations:Release"
        defines {
            "PEAR_RELEASE",
            "PEAR_ENABLE_EDITOR"
        }
        optimize "On"
        symbols "On"
