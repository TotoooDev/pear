#include <loaders/image.h>
#include <core/log.h>

#define STB_IMAGE_IMPLEMENTATION
#include <loaders/vendor/stb_image.h>

image_t* loader_load_image(const char* filename, bool flip_vertically) {
    i32 width;
    i32 height;
    i32 num_channels;
    
    stbi_set_flip_vertically_on_load(flip_vertically);
    u8* data = stbi_load(filename, &width, &height, &num_channels, 0);
    if (data == NULL) {
        PEAR_ERROR("failed to load image %s!", filename);
    }

    return image_new(width, height, num_channels, data);
}

image_t* loader_load_image_data(u8* data, u32 data_size, bool flip_vertically) {
    i32 width;
    i32 height;
    i32 num_channels;

    stbi_set_flip_vertically_on_load(flip_vertically);
    u8* image_data = stbi_load_from_memory(data, data_size, &width, &height, &num_channels, 0);
    if (image_data == NULL) {
        PEAR_ERROR("failed to load image data at location %p!", data);
        return NULL;
    }

    return image_new(width, height, num_channels, image_data);
}
