#ifdef PEAR_PLATFORM_OPENGL

#ifndef PEAR_GRAPHICS_OPENGL_RENDERER_H_
#define PEAR_GRAPHICS_OPENGL_RENDERER_H_

#include <graphics/renderer.h>
#include <graphics/platform/opengl/ubo.h>
#include <graphics/platform/opengl/framebuffer.h>

#define RENDERER_SHADOW_MAP_SIZE 4096

framebuffer_t* renderer_get_shadow_framebuffer(renderer_t* renderer);
framebuffer_t* renderer_get_screen_framebuffer(renderer_t* renderer);
ubo_t* renderer_get_matrices_ubo(renderer_t* renderer);
ubo_t* renderer_get_lights_ubo(renderer_t* renderer);

#endif

#endif
