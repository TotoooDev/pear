#ifdef PEAR_PLATFORM_OPENGL

#ifndef PEAR_GRAPHICS_OPENGL_SCREEN_RENDERER_H_
#define PEAR_GRAPHICS_OPENGL_SCREEN_RENDERER_H_

#include <graphics/texture.h>
#include <core/types.h> 

typedef struct screen_renderer_t screen_renderer_t;

screen_renderer_t* screenrenderer_new(texture_t* screen_texture);
void screenrenderer_delete(screen_renderer_t* renderer);

void screenrenderer_render_to_screen(screen_renderer_t* renderer);
void screenrenderer_set_screen_texture(screen_renderer_t* renderer, texture_t* screen_texture);

#endif

#endif
