#include <fs/loaders/image_loader.h>
#include <fs/fs.h>
#include <core/log.h>

#define STB_IMAGE_IMPLEMENTATION
#include <fs/loaders/vendor/stb_image.h>

Image* imageloader_from_file(const char* filename) {
    stbi_set_flip_vertically_on_load(true);

    char path[1024];
    fs_get_path(filename, path);

    u32 width;
    u32 height;
    u32 num_channels;
    void* data = stbi_load(path, &width, &height, &num_channels, 0);

    if (data == NULL) {
        PEAR_ERROR("failed to load image %s!", path);
        return NULL;
    }

    return image_new(width, height, num_channels, data);
}
