#ifndef PEAR_TEXTURE_H
#define PEAR_TEXTURE_H

#include <graphics/image.h>

typedef enum TextureFormat {
    TEXTURE_FORMAT_R,
    TEXTURE_FORMAT_RG,
    TEXTURE_FORMAT_RGB,
    TEXTURE_FORMAT_RGBA
} TextureFormat;

typedef enum TextureWrapping {
    TEXTURE_WRAPPING_REPEAT,
    TEXTURE_WRAPPING_REPEAT_MIRROR,
    TEXTURE_WRAPPING_CLAMP,
    TEXTURE_WRAPPING_NONE
} TextureWrapping;

typedef enum TextureFiltering {
    TEXTURE_FILTERING_NEAREST,
    TEXTURE_FILTERING_LINEAR
} TextureFiltering;

typedef struct Texture Texture;

Texture* texture_new(u32 width, u32 height, TextureWrapping wrapping, TextureFiltering filtering, TextureFormat format);
Texture* texture_new_from_image(Image* image, TextureWrapping wrapping, TextureFiltering filtering);
void texture_delete(Texture* texture);

#endif
