#ifdef PEER_PLATFORM_OPENGL

#include <graphics/renderer.h>
#include <graphics/platform/opengl/shader.h>
#include <graphics/platform/opengl/mesh.h>
#include <core/log.h>
#include <GL/glew.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Renderer {
    Shader* shader;
} Renderer;

static bool renderer_is_glew_init = false;

void renderer_set_uniforms(Renderer* renderer) {
    shader_set_u32(renderer->shader, 0, "u_platform");
}

void renderer_init_shaders(Renderer* renderer) {
    FILE* vertex_file = fopen("default.vert", "r");
    if (vertex_file == NULL) {
        PEER_ERROR("failed to open file default.vert!");
        return;
    }

    char* vertex_buffer = NULL;
    size_t vertex_length;
    ssize_t vertex_bytes_read = getdelim(&vertex_buffer, &vertex_length, '\0', vertex_file);
    if (vertex_bytes_read == -1) {
        PEER_ERROR("failed to read file default.vert!");
        return;
    }

    FILE* fragment_file = fopen("default.frag", "r");
    if (fragment_file == NULL) {
        PEER_ERROR("failed to open file default.frag!");
        return;
    }

    char* fragment_buffer = NULL;
    size_t fragment_length;
    ssize_t fragment_bytes_read = getdelim(&fragment_buffer, &fragment_length, '\0', fragment_file);
    if (fragment_bytes_read == -1) {
        PEER_ERROR("failed to read file default.frag!");
        return;
    }

    renderer->shader = shader_new(vertex_buffer, fragment_buffer);

    fclose(vertex_file);
    fclose(fragment_file);
}

Renderer* renderer_new() {
    if (renderer_is_glew_init == false) {
        if (glewInit() != GLEW_OK) {
            PEER_ERROR("failed to init glew!");
            return NULL;
        }

        renderer_is_glew_init = true;
    }

    Renderer* renderer = (Renderer*)malloc(sizeof(Renderer));

    renderer_init_shaders(renderer);

    return renderer;
}

void renderer_delete(Renderer* renderer) {
    shader_delete(renderer->shader);
    free(renderer);
}

void renderer_clear(Renderer* renderer, f32 r, f32 g, f32 b, f32 a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void renderer_draw_mesh(Renderer* renderer, Mesh* mesh, Mesh3D* node) {
    shader_use(renderer->shader);
    mesh_use(mesh);
    glDrawElements(GL_TRIANGLES, mesh_get_num_indices(mesh), GL_UNSIGNED_INT, 0);
}

#endif
