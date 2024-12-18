#ifndef PEAR_GRAPHICS_FRAMEBUFFER_H_
#define PEAR_GRAPHICS_FRAMEBUFFER_H_

#include <graphics/texture.h>
#include <core/types.h>

typedef struct framebuffer_t framebuffer_t;

framebuffer_t* framebuffer_new();
void framebuffer_delete(framebuffer_t* framebuffer);

void framebuffer_add_texture(framebuffer_t* framebuffer, texture_t* texture);

#endif
