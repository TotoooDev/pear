#ifdef PEAR_PLATFORM_OPENGL

#ifndef PEAR_GRAPHICS_OPENGL_TEXTURE_H_
#define PEAR_GRAPHICS_OPENGL_TEXTURE_H_

#include <graphics/texture.h>
#include <glad/glad.h>

GLenum texture_format_to_opengl(texture_format_t format);
GLenum texture_wrapping_to_opengl(texture_wrapping_t wrapping);
GLenum texture_filtering_to_opengl(texture_filtering_t filtering);
GLint texture_num_channels_to_opengl(u32 num_channels);

u32 texture_get_id(texture_t* texture);
void texture_use(texture_t* texture, u32 texture_id);
void texture_set_border_color(texture_t* texture, vec4 color);

#endif

#endif
