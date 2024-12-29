#include <graphics/image.h>
#include <core/log.h>
#include <core/alloc.h>

typedef struct image_t {
    u32 width;
    u32 height;
    u32 num_channels;
    u32 data_size;
    void* data;
} image_t;
image_t* image_new(u32 width, u32 height, u32 num_channels, void* data) {
    image_t* image = (image_t*)PEAR_MALLOC(sizeof(image_t));

    image->width = width;
    image->height = height;
    image->num_channels = num_channels;
    image->data_size = width * height * num_channels;
    image->data = data;

    return image;
}

void image_delete(image_t* image) {
    free(image->data);
    free(image);
}

u32 image_get_width(image_t* image) {
    return image->width;
}

u32 image_get_height(image_t* image) {
    return image->height;
}

u32 image_get_num_channels(image_t* image) {
    return image->num_channels;
}

void* image_get_data(image_t* image) {
    return image->data;
}

void image_set_data(image_t* image, void* data) {
    free(image->data);
    image->data = data;
}
