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
		"pear/src/**.c",
		"pear/src/vendor/**.c",
	}

    includedirs {
		"pear/include",
		"pear/include/vendor",
		"pear-formats/include"
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

project "pear-project"
    location "pear-project"
    kind "ConsoleApp"
    language "C"

    targetdir ("bin/" .. outputDir .. "/%{prj.name}")
    objdir ("bin-intermediate/" .. outputDir .. "/%{prj.name}")
    libdirs ("pear/lib")

    debugdir ("bin/" .. outputDir .. "/%{prj.name}")

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.c",
    }

    includedirs {
        "pear-project/include",
        "pear-project/include/vendor",
        "pear/include/vendor",
        "pear/include",
        "pear-formats/include"
    }
    
    links {
        "cimgui",
        "pear",
        "pear-formats"
    }

    filter "system:linux"
        linkoptions {
            -- "-Wl,-rpath=/home/toto/Dev/pear/bin/Debug-linux-x86_64/pear-project/"
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
            "cp -R ../pear/lib/* ../bin/" .. outputDir .. "/%{prj.name}/",
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

project "pear-formats"
    location "pear-formats"
    kind "StaticLib"
    language "C"

    targetdir ("bin/" .. outputDir .. "/%{prj.name}")
    objdir ("bin-intermediate/" .. outputDir .. "/%{prj.name}")

    debugdir ("bin/" .. outputDir .. "/%{prj.name}")

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.c",
    }

    includedirs {
        "pear-formats/include",
        "pear-formats/include/vendor"
    }
    
    filter "configurations:Debug"
        defines {
            "PEAR_DEBUG",
        }
        symbols "On"
        
    filter "configurations:Release"
        optimize "On"
        symbols "On"

project "converter"
        location "converter"
        kind "ConsoleApp"
        language "C++"
    
        targetdir ("bin/" .. outputDir .. "/%{prj.name}")
        objdir ("bin-intermediate/" .. outputDir .. "/%{prj.name}")
    
        debugdir ("bin/" .. outputDir .. "/%{prj.name}")
    
        files {
            "%{prj.name}/src/**.hpp",
            "%{prj.name}/src/**.cpp",
        }
    
        includedirs {
            "converter/include",
            "converter/include/vendor",
            "pear-formats/include"
        }
        
        links ("pear-formats")
    
        filter "system:linux"
            defines {
                "PEAR_PLATFORM_LINUX",
            }
            links ("assimp")
            postbuildcommands {
                "cp -R assets/* ../bin/" .. outputDir .. "/%{prj.name}/",
            }
    
        filter "configurations:Debug"
            defines {
                "PEAR_DEBUG",
            }
            symbols "On"
    
        filter "configurations:Release"
            optimize "On"
