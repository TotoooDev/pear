#ifdef PEAR_PLATFORM_OPENGL

#include <graphics/platform/opengl/renderers/shadow_renderer.h>
#include <graphics/platform/opengl/shader.h>
#include <core/log.h>
#include <GL/glew.h>
#include <core/alloc.h>

typedef struct shadow_renderer_t {
    shader_t* shader;
    texture_t* screen_texture;
} shadow_renderer_t;

shadow_renderer_t* shadowrenderer_new(texture_t* screen_texture) {
    shadow_renderer_t* renderer = (shadow_renderer_t*)PEAR_MALLOC(sizeof(shadow_renderer_t));

    renderer->screen_texture = screen_texture;
    renderer->shader = shader_new_from_file("shaders/shadow.vert", "shaders/shadow.frag");

    return renderer;
}

void shadowrenderer_delete(shadow_renderer_t* renderer) {
    shader_delete(renderer->shader);
    PEAR_FREE(renderer);
}

void shadowrenderer_clear(shadow_renderer_t* renderer) {
    glClear(GL_DEPTH_BUFFER_BIT);
}

void shadowrenderer_draw_scene(shadow_renderer_t* renderer, scene_t* scene) {

}

#endif
