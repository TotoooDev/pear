#ifdef PEAR_PLATFORM_OPENGL

#include <graphics/platform/opengl/renderers/screen_renderer.h>
#include <graphics/platform/opengl/renderer.h>
#include <graphics/platform/opengl/shader.h>
#include <graphics/platform/opengl/mesh.h>
#include <graphics/platform/opengl/texture.h>
#include <core/alloc.h>
#include <GL/glew.h>

typedef struct screen_renderer_t {
    shader_t* shader;
    mesh_t* mesh;
} screen_renderer_t;

void screenrenderer_init_mesh(screen_renderer_t* renderer) {
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

void screenrenderer_draw(renderer_interface_t* interface, renderer_t* renderer) {
    screen_renderer_t* screen_renderer = (screen_renderer_t*)interface->renderer;

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    framebuffer_use_default();
    shader_use(screen_renderer->shader);
    shader_set_i32(screen_renderer->shader, 0, "u_screen_texture");
    texture_use(renderer_get_screen_texture(renderer), 0);
    mesh_use(screen_renderer->mesh);
    glDrawElements(GL_TRIANGLES, mesh_get_num_indices(screen_renderer->mesh), GL_UNSIGNED_INT, 0);
}

void screenrenderer_clear(renderer_interface_t* interface, renderer_t* renderer, f32 r, f32 g, f32 b) {
    screen_renderer_t* screen_renderer = (screen_renderer_t*)interface->renderer;
    
    framebuffer_use_default();
    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void screenrenderer_delete(renderer_interface_t* interface) {
    screen_renderer_t* screen_renderer = (screen_renderer_t*)interface->renderer;
    shader_delete(screen_renderer->shader);
    mesh_delete(screen_renderer->mesh);
    PEAR_FREE(screen_renderer);
    PEAR_FREE(interface);
}

renderer_interface_t* screenrenderer_new(renderer_t* renderer) {
    screen_renderer_t* screen_renderer = (screen_renderer_t*)PEAR_MALLOC(sizeof(screen_renderer_t));
    screenrenderer_init_mesh(screen_renderer);

    screen_renderer->shader = shader_new_from_file("shaders/framebuffer.vert", "shaders/framebuffer.frag");
    shader_use(screen_renderer->shader);
    shader_set_ubo(screen_renderer->shader, renderer_get_matrices_ubo(renderer), "ubo_matrices");

    renderer_interface_t* interface = (renderer_interface_t*)PEAR_MALLOC(sizeof(renderer_interface_t));
    interface->system = NULL;
    interface->draw_function = screenrenderer_draw;
    interface->clear_function = screenrenderer_clear;
    interface->delete_function = screenrenderer_delete;
    interface->renderer = screen_renderer;

    return interface;
}

#endif
