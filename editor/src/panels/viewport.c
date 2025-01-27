#include <panels/viewport.h>
#include <editor.h>
#include <scene/components/transform.h>
#include <graphics/renderer.h>
#include <graphics/editor/editor.h>
#include <core/app.h>

#ifdef PEAR_PLATFORM_OPENGL
#include <graphics/platform/opengl/texture.h>
#endif

void panel_viewport_gizmo(ImVec2 pos, ImVec2 size) {
    entity_t* entity = editor_get_selected_entity();
    if (entity == NULL) {
        return;
    }
    if (!scene_has_component(app_get_scene(), entity, "transform")) {
        return;
    }

    transform_component_t* transform = scene_get_component(app_get_scene(), entity, "transform");

    mat4 model_matrix;
    transformcomponent_get_model_matrix(transform, model_matrix);

    mat4 view;
    mat4 projection;
    renderer_get_view_matrix(app_get_renderer(), view);
    renderer_get_projection_matrix(app_get_renderer(), projection);

    ImGuizmo_SetOrthographic(false);
    ImGuizmo_SetDrawlist(NULL);
    ImGuizmo_SetRect(pos.x, pos.y, size.x, size.y);
    if (ImGuizmo_Manipulate((f32*)view, (f32*)projection, UNIVERSAL, WORLD, (f32*)model_matrix, NULL, NULL, NULL, NULL)) {
        ImGuizmo_DecomposeMatrixToComponents((f32*)model_matrix, transform->pos, transform->rotation, transform->scale);
    }
}

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

        panel_viewport_gizmo(pos, size);
    }
    igEnd();
    igPopStyleVar(1);
}
