#include <panels/viewport.h>
#include <editor.h>
#include <graphics/renderer.h>
#include <graphics/editor/editor.h>
#include <core/app.h>

#ifdef PEAR_PLATFORM_OPENGL
#include <graphics/platform/opengl/texture.h>
#endif

void panel_viewport() {
    igPushStyleVar_Vec2(ImGuiStyleVar_WindowPadding, (ImVec2){ 0.0f, 0.0f });
    if (igBegin("viewport", NULL, ImGuiWindowFlags_None)) {
        editor_set_viewport_hovered(igIsWindowHovered(ImGuiHoveredFlags_None));

        ImVec2 pos;
        ImVec2 size;
        igGetWindowPos(&pos);
        igGetContentRegionAvail(&size);
        texture_t* texture = renderer_get_screen_texture(app_get_renderer());
        texture_t* texture_depth = renderer_get_screen_depth_texture(app_get_renderer());

        if (texture_get_width(texture) != size.x || texture_get_height(texture) != size.y) {
            renderer_set_viewport_size(app_get_renderer(), size.x, size.y , 1.0f, 1.0f);
        }

        #ifdef PEAR_PLATFORM_OPENGL
            igImage(texture_get_id(texture), size, (ImVec2){ 0.0f, 1.0f }, (ImVec2){ 1.0f, 0.0f }, (ImVec4){ 1.0f, 1.0f, 1.0f, 1.0f }, (ImVec4){ 0.0f, 0.0f, 0.0f, 0.0f });
        #endif

        static mat4 matrix = GLM_MAT4_IDENTITY;
        mat4 view;
        mat4 projection;
        renderer_get_view_matrix(app_get_renderer(), view);
        renderer_get_projection_matrix(app_get_renderer(), projection);

        ImGuizmo_SetOrthographic(false);
        ImGuizmo_SetDrawlist(NULL);
        ImGuizmo_SetRect(pos.x, pos.y, size.x, size.y);
        ImGuizmo_Manipulate((f32*)view, (f32*)projection, TRANSLATE, WORLD, (f32*)matrix, NULL, NULL, NULL, NULL);
    }
    igEnd();
    igPopStyleVar(1);
}
