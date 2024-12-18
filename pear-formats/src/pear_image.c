#include <pear_image.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define PEAR_IMAGE_ID_STRING "pear_image1.0"

static const char* pear_image_id_string = PEAR_IMAGE_ID_STRING;
static const uint32_t pear_image_id_string_length = 14;

pear_image_t pear_image_load(const char* filename, bool* success) {
    pear_image_t image;
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("failed to open file %s!\n", filename);
        *success = false;
        return image;
    }
    

    char* id_string = (char*)malloc(sizeof(char) * pear_image_id_string_length);
    fread(id_string, sizeof(char), pear_image_id_string_length - 1, file);
    id_string[pear_image_id_string_length - 1] = '\0';
    if (strcmp(id_string, pear_image_id_string) != 0) {
        free(id_string);
        printf("invalid pear_image file! expected id string %s, got %s", pear_image_id_string, id_string);
        *success = false;
        return image;
    }
    free(id_string);

    fread(&image.width, sizeof(uint32_t), 1, file);
    fread(&image.height, sizeof(uint32_t), 1, file);
    fread(&image.num_channels, sizeof(uint32_t), 1, file);
    fread(&image.data_size, sizeof(uint32_t), 1, file);

    image.data = (uint8_t*)malloc(sizeof(uint8_t) * image.data_size);
    fread(image.data, sizeof(uint8_t), image.data_size, file);

    *success = true;
    return image;
}

void pear_image_write(pear_image_t image, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("failed to open file %s!\n", filename);
        return;
    }

    fwrite(pear_image_id_string, sizeof(char), strlen(pear_image_id_string), file);
    fwrite(&image.width, sizeof(uint32_t), 1, file);
    fwrite(&image.height, sizeof(uint32_t), 1, file);
    fwrite(&image.num_channels, sizeof(uint32_t), 1, file);
    fwrite(&image.data_size, sizeof(uint32_t), 1, file);
    fwrite(image.data, sizeof(uint8_t), image.data_size, file);

    fclose(file);
}
