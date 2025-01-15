#ifdef PEAR_PLATFORM_OPENGL

#ifndef PEAR_GRAPHICS_OPENGL_BILLBOARD_RENDERER_H_
#define PEAR_GRAPHICS_OPENGL_BILLBOARD_RENDERER_H_

#include <graphics/platform/opengl/ubo.h>
#include <util/array.h>

typedef struct billboard_renderer_t billboard_renderer_t;

billboard_renderer_t* billboardrenderer_new(ubo_t* ubo_matrices);
void billboardrenderer_delete(billboard_renderer_t* renderer);

void billboardrenderer_draw_scene(billboard_renderer_t* renderer, array_t* billboards, array_t* transforms, mat4 view_matrix);

#endif

#endif
