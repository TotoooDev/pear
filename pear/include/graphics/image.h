#ifndef PEAR_GRAPHICS_IMAGE_H_
#define PEAR_GRAPHICS_IMAGE_H_

#include <core/types.h>

typedef struct image_t image_t;

image_t* image_new(u32 num_components, void* data, u32 data_size);
image_t* image_new_size(u32 width, u32 height, u32 num_channels, void* data);
void image_delete(image_t* image);

u32 image_get_width(image_t* image);
u32 image_get_height(image_t* image);
u32 image_get_num_channels(image_t* image);
void* image_get_data(image_t* image);

#endif
