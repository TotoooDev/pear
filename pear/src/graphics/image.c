#include <graphics/image.h>
#include <core/log.h>
#include <stdlib.h>

typedef struct Image {
    u32 width;
    u32 height;
    u32 num_channels;
    void* data;
} Image;

Image* image_new(u32 width, u32 height, u32 num_channels, void* data) {
    Image* image = (Image*)malloc(sizeof(Image));

    image->width = width;
    image->height = height;
    image->num_channels = num_channels;
    image->data = data;

    return image;
}

void image_delete(Image* image) {
    if (image->data != NULL)
        free(image->data);

    free(image);
}

u32 image_get_width(Image* image) {
    return image->width;
}

u32 image_get_height(Image* image) {
    return image->height;
}

u32 image_get_num_channels(Image* image) {
    return image->num_channels;
}

void* image_get_data(Image* image) {
    return image->data;
}

void image_set_data(Image* image, void* data) {
    free(image->data);
    image->data = data;
}
