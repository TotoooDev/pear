#ifdef PEAR_PLATFORM_OPENGL

#include <graphics/platform/opengl/renderers/screen_renderer.h>
#include <graphics/mesh.h>
#include <graphics/mesh_info.h>
#include <graphics/texture.h>
#include <graphics/framebuffer.h>
#include <graphics/platform/opengl/shader.h>
#include <graphics/platform/opengl/mesh.h>
#include <graphics/platform/opengl/mesh_info.h>
#include <graphics/platform/opengl/texture.h>
#include <graphics/platform/opengl/framebuffer.h>
#include <event/event_dispatcher.h>
#include <util/filesystem.h>
#include <core/app.h>
#include <core/log.h>
#include <GL/glew.h>
#include <core/alloc.h>

typedef struct screen_renderer_t {
    shader_t* shader_framebuffer;
    texture_t* framebuffer_color_texture;
    mesh_t* screen_mesh;
} screen_renderer_t;

void screenrenderer_init_shaders(screen_renderer_t* renderer) {
    renderer->shader_framebuffer = shader_new_from_file("shaders/framebuffer.vert", "shaders/framebuffer.frag");
}

void screenrenderer_init_screen_mesh(screen_renderer_t* renderer) {
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
    renderer->screen_mesh = mesh_new(mesh_info, 0);
    meshinfo_delete(mesh_info);
}

screen_renderer_t* screenrenderer_new() {
    screen_renderer_t* renderer = (screen_renderer_t*)PEAR_MALLOC(sizeof(screen_renderer_t));

    screenrenderer_init_shaders(renderer);
    screenrenderer_init_screen_mesh(renderer);    

    return renderer;
}

void screenrenderer_delete(screen_renderer_t* renderer) {
    shader_delete(renderer->shader_framebuffer);
    mesh_delete(renderer->screen_mesh);

    PEAR_FREE(renderer);
}

void screenrenderer_render_to_screen(screen_renderer_t* renderer) {
    glClear(GL_COLOR_BUFFER_BIT);

    shader_use(renderer->shader_framebuffer);
    shader_set_i32(renderer->shader_framebuffer, 0, "u_screen_texture");
    texture_use(renderer->framebuffer_color_texture, 0);
    mesh_use(renderer->screen_mesh);
    glDrawElements(GL_TRIANGLES, mesh_get_num_indices(renderer->screen_mesh), GL_UNSIGNED_INT, 0);
}

void screenrenderer_set_screen_texture(screen_renderer_t* renderer, texture_t* texture) {
    renderer->framebuffer_color_texture = texture;
}

#endif
