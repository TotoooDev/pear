#ifndef PEAR_GRAPHICS_RENDERER_H_
#define PEAR_GRAPHICS_RENDERER_H_

#include <scene/scene.h>
#include <core/types.h>

typedef struct renderer_t renderer_t;

renderer_t* renderer_new();
void renderer_delete(renderer_t* renderer);

void renderer_clear(renderer_t* renderer, f32 r, f32 g, f32 b);
void renderer_draw_scene(renderer_t* renderer, scene_t* scene);

u32 renderer_get_num_meshes(renderer_t* renderer);
u32 renderer_get_num_vertices(renderer_t* renderer);

void renderer_enable_wireframe(renderer_t* renderer, bool active);

#endif
