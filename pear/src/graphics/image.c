#include <graphics/image.h>
#include <core/log.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include <graphics/vendor/stb_image.h>

typedef struct Image {
    u32 width;
    u32 height;
    u32 num_channels;
    void* data;
} Image;

Image* image_new_from_file(const char* path) {
    Image* image = (Image*)malloc(sizeof(Image));

    image->data = stbi_load(path, &(image->width), &(image->height), &(image->num_channels), 0);
    if (image->data == NULL)
        PEAR_ERROR("failed to load image %s!", path);

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
