#ifdef PEAR_ENABLE_EDITOR

#ifdef PEAR_PLATFORM_OPENGL

#include <graphics/editor/editor.h>
#include <graphics/platform/opengl/window.h>
#include <util/array.h>
#include <core/app.h>

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#define CIMGUI_USE_GLFW
#define CIMGUI_USE_OPENGL3
#include <graphics/editor/vendor/cimgui/cimgui.h>
#include <graphics/editor/vendor/cimgui/cimconfig.h>
#include <graphics/platform/opengl/editor/vendor/cimgui/cimgui_impl.h>

static array_t* editor_functions;
static array_t* editor_user_datas;

void editor_init() {
    igCreateContext(NULL);

    // set docking
    ImGuiIO *ioptr = igGetIO();
    // ioptr->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
    //ioptr->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

    ImGui_ImplGlfw_InitForOpenGL(window_get_glfw(app_get_window()), true);
    ImGui_ImplOpenGL3_Init("#version 130");

    igStyleColorsDark(NULL);

    editor_functions = array_new(10);
    editor_user_datas = array_new(10);
}

void editor_free() {
    array_delete(editor_functions);
    array_delete(editor_user_datas);

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
    for (u32 i = 0; i < array_get_length(editor_functions); i++) {
        editor_function_t function = array_get(editor_functions, i);
        void* user_data = array_get(editor_user_datas, i);
        function(user_data);
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

void editor_add_function(editor_function_t function, void* user_data) {
    array_add(editor_functions, function);
    array_add(editor_user_datas, user_data);
}

#endif

#endif
