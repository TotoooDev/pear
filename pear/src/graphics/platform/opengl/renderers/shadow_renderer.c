#ifdef PEAR_PLATFORM_OPENGL

#include <graphics/platform/opengl/renderers/shadow_renderer.h>
#include <graphics/platform/opengl/renderer.h>
#include <graphics/platform/opengl/shader.h>
#include <graphics/platform/opengl/texture.h>
#include <graphics/platform/opengl/mesh.h>
#include <scene/components/transform.h>
#include <scene/components/model.h>
#include <scene/components/light.h>
#include <util/array.h>
#include <core/alloc.h>
#include <glad/glad.h>

typedef struct shadow_renderer_t {
    shader_t* shader;
    array_t* models;
    array_t* lights;
    array_t* model_transforms;
    array_t* light_transforms;
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

void shadowrenderer_system(scene_t* scene, entity_t* entity, f32 timestep, void* user_pointer) {
    if (!scene_has_component(scene, entity, "transform")) {
        return;
    }

    renderer_interface_t* interface = (renderer_interface_t*)user_pointer;
    shadow_renderer_t* shadow_renderer = (shadow_renderer_t*)interface->renderer;
    transform_component_t* transform = scene_get_component(scene, entity, "transform");

    if (scene_has_component(scene, entity, "model")) {
        model_component_t* model = scene_get_component(scene, entity, "model");
        if (model->draw && model->shadow_caster && model->model != NULL) {
            array_add(shadow_renderer->models, model->model);
            array_add(shadow_renderer->model_transforms, transform);
        }
    }

    if (scene_has_component(scene, entity, "light")) {
        light_component_t* light = scene_get_component(scene, entity, "light");
        if (light->cast && light->shadow_caster) {
            array_add(shadow_renderer->lights, &light->light);
            array_add(shadow_renderer->light_transforms, transform);
        }
    }
}

void shadowrenderer_draw(renderer_interface_t* interface, renderer_t* renderer) {
    shadow_renderer_t* shadow_renderer = (shadow_renderer_t*)interface->renderer;

    glViewport(0, 0, RENDERER_SHADOW_MAP_SIZE, RENDERER_SHADOW_MAP_SIZE);
    framebuffer_use(renderer_get_shadow_framebuffer(renderer));

    u32 num_light_casters = 0;
    vec4 frustum_corners[8];
    mat4 light_view = GLM_MAT4_IDENTITY_INIT;
    mat4 light_projection = GLM_MAT4_IDENTITY_INIT;
    mat4 light_space_transform = GLM_MAT4_IDENTITY_INIT;

    mat4 projection;
    mat4 view;
    renderer_get_projection_matrix(renderer, projection);
    renderer_get_view_matrix(renderer, view);
    shadowrenderer_get_frustum_corners(frustum_corners, projection, view);

    for (u32 i = 0; i < array_get_length(shadow_renderer->light_transforms); i++) {
        transform_component_t* transform = array_get(shadow_renderer->light_transforms, i);
        light_t* light = array_get(shadow_renderer->lights, i);

        if (light->type != LIGHT_TYPE_DIRECTIONAL) {
            continue;
        }

        vec3 light_direction;
        glm_vec3_normalize_to(transform->rotation, light_direction);
            
        shadowrenderer_get_light_view(frustum_corners, light_direction, light_view);
        shadowrenderer_get_light_projection(frustum_corners, light_view, light_projection);
        glm_mat4_mul(light_projection, light_view, light_space_transform);

        num_light_casters++;
    }

    if (num_light_casters <= 0) {
        goto end;
    }

    ubo_t* ubo_matrices = renderer_get_matrices_ubo(renderer);
    ubo_use(ubo_matrices);
    ubo_set_mat4(ubo_matrices, 4, light_space_transform);

    for (u32 i = 0; i < array_get_length(shadow_renderer->models); i++) {
        model_t* model = array_get(shadow_renderer->models, i);
        transform_component_t* transform = array_get(shadow_renderer->model_transforms, i);

        mat4 model_matrix;
        transformcomponent_get_model_matrix(transform, model_matrix);

        for (u32 j = 0; j < model_get_num_meshes(model); j++) {
            mesh_t* mesh = model_get_meshes(model)[j];

            ubo_use(ubo_matrices);
            ubo_set_mat4(ubo_matrices, 0, model_matrix);

            shader_use(shadow_renderer->shader);
            mesh_use(mesh);
            glDrawElements(GL_TRIANGLES, mesh_get_num_indices(mesh), GL_UNSIGNED_INT, 0);
        }
    }

end:
    f32 width, height;
    renderer_get_viewport_size(renderer, NULL, NULL, &width, &height);
    glViewport(0, 0, width, height);
    framebuffer_use(renderer_get_screen_framebuffer(renderer));
}

void shadowrenderer_clear(renderer_interface_t* interface, renderer_t* renderer, f32 r, f32 g, f32 b) {
    shadow_renderer_t* shadow_renderer = (shadow_renderer_t*)interface->renderer;
    array_clear(shadow_renderer->models);
    array_clear(shadow_renderer->lights);
    array_clear(shadow_renderer->model_transforms);
    array_clear(shadow_renderer->light_transforms);

    framebuffer_use(renderer_get_shadow_framebuffer(renderer));
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void shadowrenderer_delete(renderer_interface_t* interface) {
    shadow_renderer_t* shadow_renderer = (shadow_renderer_t*)interface->renderer;
    array_delete(shadow_renderer->models);
    array_delete(shadow_renderer->lights);
    array_delete(shadow_renderer->model_transforms);
    array_delete(shadow_renderer->light_transforms);
    shader_delete(shadow_renderer->shader);
    PEAR_FREE(shadow_renderer);
    PEAR_FREE(interface);
}

renderer_interface_t* shadowrenderer_new(renderer_t* renderer) {
    shadow_renderer_t* shadow_renderer = (shadow_renderer_t*)PEAR_MALLOC(sizeof(shadow_renderer_t));
    shadow_renderer->models = array_new(10);
    shadow_renderer->lights = array_new(10);
    shadow_renderer->model_transforms = array_new(10);
    shadow_renderer->light_transforms = array_new(10);

    shadow_renderer->shader = shader_new_from_file("shaders/shadow.vert", "shaders/shadow.frag");
    shader_use(shadow_renderer->shader);
    shader_set_ubo(shadow_renderer->shader, renderer_get_matrices_ubo(renderer), "ubo_matrices");

    renderer_interface_t* interface = (renderer_interface_t*)PEAR_MALLOC(sizeof(renderer_interface_t));
    interface->system = shadowrenderer_system;
    interface->draw_function = shadowrenderer_draw;
    interface->clear_function = shadowrenderer_clear;
    interface->delete_function = shadowrenderer_delete;
    interface->renderer = shadow_renderer;

    return interface;
}

#endif
