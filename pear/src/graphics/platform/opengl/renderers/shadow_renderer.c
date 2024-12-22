#ifdef PEAR_PLATFORM_OPENGL

#include <graphics/platform/opengl/renderers/shadow_renderer.h>
#include <graphics/platform/opengl/shader.h>
#include <graphics/platform/opengl/mesh.h>
#include <graphics/camera.h>
#include <scene/components/model.h>
#include <scene/components/transform.h>
#include <scene/components/camera.h>
#include <scene/components/light.h>
#include <core/log.h>
#include <GL/glew.h>
#include <core/alloc.h>

typedef struct shadow_renderer_t {
    ubo_t* ubo_matrices;
    shader_t* shader;
    texture_t* screen_texture;
} shadow_renderer_t;

void shadowrenderer_get_frustum_corners(vec4* corners, mat4 projection, mat4 view) {
    mat4 transform;
    mat4 inverse;
    glm_mat4_mul(projection, view, transform);
    glm_mat4_inv(transform, inverse);

    u32 index = 0;
    for (u32 x = 0; x < 2; x++) {
        for (u32 y = 0; y < 2; y++) {
            for (u32 z = 0; z < 2; z++) {
                vec4 point = {
                    2.0f * x - 1.0f,
                    2.0f * y - 1.0f,
                    2.0f * z - 1.0f,
                    1.0f,
                };
                glm_mat4_mulv(inverse, point, point);
                glm_vec4_divs(point, point[3], point);
                glm_vec4_copy(point, corners[index]);
                
                index++;
            }
        }
    }
}

void shadowrenderer_get_light_view(vec4* corners, vec3 light_dir, mat4 dest) {
    vec3 center = { 0.0f, 0.0f, 0.0f };
    for (u32 i = 0; i < 8; i++) {
        glm_vec3_add(center, (vec3){ corners[i][0], corners[i][1], corners[i][2] }, center);
    }
    glm_vec3_divs(center, 8.0f, center);

    vec3 light_pos;
    glm_vec3_sub(center, light_dir, light_pos);
    glm_lookat(light_pos, center, (vec3){ 0.0f, 1.0f, 0.0f }, dest);
}

void shadowrenderer_get_light_projection(vec4* corners, mat4 light_view, mat4 dest) {
    f32 x_min = FLT_MAX;
    f32 x_max = FLT_MIN;
    f32 y_min = FLT_MAX;
    f32 y_max = FLT_MIN;
    f32 z_min = FLT_MAX;
    f32 z_max = FLT_MIN;

    for (u32 i = 0; i < 8; i++) {
        vec4 transform;
        glm_mat4_mulv(light_view, corners[i], transform);

        x_min = glm_min(x_min, transform[0]);
        x_max = glm_max(x_max, transform[0]);
        y_min = glm_min(y_min, transform[1]);
        y_max = glm_max(y_max, transform[1]);
        z_min = glm_min(z_min, transform[2]);
        z_max = glm_max(z_max, transform[2]);
    }

    f32 z_multiplier = 10.0f;
    if (z_min < 0.0f) {
        z_min *= z_multiplier;
    } else {
        z_min /= z_multiplier;
    }
    if (z_max < 0.0f) {
        z_max /= z_multiplier;
    } else {
        z_max *= z_multiplier;
    }

    glm_ortho(x_min, x_max, y_min, y_max, z_min, z_max, dest);
}

shadow_renderer_t* shadowrenderer_new(ubo_t* ubo_matrices, texture_t* screen_texture) {
    shadow_renderer_t* renderer = (shadow_renderer_t*)PEAR_MALLOC(sizeof(shadow_renderer_t));

    renderer->ubo_matrices = ubo_matrices;
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

void shadowrenderer_draw_scene(shadow_renderer_t* renderer, scene_t* scene, mat4 projection) {
    vec4 frustum_corners[8];
    mat4 view;

    for (u32 i = 0; i < array_get_length(scene_get_entities(scene)); i++) {
        entity_t* entity = array_get(scene_get_entities(scene), i);

        if (entity_has_component(entity, ENTITY_COMPONENT_CAMERA) && entity_has_component(entity, ENTITY_COMPONENT_TRANSFORM)) {
            transform_component_t* transform = (transform_component_t*)entity_get_component(entity, ENTITY_COMPONENT_TRANSFORM);
            camera_component_t* cam = (camera_component_t*)entity_get_component(entity, ENTITY_COMPONENT_CAMERA);

            mat4 light_view;
            mat4 light_projection;
            mat4 light_space_transform;
            
            camera_get_view_matrix(transform->pos, transform->rotation[0], transform->rotation[1], transform->rotation[2], view);
            shadowrenderer_get_frustum_corners(frustum_corners, projection, view);
        }
    }

    mat4 light_view = GLM_MAT4_IDENTITY_INIT;
    mat4 light_projection = GLM_MAT4_IDENTITY_INIT;
    mat4 light_space_transform = GLM_MAT4_IDENTITY_INIT;

    for (u32 i = 0; i < array_get_length(scene_get_entities(scene)); i++) {
        entity_t* entity = array_get(scene_get_entities(scene), i);

        if (entity_has_component(entity, ENTITY_COMPONENT_LIGHT) && entity_has_component(entity, ENTITY_COMPONENT_TRANSFORM)) {
            transform_component_t* transform = (transform_component_t*)entity_get_component(entity, ENTITY_COMPONENT_TRANSFORM);
            light_component_t* light = (light_component_t*)entity_get_component(entity, ENTITY_COMPONENT_LIGHT);

            if (!light->cast || !light->shadow_caster || light->light.type != LIGHT_TYPE_DIRECTIONAL) {
                continue;
            }


            vec3 light_direction;
            glm_vec3_normalize_to(transform->rotation, light_direction);
            
            shadowrenderer_get_light_view(frustum_corners, light_direction, light_view);
            shadowrenderer_get_light_projection(frustum_corners, light_view, light_projection);
            glm_mat4_mul(light_projection, light_view, light_space_transform);

        }
    }
    
    ubo_use(renderer->ubo_matrices);
    ubo_set_mat4(renderer->ubo_matrices, 4, light_space_transform);

    for (u32 i = 0; i < array_get_length(scene_get_entities(scene)); i++) {
        entity_t* entity = array_get(scene_get_entities(scene), i);

        if (entity_has_component(entity, ENTITY_COMPONENT_MODEL) && entity_has_component(entity, ENTITY_COMPONENT_TRANSFORM)) {
            transform_component_t* transform = (transform_component_t*)entity_get_component(entity, ENTITY_COMPONENT_TRANSFORM);
            model_component_t* model = (model_component_t*)entity_get_component(entity, ENTITY_COMPONENT_MODEL);

            if (!model->shadow_caster) {
                continue;
            }

            mat4 model_matrix;
            transformcomponent_get_model_matrix(transform, model_matrix);

            for (u32 j = 0; j < model_get_num_meshes(model->model); j++) {
                mesh_t* mesh = model_get_meshes(model->model)[j];

                ubo_use(renderer->ubo_matrices);
                ubo_set_mat4(renderer->ubo_matrices, 0, model_matrix);

                shader_use(renderer->shader);
                shader_set_ubo(renderer->shader, renderer->ubo_matrices, "ubo_matrices");

                mesh_use(mesh);
                glDrawElements(GL_TRIANGLES, mesh_get_num_indices(mesh), GL_UNSIGNED_INT, 0);
            }
        }
    }
}

#endif
