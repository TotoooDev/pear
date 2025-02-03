#ifdef PEAR_PLATFORM_OPENGL

#include <graphics/platform/opengl/renderers/model_renderer.h>
#include <graphics/platform/opengl/renderer.h>
#include <graphics/platform/opengl/shader.h>
#include <graphics/platform/opengl/mesh.h>
#include <graphics/platform/opengl/texture.h>
#include <scene/components/transform.h>
#include <scene/components/model.h>
#include <scene/components/light.h>
#include <util/array.h>
#include <core/alloc.h>
#include <glad/glad.h>

#define RENDERER_NUM_MAX_LIGHTS 128

#define RENDERER_DIFFUSE_TEXTURE_INDEX 0
#define RENDERER_SPECULAR_TEXTURE_INDEX 0
#define RENDERER_SHADOW_MAP_INDEX 10

typedef struct model_renderer_t {
    shader_t* shader_texture;
    shader_t* shader_color;

    array_t* models;
    array_t* lights;
    array_t* model_transforms;
    array_t* light_transforms;
} model_renderer_t;

void modelrenderer_init_shaders(model_renderer_t* model_renderer, renderer_t* renderer) {
    model_renderer->shader_texture = shader_new_from_file("shaders/shader.vert", "shaders/shader.frag");
    model_renderer->shader_color = shader_new_from_file("shaders/shader.vert", "shaders/color.frag");

    shader_use(model_renderer->shader_texture);
    shader_set_ubo(model_renderer->shader_texture, renderer_get_matrices_ubo(renderer), "ubo_matrices");
    shader_set_ubo(model_renderer->shader_texture, renderer_get_lights_ubo(renderer), "ubo_lights");

    shader_use(model_renderer->shader_color);
    shader_set_ubo(model_renderer->shader_color, renderer_get_matrices_ubo(renderer), "ubo_matrices");
    shader_set_ubo(model_renderer->shader_color, renderer_get_lights_ubo(renderer), "ubo_lights");
}

void modelrenderer_set_matrices(renderer_t* renderer, mat4 model_matrix) {
    mat4 model_transpose_inverse;
    glm_mat4_inv(model_matrix, model_transpose_inverse);
    glm_mat4_transpose(model_transpose_inverse);

    ubo_t* ubo_matrices = renderer_get_matrices_ubo(renderer);
    ubo_use(ubo_matrices);
    ubo_set_mat4(ubo_matrices, 0, model_matrix);
    ubo_set_mat4(ubo_matrices, 3, model_transpose_inverse);
}

void modelrenderer_set_material(shader_t* shader, material_t* material) {
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

void modelrenderer_draw_mesh(renderer_t* renderer, mesh_t* mesh, mat4 model_matrix, shader_t* shader) {
    texture_use(renderer_get_shadow_map(renderer), RENDERER_SHADOW_MAP_INDEX);
    mesh_use(mesh);
    glDrawElements(GL_TRIANGLES, mesh_get_num_indices(mesh), GL_UNSIGNED_INT, 0);
}

void modelrenderer_system(scene_t* scene, entity_t* entity, f32 timestep, void* user_pointer) {
    if (!scene_has_component(scene, entity, "transform")) {
        return;
    }

    renderer_interface_t* interface = (renderer_interface_t*)user_pointer;
    model_renderer_t* model_renderer = (model_renderer_t*)interface->renderer;
    transform_component_t* transform = scene_get_component(scene, entity, "transform");

    if (scene_has_component(scene, entity, "model")) {
        model_component_t* model = scene_get_component(scene, entity, "model");
        if (model->draw && model->model != NULL) {
            array_add(model_renderer->models, model->model);
            array_add(model_renderer->model_transforms, transform);
        }
    }

    if (scene_has_component(scene, entity, "light")) {
        light_component_t* light = scene_get_component(scene, entity, "light");
        if (light->cast) {
            array_add(model_renderer->lights, &light->light);
            array_add(model_renderer->light_transforms, transform);
        }
    }
}

void modelrenderer_draw(renderer_interface_t* interface, renderer_t* renderer) {
    model_renderer_t* model_renderer = (model_renderer_t*)interface->renderer;

    u32 index = 2;
    ubo_t* ubo_lights = renderer_get_lights_ubo(renderer);
    ubo_use(ubo_lights);
    ubo_set_u32(ubo_lights, 0, array_get_length(model_renderer->lights));
    for (u32 i = 0; i < array_get_length(model_renderer->lights); i++) {
        light_t* light = array_get(model_renderer->lights, i);
        transform_component_t* transform = array_get(model_renderer->light_transforms, i);

        ubo_set_u32 (ubo_lights, index++, light->type);
        ubo_set_vec3(ubo_lights, index++, transform->pos);
        ubo_set_vec3(ubo_lights, index++, transform->rotation);
        ubo_set_vec3(ubo_lights, index++, light->ambient);
        ubo_set_vec3(ubo_lights, index++, light->diffuse);
        ubo_set_vec3(ubo_lights, index++, light->specular);
        ubo_set_f32 (ubo_lights, index++, light->constant);
        ubo_set_f32 (ubo_lights, index++, light->linear);
        ubo_set_f32 (ubo_lights, index++, light->quadratic);
        ubo_set_f32 (ubo_lights, index++, light->cutoff);
        ubo_set_f32 (ubo_lights, index++, light->outer_cutoff);
    }

    for (u32 i = 0; i < array_get_length(model_renderer->models); i++) {
        model_t* model = array_get(model_renderer->models, i);
        transform_component_t* transform = array_get(model_renderer->model_transforms, i);

        mat4 model_matrix;
        transformcomponent_get_model_matrix(transform, model_matrix);

        mesh_t** meshes = model_get_meshes(model);
        material_t** materials = model_get_materials(model);
        for (u32 j = 0; j < model_get_num_meshes(model); j++) {
            mesh_t* mesh = meshes[j];
            material_t* material = materials[mesh_get_material_index(mesh)];

            shader_t* shader = NULL;
            if (material->use_color || material->diffuse == NULL) {
                shader = model_renderer->shader_color;
            }
            else {
                shader = model_renderer->shader_texture;
            }

            modelrenderer_set_matrices(renderer, model_matrix);
            modelrenderer_set_material(shader, material);
            modelrenderer_draw_mesh(renderer, mesh, model_matrix, shader);
        }
    }
}

void modelrenderer_clear(renderer_interface_t* interface, renderer_t* renderer, f32 r, f32 g, f32 b) {
    model_renderer_t* model_renderer = (model_renderer_t*)interface->renderer;
    array_clear(model_renderer->models);
    array_clear(model_renderer->lights);
    array_clear(model_renderer->model_transforms);
    array_clear(model_renderer->light_transforms);
}

void modelrenderer_delete(renderer_interface_t* interface) {
    model_renderer_t* model_renderer = (model_renderer_t*)interface->renderer;
    array_delete(model_renderer->models);
    array_delete(model_renderer->lights);
    array_delete(model_renderer->model_transforms);
    array_delete(model_renderer->light_transforms);
    shader_delete(model_renderer->shader_texture);
    shader_delete(model_renderer->shader_color);
    PEAR_FREE(model_renderer);
    PEAR_FREE(interface);
}

renderer_interface_t* modelrenderer_new(renderer_t* renderer) {
    model_renderer_t* model_renderer = (model_renderer_t*)PEAR_MALLOC(sizeof(model_renderer_t));
    modelrenderer_init_shaders(model_renderer, renderer);
    model_renderer->models = array_new(10);
    model_renderer->lights = array_new(10);
    model_renderer->model_transforms = array_new(10);
    model_renderer->light_transforms = array_new(10);

    renderer_interface_t* interface = (renderer_interface_t*)PEAR_MALLOC(sizeof(renderer_interface_t));
    interface->system = modelrenderer_system;
    interface->draw_function = modelrenderer_draw;
    interface->clear_function = modelrenderer_clear;
    interface->delete_function = modelrenderer_delete;
    interface->renderer = model_renderer;

    return interface;
}

#endif
