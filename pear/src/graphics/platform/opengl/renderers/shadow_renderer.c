#ifdef PEAR_PLATFORM_OPENGL

#include <graphics/platform/opengl/renderers/shadow_renderer.h>
#include <graphics/platform/opengl/shader.h>
#include <graphics/platform/opengl/mesh.h>
#include <scene/components/model.h>
#include <scene/components/transform.h>
#include <core/log.h>
#include <GL/glew.h>
#include <core/alloc.h>

typedef struct shadow_renderer_t {
    shader_t* shader;
    texture_t* screen_texture;
} shadow_renderer_t;

shadow_renderer_t* shadowrenderer_new(texture_t* screen_texture) {
    shadow_renderer_t* renderer = (shadow_renderer_t*)PEAR_MALLOC(sizeof(shadow_renderer_t));

    renderer->screen_texture = screen_texture;
    renderer->shader = shader_new_from_file("shaders/shadow.vert", "shaders/shadow.frag");

    return renderer;
}

void shadowrenderer_delete(shadow_renderer_t* renderer) {
    shader_delete(renderer->shader);
    PEAR_FREE(renderer);
}

void shadowrenderer_clear(shadow_renderer_t* renderer) {
    glClear(GL_DEPTH_BUFFER_BIT);
}

void shadowrenderer_draw_scene(shadow_renderer_t* renderer, scene_t* scene) {
    mat4 projection;
    mat4 view;
    glm_ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 100.0f, projection);
    glm_lookat((vec3){ -2.0f, 40.0f, -1.0f }, (vec3){ 0.0f, 0.0f, 0.0f }, (vec3){ 0.0f, 1.0f, 0.0f }, view);

    for (u32 i = 0; i < array_get_length(scene_get_entities(scene)); i++) {
        entity_t* entity = array_get(scene_get_entities(scene), i);

        if (entity_has_component(entity, ENTITY_COMPONENT_MODEL) && entity_has_component(entity, ENTITY_COMPONENT_TRANSFORM)) {
            transform_component_t* transform = (transform_component_t*)entity_get_component(entity, ENTITY_COMPONENT_TRANSFORM);
            model_component_t* model = (model_component_t*)entity_get_component(entity, ENTITY_COMPONENT_MODEL);

            mat4 model_matrix;
            transformcomponent_get_model_matrix(transform, model_matrix);

            for (u32 j = 0; j < model_get_num_meshes(model->model); j++) {
                mesh_t* mesh = model_get_meshes(model->model)[j];

                shader_use(renderer->shader);
                shader_set_mat4(renderer->shader, projection, "u_light_projection");
                shader_set_mat4(renderer->shader, view, "u_light_view");
                shader_set_mat4(renderer->shader, model_matrix, "u_model");

                mesh_use(mesh);
                glDrawElements(GL_TRIANGLES, mesh_get_num_indices(mesh), GL_UNSIGNED_INT, 0);
            }
        }
    }
}

#endif
