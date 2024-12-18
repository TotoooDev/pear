#ifdef PEAR_PLATFORM_OPENGL

#ifndef PEAR_GRAPHICS_OPENGL_TEXTURE_H_
#define PEAR_GRAPHICS_OPENGL_TEXTURE_H_

#include <graphics/texture.h>
#include <GL/glew.h>

GLenum texture_format_to_opengl(texture_format_t format);
u32 texture_get_id(texture_t* texture);
void texture_use(texture_t* texture, u32 texture_id);

#endif

#endif
