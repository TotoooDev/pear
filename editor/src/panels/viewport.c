#include <panels/viewport.h>
#include <editor.h>
#include <scene/components/transform.h>
#include <graphics/renderer.h>
#include <graphics/editor/editor.h>
#include <core/app.h>
#include <core/log.h>

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
    ImGuizmo_Manipulate((f32*)view, (f32*)projection, UNIVERSAL, LOCAL, (f32*)model_matrix, NULL, NULL, NULL, NULL);
    if (ImGuizmo_IsUsing()) {
        vec4 translation;
        mat4 rotation_matrix;
        vec3 scale;
        glm_decompose(model_matrix, translation, rotation_matrix, scale);
        
        vec3 rotation;
        glm_euler_angles(rotation_matrix, rotation);
        rotation[0] = glm_deg(rotation[0]);
        rotation[1] = glm_deg(rotation[1]);
        rotation[2] = glm_deg(rotation[2]);
        
        glm_vec3_copy(translation, transform->pos);
        glm_vec3_copy(rotation, transform->rotation);
        glm_vec3_copy(scale, transform->scale);
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
