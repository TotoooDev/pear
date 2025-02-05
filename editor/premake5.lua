project "editor"
    kind "ConsoleApp"
    language "C"

    debugdir (bin_directory)
    targetdir (bin_directory)
    objdir (bin_intermediate_directory)

    links {
        "imgui",
        "lua",
        "pear",
    }

    includedirs {
        "include",
        "../pear/include",
        "../pear/vendor/lua/src",
        "../pear/vendor/imgui/cimgui",
        "../pear/vendor/imgui/cimgui/generator/output", -- for cimgui_impl.h
        "../pear/vendor/imgui/cimguizmo",
        "../pear/vendor/cglm/include"
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

        includedirs {
            "../pear/vendor/glfw/include",
            "../pear/vendor/glad/include",
        }

        files {
            "vendor/glad/src/**.c",
            "vendor/glad/include/**.h",
        }

        links { "glfw" }

    filter "system:linux"
        defines {
            "PEAR_PLATFORM_LINUX",
        }

        linkoptions {
            "-Wl,-rpath='${ORIGIN}.'"
        }
        links { "m", "pthread" }

        postbuildcommands {
            "cp -R assets/* " .. bin_directory,
            "cp -R ../pear/assets/* " .. bin_directory
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
