#ifdef PEAR_PLATFORM_OPENGL

#include <graphics/platform/opengl/renderers/billboard_renderer.h>
#include <graphics/platform/opengl/renderer.h>
#include <graphics/platform/opengl/shader.h>
#include <graphics/platform/opengl/mesh.h>
#include <graphics/platform/opengl/texture.h>
#include <scene/components/transform.h>
#include <components/editor_billboard.h>
#include <util/array.h>
#include <core/alloc.h>
#include <glad/glad.h>

typedef struct editor_billboard_renderer_t {
    shader_t* shader;
    mesh_t* mesh;
    array_t* textures;
    array_t* billboard_transforms;
} editor_billboard_renderer_t;

void editorbillboardrenderer_init_mesh(editor_billboard_renderer_t* renderer) {
    vec3 positions[] = {
        {  1.0f,  1.0f, 0.0f },
        {  1.0f, -1.0f, 0.0f },
        { -1.0f, -1.0f, 0.0f },
        { -1.0f,  1.0f, 0.0f }  
    };
    vec2 texture_coords[] = {
        { 1.0f, 1.0f },
        { 1.0f, 0.0f },
        { 0.0f, 0.0f },
        { 0.0f, 1.0f }
    };
    u32 indices[] = {
        0, 3, 1,
        1, 3, 2
    };

    mesh_info_t* mesh_info = meshinfo_new();
    meshinfo_add_position(mesh_info, positions, 4);
    meshinfo_add_texture_coords(mesh_info, texture_coords, 4);
    meshinfo_add_indices(mesh_info, indices, 6);
    renderer->mesh = mesh_new(mesh_info, 0);
    meshinfo_delete(mesh_info);
}

void editorbillboardrenderer_system(scene_t* scene, entity_t* entity, f32 timestep, void* user_pointer) {
    if (!scene_has_component(scene, entity, "transform") || !scene_has_component(scene, entity, "editor_billboard")) {
        return;
    }

    renderer_interface_t* interface = (renderer_interface_t*)user_pointer;
    editor_billboard_renderer_t* billboard_renderer = (editor_billboard_renderer_t*)interface->renderer;
    transform_component_t* transform = scene_get_component(scene, entity, "transform");
    editor_billboard_component_t* billboard = scene_get_component(scene, entity, "editor_billboard");

    if (!billboard->draw || billboard->texture == NULL) {
        return;
    }

    array_add(billboard_renderer->textures, billboard->texture);
    array_add(billboard_renderer->billboard_transforms, transform);
}

void editorbillboardrenderer_draw(renderer_interface_t* interface, renderer_t* renderer) {
    editor_billboard_renderer_t* billboard_renderer = (editor_billboard_renderer_t*)interface->renderer;

    for (u32 i = 0; i < array_get_length(billboard_renderer->textures); i++) {
        texture_t* texture = array_get(billboard_renderer->textures, i);
        transform_component_t* transform = array_get(billboard_renderer->billboard_transforms, i);

        mat4 view_matrix;
        renderer_get_view_matrix(renderer, view_matrix);
        vec3 camera_right = { view_matrix[0][0], view_matrix[1][0], view_matrix[2][0] };
        vec3 camera_up = { view_matrix[0][1], view_matrix[1][1], view_matrix[2][1] };

        shader_use(billboard_renderer->shader);
        shader_set_vec3(billboard_renderer->shader, transform->pos, "u_billboard_pos");
        shader_set_vec3(billboard_renderer->shader, transform->scale, "u_billboard_size");
        shader_set_vec3(billboard_renderer->shader, camera_right, "u_camera_right");
        shader_set_vec3(billboard_renderer->shader, camera_up, "u_camera_up");
        shader_set_i32(billboard_renderer->shader, 0, "u_texture");

        mat4 model_matrix = GLM_MAT4_IDENTITY_INIT;
        glm_translate(model_matrix, transform->pos);

        ubo_t* ubo_matrices = renderer_get_matrices_ubo(renderer);
        ubo_use(ubo_matrices);
        ubo_set_mat4(ubo_matrices, 0, model_matrix);

        texture_use(texture, 0);
        mesh_use(billboard_renderer->mesh);
        glDrawElements(GL_TRIANGLES, mesh_get_num_indices(billboard_renderer->mesh), GL_UNSIGNED_INT, 0);
    }
}

void editorbillboardrenderer_clear(renderer_interface_t* interface, renderer_t* renderer, f32 r, f32 g, f32 b) {
    editor_billboard_renderer_t* billboard_renderer = (editor_billboard_renderer_t*)interface->renderer;
    array_clear(billboard_renderer->textures);
    array_clear(billboard_renderer->billboard_transforms);
}

void editorbillboardrenderer_delete(renderer_interface_t* interface) {
    editor_billboard_renderer_t* billboard_renderer = (editor_billboard_renderer_t*)interface->renderer;
    array_delete(billboard_renderer->textures);
    array_delete(billboard_renderer->billboard_transforms);
    shader_delete(billboard_renderer->shader);
    PEAR_FREE(billboard_renderer);
    PEAR_FREE(interface);
}

renderer_interface_t* editorbillboardrenderer_new(renderer_t* renderer) {
    editor_billboard_renderer_t* billboard_renderer = (editor_billboard_renderer_t*)PEAR_MALLOC(sizeof(editor_billboard_renderer_t));
    billboard_renderer->textures = array_new(10);
    billboard_renderer->billboard_transforms = array_new(10);
    editorbillboardrenderer_init_mesh(billboard_renderer);

    billboard_renderer->shader = shader_new_from_file("shaders/billboard.vert", "shaders/billboard.frag");
    shader_use(billboard_renderer->shader);
    shader_set_ubo(billboard_renderer->shader, renderer_get_matrices_ubo(renderer), "ubo_matrices");

    renderer_interface_t* interface = (renderer_interface_t*)PEAR_MALLOC(sizeof(renderer_interface_t));
    interface->system = editorbillboardrenderer_system;
    interface->draw_function = editorbillboardrenderer_draw;
    interface->clear_function = editorbillboardrenderer_clear;
    interface->delete_function = editorbillboardrenderer_delete;
    interface->renderer = billboard_renderer;

    return interface;
}

#endif
