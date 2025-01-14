#ifdef PEAR_ENABLE_EDITOR

#ifdef PEAR_PLATFORM_OPENGL

#include <graphics/editor/editor.h>
#include <graphics/editor/menu_bar.h>
#include <graphics/editor/general_info.h>
#include <graphics/editor/scene_inspector.h>
#include <graphics/editor/entity_inspector.h>
#include <graphics/editor/renderer_inspector.h>
#include <graphics/platform/opengl/window.h>
#include <core/app.h>

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#define CIMGUI_USE_GLFW
#define CIMGUI_USE_OPENGL3
#include <graphics/editor/vendor/cimgui/cimgui.h>
#include <graphics/editor/cimgui/cimconfig.h>
#include <graphics/platform/opengl/editor/cimgui/cimgui_impl.h>

static bool editor_show_menu_bar = true;
static bool editor_show_general_info = true;
static bool editor_show_scene_inspector = true;
static bool editor_show_entity_inspector = true;
static bool editor_show_renderer_inspector = true;
static bool editor_show_demo = false;

void editor_init() {
    igCreateContext(NULL);

    // set docking
    ImGuiIO *ioptr = igGetIO();
    // ioptr->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
    //ioptr->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

    ImGui_ImplGlfw_InitForOpenGL(window_get_glfw(app_get_window()), true);
    ImGui_ImplOpenGL3_Init("#version 130");

    igStyleColorsDark(NULL);
}

void editor_free() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    igDestroyContext(NULL);
}

void editor_clear() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    igNewFrame();
}

void editor_render() {
    if (editor_show_menu_bar) {
        editor_menu_bar(&editor_show_general_info);
    }
    
    if (editor_show_general_info) {
        editor_general_info(&editor_show_general_info);
    }

    if (editor_show_scene_inspector) {
        editor_scene_inspector(&editor_show_scene_inspector);
    }

    if (editor_show_entity_inspector) {
        editor_entity_inspector(&editor_show_entity_inspector);
    }

    if (editor_show_renderer_inspector) {
        editor_renderer_inspector(&editor_show_renderer_inspector);
    }

    if (editor_show_demo) {
        igShowDemoWindow(&editor_show_demo);
    }

    igRender();
    glfwMakeContextCurrent(window_get_glfw(app_get_window()));
    ImGui_ImplOpenGL3_RenderDrawData(igGetDrawData());

#ifdef IMGUI_HAS_DOCK
    ImGuiIO *ioptr = igGetIO();
    if (ioptr->ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow *backup_current_window = glfwGetCurrentContext();
        igUpdatePlatformWindows();
        igRenderPlatformWindowsDefault(NULL, NULL);
        glfwMakeContextCurrent(backup_current_window);
    }
#endif
}

void editor_enable_menu_bar(bool enable) {
    editor_show_menu_bar = enable;
}

void editor_enable_general_info(bool enable) {
    editor_show_general_info = enable;
}

void editor_enable_scene_inspector(bool enable) {
    editor_show_scene_inspector = enable;
}

void editor_enable_entity_inspector(bool enable) {
    editor_show_entity_inspector = enable;
}

void editor_enable_renderer_inspector(bool enable) {
    editor_show_renderer_inspector = enable;
}

void editor_enable_demo(bool enable) {
    editor_show_demo = enable;
}

#endif

#endif
