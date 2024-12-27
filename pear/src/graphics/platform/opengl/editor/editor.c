#ifdef PEAR_DEBUG

#ifdef PEAR_PLATFORM_OPENGL

#include <graphics/editor/editor.h>
#include <graphics/platform/opengl/window.h>
#include <core/app.h>

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#define CIMGUI_USE_GLFW
#define CIMGUI_USE_OPENGL3
#include <graphics/editor/cimgui/cimgui.h>
#include <graphics/editor/cimgui/cimconfig.h>
#include <graphics/platform/opengl/editor/cimgui/cimgui_impl.h>

static bool editor_show_demo = false;

void editor_init() {
    igCreateContext(NULL);

    // set docking
    ImGuiIO *ioptr = igGetIO();
    ioptr->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
    //ioptr->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    #ifdef IMGUI_HAS_DOCK
        ioptr->ConfigFlags |= ImGuiConfigFlags_DockingEnable;       // Enable Docking
    #endif

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

void editor_enable_demo(bool enable) {
    editor_show_demo = enable;
}

#endif

#endif
