#ifndef PEAR_GRAPHICS_CUBEMAP_H_
#define PEAR_GRAPHICS_CUBEMAP_H_

#include <graphics/texture.h>

typedef struct cubemap_t cubemap_t;

cubemap_t* cubemap_new(u32 width, u32 height, texture_wrapping_t wrapping, texture_filtering_t filtering, texture_format_t format);
cubemap_t* cubemap_new_from_images(image_t** images, texture_wrapping_t wrapping, texture_filtering_t filtering);
void cubemap_delete(cubemap_t* cubemap);

#endif
