#ifndef PEAR_IMAGE_H
#define PEAR_IMAGE_H

#include <core/types.h>

typedef struct Image Image;

Image* image_new_from_file(const char* path);
void image_delete(Image* image);

u32 image_get_width(Image* image);
u32 image_get_height(Image* image);
u32 image_get_num_channels(Image* image);
void* image_get_data(Image* image);

#endif
