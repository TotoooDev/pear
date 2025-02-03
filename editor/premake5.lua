project "editor"
    kind "ConsoleApp"
    language "C"

    debugdir (bin_directory)
    targetdir (bin_directory)
    objdir (bin_intermediate_directory)

    -- TO FIX
    libdirs ("../pear/lib")
    links {
        "cimgui_glfw",
        "lua",
        "pear",
    }

    includedirs {
        "include",
        "../pear/include"
    }

    files {
        "include/**.h",
        "src/**.c"
    }

    filter "platforms:opengl"
        defines {
            "PEAR_PLATFORM_OPENGL",
            "PEAR_PLATFORM_GLFW",
        }

    filter "system:linux"
        defines {
            "PEAR_PLATFORM_LINUX",
        }
        linkoptions {
            "-Wl,-rpath='${ORIGIN}.'"
        }

        postbuildcommands {
            "cp -R assets/* " .. bin_directory,
            "cp -R ../pear/assets/* " .. bin_directory,
            "cp ../pear/lib/libcimgui_glfw.so " .. bin_directory
        }

        -- TO FIX
        links {
            "m",
            "glfw",
            "GL",
            "GLEW"
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
