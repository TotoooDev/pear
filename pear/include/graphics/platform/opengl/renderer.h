#ifdef PEAR_PLATFORM_OPENGL

#ifndef PEAR_GRAPHICS_OPENGL_RENDERER_H_
#define PEAR_GRAPHICS_OPENGL_RENDERER_H_

#include <graphics/renderer.h>
#include <graphics/platform/opengl/ubo.h>

ubo_t* renderer_get_matrices_ubo(renderer_t* renderer);
ubo_t* renderer_get_lights_ubo(renderer_t* renderer);

#endif

#endif
