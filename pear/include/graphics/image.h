#ifndef PEAR_GRAPHICS_IMAGE_H_
#define PEAR_GRAPHICS_IMAGE_H_

#include <pear_image.h>
#include <core/types.h>

typedef struct image_t image_t;

image_t* image_new(u32 width, u32 height, u32 num_channels, void* data);
image_t* image_new_from_pear_image(pear_image_t pear_image);
void image_delete(image_t* image);

u32 image_get_width(image_t* image);
u32 image_get_height(image_t* image);
u32 image_get_num_channels(image_t* image);
void* image_get_data(image_t* image);

#endif
