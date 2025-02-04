project "pear"
    kind "StaticLib"
    language "C"

    debugdir (bin_directory)
    targetdir (bin_directory)
    objdir (bin_intermediate_directory)

    includedirs {
        "include",
        "vendor/lua",
        "vendor/imgui/cimgui",
        "vendor/imgui/cimgui/generator/output", -- for cimgui_impl.h
        "vendor/imgui/cimguizmo",
        "vendor/cglm/include"
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
            "vendor/glfw/include",
            "vendor/glad/include",
        }

        files {
            "vendor/glad/src/**.c",
            "vendor/glad/include/**.h",
        }

    filter "system:linux"
        defines {
            "PEAR_PLATFORM_LINUX",
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
