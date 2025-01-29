#ifndef PEAR_LOADERS_IMAGE_H_
#define PEAR_LOADERS_IMAGE_H_

#include <graphics/image.h>

image_t* loader_load_image(const char* filename, bool flip_vertically);
image_t* loader_load_image_data(u8* data, u32 data_size, bool flip_vertically);

#endif
