#ifndef PEAR3D_IMAGE_H
#define PEAR3D_IMAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

typedef struct Pear3D_Image {
    uint8_t* data;
    uint32_t size;

    int32_t width;
    int32_t height;
    int32_t num_channels;
} Pear3D_Image;

void pear3d_save_image(const char* filename, Pear3D_Image image);
Pear3D_Image pear3d_load_image(const char* filename, bool* success);
void pear3d_delete_image(Pear3D_Image image);

#ifdef __cplusplus
}
#endif

#endif
