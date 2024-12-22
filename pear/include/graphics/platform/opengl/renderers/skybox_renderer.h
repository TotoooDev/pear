#ifdef PEAR_PLATFORM_OPENGL

#ifndef PEAR_GRAPHICS_OPENGL_SKYBOX_RENDERER_H_
#define PEAR_GRAPHICS_OPENGL_SKYBOX_RENDERER_H_

#include <graphics/platform/opengl/ubo.h>
#include <scene/scene.h>

typedef struct skybox_renderer_t skybox_renderer_t;

skybox_renderer_t* skyboxrenderer_new(ubo_t* ubo_matrices);
void skyboxrenderer_delete(skybox_renderer_t* renderer);

void skyboxrenderer_draw_scene(skybox_renderer_t* renderer, array_t* skyboxes);

#endif

#endif
