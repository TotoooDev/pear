project "imgui"
    kind "SharedLib"
    language "C++"

    includedirs {
        "cimgui",
        "cimgui/imgui",
        "cimguizmo",
        "cimguizmo/ImGuizmo",
    }

    files {
        "cimgui/cimgui.cpp",
        "cimgui/imgui/imgui.cpp",
        "cimgui/imgui/imgui_draw.cpp",
        "cimgui/imgui/imgui_demo.cpp",
        "cimgui/imgui/imgui_tables.cpp",
        "cimgui/imgui/imgui_widgets.cpp",

        "cimguizmo/cimguizmo.cpp",
        "cimguizmo/ImGuizmo/GraphEditor.cpp",
        "cimguizmo/ImGuizmo/ImCurveEdit.cpp",
        "cimguizmo/ImGuizmo/ImGradient.cpp",
        "cimguizmo/ImGuizmo/ImGuizmo.cpp",
        "cimguizmo/ImGuizmo/ImSequencer.cpp",
    }

    filter "platforms:opengl"
        defines { "CIMGUI_USE_GLFW", "CIMGUI_USE_OPENGL3" } 
        files {
            "cimgui/imgui/backends/imgui_impl_glfw.cpp",
            "cimgui/imgui/backends/imgui_impl_opengl3.cpp",
        }

    filter "system:linux"
        pic "On"
        defines { "IMGUI_IMPL_API=extern \"C\"" }

    filter "system:windows"
        pic "On"
        defines { "IMGUI_IMPL_API=extern \"C\" __declspec(dllexport)" }
