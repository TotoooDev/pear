#ifdef PEAR_PLATFORM_OPENGL

#ifndef PEAR_GRAPHICS_OPENGL_SCENE_RENDERER_H_
#define PEAR_GRAPHICS_OPENGL_SCENE_RENDERER_H_

#include <graphics/platform/opengl/ubo.h>
#include <graphics/texture.h>
#include <scene/scene.h>

typedef struct scene_renderer_t scene_renderer_t;

scene_renderer_t* scenerenderer_new(ubo_t* ubo_matrices, ubo_t* ubo_lights, texture_t* shadow_map);
void scenerenderer_delete(scene_renderer_t* renderer);

void scenerenderer_clear(scene_renderer_t* renderer, f32 r, f32 g, f32 b);
void scenerenderer_draw_scene(scene_renderer_t* renderer, array_t* models, array_t* lights, array_t* model_transforms, array_t* light_transforms);

#endif

#endif
