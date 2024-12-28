#ifdef PEAR_ENABLE_EDITOR

#include <graphics/editor/renderer_inspector.h>
#include <graphics/renderer.h>
#include <core/app.h>

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <graphics/editor/cimgui/cimgui.h>

void editor_renderer_inspector(bool* show) {
    if (igBegin("renderer inspector", show, ImGuiWindowFlags_None)) {
        #ifdef PEAR_PLATFORM_OPENGL
            igText("graphics api: opengl");
        #else
            igText("graphics api: unknow");
        #endif

        igText("%d meshes", renderer_get_num_meshes(app_get_renderer()));
        igText("%d vertices", renderer_get_num_vertices(app_get_renderer()));

        static bool enable_wireframe = false;
        if (igCheckbox("wireframe", &enable_wireframe)) {
            renderer_enable_wireframe(app_get_renderer(), enable_wireframe);
        }

        igEnd();
    }
}

#endif
