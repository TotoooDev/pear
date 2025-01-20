#ifndef PEAR_GRAPHICS_TEXTURE_H_
#define PEAR_GRAPHICS_TEXTURE_H_

#include <graphics/image.h>

typedef enum texture_format_t {
    TEXTURE_FORMAT_R,
    TEXTURE_FORMAT_RG,
    TEXTURE_FORMAT_RGB,
    TEXTURE_FORMAT_RGBA,
    TEXTURE_FORMAT_DEPTH,
    TEXTURE_FORMAT_STENCIL
} texture_format_t;

typedef enum texture_wrapping_t {
    TEXTURE_WRAPPING_REPEAT,
    TEXTURE_WRAPPING_REPEAT_MIRROR,
    TEXTURE_WRAPPING_CLAMP,
    TEXTURE_WRAPPING_NONE
} texture_wrapping_t;

typedef enum texture_filtering_t {
    TEXTURE_FILTERING_NEAREST,
    TEXTURE_FILTERING_LINEAR
} texture_filtering_t;

typedef struct texture_t texture_t;

texture_t* texture_new(u32 width, u32 height, texture_wrapping_t wrapping, texture_filtering_t filtering, texture_format_t format);
texture_t* texture_new_from_image(image_t* image, texture_wrapping_t wrapping, texture_filtering_t filtering);
void texture_delete(texture_t* texture);

void texture_resize(texture_t* texture, u32 width, u32 height);

u32 texture_get_width(texture_t* texture);
u32 texture_get_height(texture_t* texture);

texture_format_t texture_get_format(texture_t* texture);

#endif
