#ifndef PEAR_FRAMEBUFFER_H
#define PEAR_FRAMEBUFFER_H

#include <graphics/texture.h>
#include <core/types.h>

typedef struct Framebuffer Framebuffer;

Framebuffer* framebuffer_new(u32 width, u32 height, TextureFormat* formats, u32 num_formats, bool has_depth);
void framebuffer_delete(Framebuffer* framebuffer);

Texture* framebuffer_get_texture(Framebuffer* framebuffer, u32 index);

#endif
