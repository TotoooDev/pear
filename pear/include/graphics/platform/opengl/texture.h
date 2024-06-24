#ifndef PEAR_OPENGL_TEXTURE_H
#define PEAR_OPENGL_TEXTURE_H

#include <graphics/texture.h>

Texture* texture_new_depth(u32 width, u32 height, TextureWrapping wrapping, TextureFiltering filtering);

u32 texture_get_id(Texture* texture);
void texture_use(Texture* texture);

#endif
