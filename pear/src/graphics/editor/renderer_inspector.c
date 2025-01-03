#ifdef PEAR_ENABLE_EDITOR

#include <graphics/editor/renderer_inspector.h>
#include <graphics/renderer.h>
#include <graphics/platform/opengl/texture.h>
#include <core/app.h>

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <graphics/editor/cimgui/cimgui.h>

static texture_t* editor_shadow_map = NULL;

void editor_set_shadow_map(texture_t* shadow_map) {
    editor_shadow_map = shadow_map;
}

void editor_renderer_inspector(bool* show) {
    if (igBegin("renderer inspector", show, ImGuiWindowFlags_None)) {
        #ifdef PEAR_PLATFORM_OPENGL
            igText("graphics api: opengl");
        #else
            igText("graphics api: unknow");
        #endif

        igSeparator();

        igText("%d meshes", renderer_get_num_meshes(app_get_renderer()));
        igText("%d vertices", renderer_get_num_vertices(app_get_renderer()));

        igSeparator();

        static f32 near = 0.1f;
        if (igDragFloat("near", &near, 0.1f, 0.0f, FLT_MAX, "%.3f", ImGuiSliderFlags_None)) {
            renderer_set_near(app_get_renderer(), near);
        }

        static f32 far = 30.0f;
        if (igDragFloat("far", &far, 0.1f, 0.0f, FLT_MAX, "%.3f", ImGuiSliderFlags_None)) {
            renderer_set_far(app_get_renderer(), far);
        }

        static f32 fov = 45.0f;
        if (igDragFloat("fov", &fov, 0.1f, 0.0f, FLT_MAX, "%.3f", ImGuiSliderFlags_None)) {
            renderer_set_fov(app_get_renderer(), fov);
        }

        static bool enable_wireframe = false;
        if (igCheckbox("wireframe", &enable_wireframe)) {
            renderer_enable_wireframe(app_get_renderer(), enable_wireframe);
        }

        igSeparator();

        if (editor_shadow_map != NULL) {
            if (igTreeNode_Str("shadow map")) {
                #ifdef PEAR_PLATFORM_OPENGL
                    igImage(texture_get_id(editor_shadow_map), (ImVec2){ 128.0f, 128.0f }, (ImVec2){ 0.0f, 0.0f }, (ImVec2){ 1.0f, 1.0f }, (ImVec4){ 1.0f, 1.0f, 1.0f, 1.0f }, (ImVec4){ 0.0f, 0.0f, 0.0f, 0.0f });
                #endif

                igTreePop();
            }
        }
    }
    igEnd();
}

#endif
