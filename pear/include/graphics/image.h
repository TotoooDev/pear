#ifndef PEAR_IMAGE_H
#define PEAR_IMAGE_H

#include <core/types.h>

typedef struct Image Image;

Image* image_new(u32 width, u32 height, u32 num_channels, void* data);
void image_delete(Image* image);

u32 image_get_width(Image* image);
u32 image_get_height(Image* image);
u32 image_get_num_channels(Image* image);
void* image_get_data(Image* image);

#endif
