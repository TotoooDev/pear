#include <graphics/renderer.h>
#include <core/log.h>
#include <GL/glew.h>
#include <stdlib.h>

typedef struct Renderer {
    int foo;
} Renderer;

Renderer* renderer_new() {
    if (glewInit() != GLEW_OK) {
        PEER_ERROR("failed to init glew!");
        return NULL;
    }

    Renderer* renderer = (Renderer*)malloc(sizeof(Renderer));
    return renderer;
}

void renderer_delete(Renderer* renderer) {
    free(renderer);
}

void renderer_clear(Renderer* renderer, f32 r, f32 g, f32 b, f32 a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
