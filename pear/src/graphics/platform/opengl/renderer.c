#ifdef PEAR_PLATFORM_OPENGL

#include <graphics/renderer.h>
#include <graphics/mesh.h>
#include <graphics/mesh_info.h>
#include <graphics/platform/opengl/shader.h>
#include <graphics/platform/opengl/mesh.h>
#include <graphics/platform/opengl/mesh_info.h>
#include <util/filesystem.h>
#include <core/log.h>
#include <GL/glew.h>
#include <core/alloc.h>

typedef struct renderer_t {
    shader_t* shader;
    mesh_t* mesh;
} renderer_t;

renderer_t* renderer_new() {
    GLenum res = glewInit();
    if (res != GLEW_OK) {
        PEAR_ERROR("failed to initialize glew! %s", glewGetErrorString(res));
        return NULL;
    }

    renderer_t* renderer = (renderer_t*)PEAR_MALLOC(sizeof(renderer_t));

    vec3 positions[] = {
        {0.5f,  0.5f, 0.0f},
        {0.5f, -0.5f, 0.0f},
        {-0.5f, -0.5f, 0.0f},
        {-0.5f,  0.5f, 0.0f}
    };
    vec3 colors[] = {
        {1.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 1.0f},
        {1.0f, 0.0f, 1.0f}
    };
    u32 indices[] = {
        0, 1, 3,
        1, 2, 3
    };
    mesh_info_t* mesh_info = meshinfo_new();
    meshinfo_add_indices(mesh_info, indices, 6);
    meshinfo_add_position(mesh_info, positions, 4);
    meshinfo_add_color(mesh_info, colors, 4);

    renderer->mesh = mesh_new(mesh_info);

    meshinfo_delete(mesh_info);
    
    renderer->shader = shader_new(fileystem_read_file("shaders/shader.vert"), fileystem_read_file("shaders/shader.frag"));

    return renderer;
}

void renderer_delete(renderer_t* renderer) {
    PEAR_FREE(renderer);
}

void renderer_clear(renderer_t* renderer, f32 r, f32 g, f32 b) {
    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void renderer_draw_scene(renderer_t* renderer, scene_t* scene) {
    shader_use(renderer->shader);

    mesh_use(renderer->mesh);
    glDrawElements(GL_TRIANGLES, mesh_get_num_indices(renderer->mesh), GL_UNSIGNED_INT, 0);
}

#endif
