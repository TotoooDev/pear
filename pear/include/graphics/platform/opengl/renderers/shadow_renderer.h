#ifdef PEAR_PLATFORM_OPENGL

#ifndef PEAR_GRAPHICS_OPENGL_SHADOW_RENDERER_H_
#define PEAR_GRAPHICS_OPENGL_SHADOW_RENDERER_H_

#include <graphics/platform/opengl/ubo.h>
#include <graphics/texture.h>
#include <scene/scene.h>

typedef struct shadow_renderer_t shadow_renderer_t;

shadow_renderer_t* shadowrenderer_new(ubo_t* ubo_matrices, texture_t* shadow_map);
void shadowrenderer_delete(shadow_renderer_t* renderer);

void shadowrenderer_clear(shadow_renderer_t* renderer);
void shadowrenderer_draw_scene(shadow_renderer_t* renderer, array_t* models, array_t* lights, array_t* model_transforms, array_t* light_transforms, mat4 projection, mat4 view);

#endif

#endif
