#ifdef PEAR_PLATFORM_OPENGL

#include <graphics/renderer.h>
#include <graphics/mesh.h>
#include <graphics/mesh_info.h>
#include <graphics/platform/opengl/shader.h>
#include <graphics/platform/opengl/mesh.h>
#include <graphics/platform/opengl/mesh_info.h>
#include <core/log.h>
#include <GL/glew.h>
#include <stdlib.h>

typedef struct renderer_t {
    shader_t* shader;
    mesh_t* mesh;
    // u32 vao, vbo, ebo;
} renderer_t;

renderer_t* renderer_new() {
    GLenum res = glewInit();
    if (res != GLEW_OK) {
        PEAR_ERROR("failed to initialize glew! %s", glewGetErrorString(res));
        return NULL;
    }

    renderer_t* renderer = (renderer_t*)malloc(sizeof(renderer_t));

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

    const char* vertex_source = "#version 330 core\n"
        "layout (location = 0) in vec3 a_pos;\n"
        "layout (location = 1) in vec3 a_color;\n"
        "out vec3 color;\n"
        "void main() {\n"
        "   color = a_color;\n"
        "   gl_Position = vec4(a_pos, 1.0f);\n"
        "}\0"
    ;
    const char* fragment_source = "#version 330 core\n"
        "out vec4 frag_color;\n"
        "in vec3 color;\n"
        "void main() {\n"
        "   // frag_color = vec4(1.0, 0.0, 1.0, 1.0);\n"
        "   frag_color = vec4(color, 1.0);\n"
        "}\0"
    ;
    renderer->shader = shader_new(vertex_source, fragment_source);

    return renderer;
}

void renderer_delete(renderer_t* renderer) {
    free(renderer);
}

void renderer_clear(renderer_t* renderer, f32 r, f32 g, f32 b) {
    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void renderer_draw_scene(renderer_t* renderer, scene_t* scene) {
    shader_use(renderer->shader);

    // glBindVertexArray(renderer->vao);
    mesh_use(renderer->mesh);
    glDrawElements(GL_TRIANGLES, mesh_get_num_indices(renderer->mesh), GL_UNSIGNED_INT, 0);
}

#endif
