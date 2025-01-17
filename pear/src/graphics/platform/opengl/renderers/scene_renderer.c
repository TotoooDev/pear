#ifdef PEAR_PLATFORM_OPENGL

#include <graphics/platform/opengl/renderers/scene_renderer.h>
#include <graphics/mesh.h>
#include <graphics/mesh_info.h>
#include <graphics/texture.h>
#include <graphics/camera.h>
#include <graphics/platform/opengl/shader.h>
#include <graphics/platform/opengl/mesh.h>
#include <graphics/platform/opengl/mesh_info.h>
#include <graphics/platform/opengl/texture.h>
#include <graphics/platform/opengl/ubo_info.h>
#include <graphics/platform/opengl/ubo.h>
#include <scene/components/transform.h>
#include <scene/components/model.h>
#include <scene/components/camera.h>
#include <scene/components/light.h>
#include <event/event_dispatcher.h>
#include <util/filesystem.h>
#include <util/array.h>
#include <core/app.h>
#include <core/log.h>
#include <GL/glew.h>
#include <core/alloc.h>

#define RENDERER_NUM_MAX_LIGHTS 128

#define RENDERER_DIFFUSE_TEXTURE_INDEX 0
#define RENDERER_SPECULAR_TEXTURE_INDEX 0
#define RENDERER_SHADOW_MAP_INDEX 10

typedef struct scene_renderer_t {
    ubo_t* ubo_matrices;
    ubo_t* ubo_lights;

    shader_t* shader_texture;
    shader_t* shader_color;

    texture_t* shadow_map;

    vec3 camera_pos;
} scene_renderer_t;

void scenerenderer_init_shaders(scene_renderer_t* renderer) {
    renderer->shader_texture = shader_new_from_file("shaders/shader.vert", "shaders/shader.frag");
    renderer->shader_color = shader_new_from_file("shaders/shader.vert", "shaders/color.frag");

    shader_use(renderer->shader_texture);
    shader_set_ubo(renderer->shader_texture, renderer->ubo_matrices, "ubo_matrices");
    shader_set_ubo(renderer->shader_texture, renderer->ubo_lights, "ubo_lights");
    shader_set_i32(renderer->shader_texture, RENDERER_SHADOW_MAP_INDEX, "u_shadow_map");

    shader_use(renderer->shader_color);
    shader_set_ubo(renderer->shader_color, renderer->ubo_matrices, "ubo_matrices");
    shader_set_ubo(renderer->shader_color, renderer->ubo_lights, "ubo_lights");
    shader_set_i32(renderer->shader_color, RENDERER_SHADOW_MAP_INDEX, "u_shadow_map");
}

void scenerenderer_set_matrices(scene_renderer_t* renderer, mat4 model_matrix) {
    mat4 model_transpose_inverse;
    glm_mat4_inv(model_matrix, model_transpose_inverse);
    glm_mat4_transpose(model_transpose_inverse);

    ubo_use(renderer->ubo_matrices);
    ubo_set_mat4(renderer->ubo_matrices, 0, model_matrix);
    ubo_set_mat4(renderer->ubo_matrices, 3, model_transpose_inverse);
}

void scenerenderer_set_material(shader_t* shader, material_t* material) {
    shader_use(shader);
    shader_set_i32(shader, RENDERER_DIFFUSE_TEXTURE_INDEX, "u_material.diffuse");
    shader_set_i32(shader, RENDERER_SPECULAR_TEXTURE_INDEX, "u_material.specular");
    shader_set_vec3(shader, material->color, "u_material.color");
    shader_set_f32(shader, material->shininess, "u_material.shininess");

    if (material->diffuse != NULL) {
        texture_use(material->diffuse, RENDERER_DIFFUSE_TEXTURE_INDEX);
    }
    if (material->specular != NULL) {
        texture_use(material->specular, RENDERER_SPECULAR_TEXTURE_INDEX);
    }
}

void scenerenderer_draw_mesh(scene_renderer_t* renderer, shader_t* shader, mesh_t* mesh, material_t* material, mat4 model_matrix) {
    scenerenderer_set_matrices(renderer, model_matrix);
    scenerenderer_set_material(shader, material);
    
    texture_use(renderer->shadow_map, RENDERER_SHADOW_MAP_INDEX);

    mesh_use(mesh);
    glDrawElements(GL_TRIANGLES, mesh_get_num_indices(mesh), GL_UNSIGNED_INT, 0);
}

scene_renderer_t* scenerenderer_new(ubo_t* ubo_matrices, ubo_t* ubo_lights, texture_t* shadow_map) {
    GLenum res = glewInit();
    if (res != GLEW_OK) {
        PEAR_ERROR("failed to initialize glew! %s", glewGetErrorString(res));
        return NULL;
    }

    scene_renderer_t* renderer = (scene_renderer_t*)PEAR_MALLOC(sizeof(scene_renderer_t));

    renderer->ubo_matrices = ubo_matrices;
    renderer->ubo_lights = ubo_lights;
    renderer->shadow_map = shadow_map;

    scenerenderer_init_shaders(renderer);

    return renderer;
}

void scenerenderer_delete(scene_renderer_t* renderer) {
    shader_delete(renderer->shader_texture);
    shader_delete(renderer->shader_color);
    PEAR_FREE(renderer);
}

void scenerenderer_clear(scene_renderer_t* renderer, f32 r, f32 g, f32 b) {
    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void scenerenderer_draw_scene(scene_renderer_t* renderer, array_t* models, array_t* lights, array_t* model_transforms, array_t* light_transforms) {
    u32 index = 2;
    ubo_use(renderer->ubo_lights);
    for (u32 i = 0; i < array_get_length(lights); i++) {
        light_component_t* light = array_get(lights, i);
        transform_component_t* transform = array_get(light_transforms, i);

        ubo_set_u32 (renderer->ubo_lights, index++, light->light.type);
        ubo_set_vec3(renderer->ubo_lights, index++, transform->pos);
        ubo_set_vec3(renderer->ubo_lights, index++, transform->rotation);
        ubo_set_vec3(renderer->ubo_lights, index++, light->light.ambient);
        ubo_set_vec3(renderer->ubo_lights, index++, light->light.diffuse);
        ubo_set_vec3(renderer->ubo_lights, index++, light->light.specular);
        ubo_set_f32 (renderer->ubo_lights, index++, light->light.constant);
        ubo_set_f32 (renderer->ubo_lights, index++, light->light.linear);
        ubo_set_f32 (renderer->ubo_lights, index++, light->light.quadratic);
        ubo_set_f32 (renderer->ubo_lights, index++, light->light.cutoff);
        ubo_set_f32 (renderer->ubo_lights, index++, light->light.outer_cutoff);
    }

    for (u32 i = 0; i < array_get_length(models); i++) {
        model_component_t* model = array_get(models, i);
        transform_component_t* transform = array_get(model_transforms, i);

        mat4 model_matrix;
        transformcomponent_get_model_matrix(transform, model_matrix);

        for (u32 j = 0; j < model_get_num_meshes(model->model); j++) {
            mesh_t* mesh = model_get_meshes(model->model)[j];
            material_t* material = model_get_materials(model->model)[mesh_get_material_index(mesh)];

            shader_t* shader = NULL;
            if (material->use_color || material->diffuse == NULL) {
                shader = renderer->shader_color;
            }
            else {
                shader = renderer->shader_texture;
            }

            scenerenderer_draw_mesh(renderer, shader, mesh, material, model_matrix);
        }
    }
}

#endif
