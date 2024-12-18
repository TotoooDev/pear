#ifdef PEAR_PLATFORM_OPENGL

#ifndef PEAR_GRAPHICS_OPENGL_SCENE_RENDERER_H_
#define PEAR_GRAPHICS_OPENGL_SCENE_RENDERER_H_

#include <scene/scene.h>
#include <core/types.h> 

typedef struct scene_renderer_t scene_renderer_t;

scene_renderer_t* scenerenderer_new();
void scenerenderer_delete();

void scenerenderer_clear(scene_renderer_t* renderer, f32 r, f32 g, f32 b);
void scenerenderer_draw_scene(scene_renderer_t* renderer, scene_t* scene);

#endif

#endif
