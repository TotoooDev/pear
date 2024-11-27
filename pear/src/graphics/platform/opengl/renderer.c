#ifdef PEAR_PLATFORM_OPENGL

#include <graphics/renderer.h>
#include <core/log.h>
#include <GL/glew.h>
#include <stdlib.h>

typedef struct renderer_t {
    u32 foo;
} renderer_t;

renderer_t* renderer_new() {
    GLenum res = glewInit();
    if (res != GLEW_OK) {
        PEAR_ERROR("failed to initialize glew! %s", glewGetErrorString(res));
        return NULL;
    }

    renderer_t* renderer = (renderer_t*)malloc(sizeof(renderer_t));
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

}

#endif
