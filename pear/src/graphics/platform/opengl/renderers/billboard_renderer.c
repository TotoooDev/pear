#ifdef PEAR_PLATFORM_OPENGL

#include <graphics/platform/opengl/renderers/billboard_renderer.h>
#include <graphics/platform/opengl/shader.h>
#include <graphics/platform/opengl/mesh.h>
#include <graphics/platform/opengl/texture.h>
#include <scene/components/billboard.h>
#include <scene/components/transform.h>
#include <core/alloc.h>
#include <GL/glew.h>

typedef struct billboard_renderer_t {
    ubo_t* ubo_matrices;
    shader_t* shader;
    mesh_t* mesh;
} billboard_renderer_t;

void billboardrenderer_init_screen_mesh(billboard_renderer_t* renderer) {
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

billboard_renderer_t* billboardrenderer_new(ubo_t* ubo_matrices) {
    billboard_renderer_t* renderer = (billboard_renderer_t*)PEAR_MALLOC(sizeof(billboard_renderer_t));

    renderer->ubo_matrices = ubo_matrices;
    renderer->shader = shader_new_from_file("shaders/billboard.vert", "shaders/billboard.frag");
    billboardrenderer_init_screen_mesh(renderer);

    shader_use(renderer->shader);
    shader_set_ubo(renderer->shader, renderer->ubo_matrices, "ubo_matrices");

    return renderer;
}

void billboardrenderer_delete(billboard_renderer_t* renderer) {
    mesh_delete(renderer->mesh);
    shader_delete(renderer->shader);
    PEAR_FREE(renderer);
}

void billboardrenderer_draw_scene(billboard_renderer_t* renderer, array_t* billboards, array_t* transforms, mat4 view_matrix) {
    for (u32 i = 0; i < array_get_length(billboards); i++) {
        billboard_component_t* billboard = array_get(billboards, i);
        transform_component_t* transform = array_get(transforms, i);

        vec3 camera_right = { view_matrix[0][0], view_matrix[1][0], view_matrix[2][0] };
        vec3 camera_up = { view_matrix[0][1], view_matrix[1][1], view_matrix[2][1] };

        shader_use(renderer->shader);
        shader_set_vec3(renderer->shader, transform->pos, "u_billboard_pos");
        shader_set_vec3(renderer->shader, transform->scale, "u_billboard_size");
        shader_set_vec3(renderer->shader, camera_right, "u_camera_right");
        shader_set_vec3(renderer->shader, camera_up, "u_camera_up");
        shader_set_i32(renderer->shader, 0, "u_texture");

        mat4 model_matrix = GLM_MAT4_IDENTITY_INIT;
        glm_translate(model_matrix, transform->pos);
        ubo_use(renderer->ubo_matrices);
        ubo_set_mat4(renderer->ubo_matrices, 0, model_matrix);

        texture_use(billboard->texture, 0);
        mesh_use(renderer->mesh);
        glDrawElements(GL_TRIANGLES, mesh_get_num_indices(renderer->mesh), GL_UNSIGNED_INT, 0);
    }
}

#endif
