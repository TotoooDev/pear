#ifndef PEAR_FORMATS_IMAGE_H_
#define PEAR_FORMATS_IMAGE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

typedef struct pear_image_t {
    uint32_t width;
    uint32_t height;
    uint32_t num_channels;

    uint8_t* data;
    uint32_t data_size;
} pear_image_t;

pear_image_t pear_image_load(const char* filename, bool* success);
void pear_image_write(pear_image_t image, const char* filename);

#ifdef __cplusplus
}
#endif

#endif
