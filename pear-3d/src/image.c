#include <image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PEAR3D_MAGIC_LENGTH 12
#define PEAR3D_VERSION_LENGTH 3
#define PEAR3D_MAGIC "pear3d_image"
#define PEAR3D_VERSION "0.1"
#define PEAR3D_SIZE 's'
#define PEAR3D_CHANNELS 'c'
#define PEAR3D_DATA 'd'

static const char* pear3d_magic = PEAR3D_MAGIC;
static const char* pear3d_version = PEAR3D_VERSION;
static const char pear3d_size = PEAR3D_SIZE;
static const char pear3d_channels = PEAR3D_CHANNELS;
static const char pear3d_data = PEAR3D_DATA;

void pear3d_save_image(const char* filename, Pear3D_Image image) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("pear3d: failed to open file %s!\n", filename);
        return;
    }

    fwrite(pear3d_magic, sizeof(char) * PEAR3D_MAGIC_LENGTH, 1, file);
    fwrite(pear3d_version, sizeof(char) * PEAR3D_VERSION_LENGTH, 1, file);

    // write size
    fwrite(&pear3d_size, sizeof(char), 1, file);
    fwrite(&(image.width), sizeof(uint32_t), 1, file);
    fwrite(&(image.height), sizeof(uint32_t), 1, file);

    // write channels
    fwrite(&pear3d_channels, sizeof(char), 1, file);
    fwrite(&(image.num_channels), sizeof(uint32_t), 1, file);

    // write data
    fwrite(&pear3d_data, sizeof(char), 1, file);
    fwrite(&(image.size), sizeof(uint32_t), 1, file);
    fwrite(image.data, sizeof(uint8_t), image.size, file);

    fclose(file);
}

Pear3D_Image pear3d_load_image(const char* filename, bool* success) {
    Pear3D_Image image;
    *success = true;
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("pear3d: failed to open file %s!\n", filename);
        *success = false;
        return image;
    }

    // read magic
    char magic[PEAR3D_MAGIC_LENGTH + 1];
    fread(magic, sizeof(char) * PEAR3D_MAGIC_LENGTH, 1, file);
    magic[PEAR3D_MAGIC_LENGTH] = '\0';
    if (strcmp(magic, pear3d_magic) != 0) {
        printf("pear3d: file %s is not a pear3d file!\n", filename);
        *success = false;
        return image;
    }

    char version[PEAR3D_VERSION_LENGTH + 1];
    fread(version, sizeof(char) * PEAR3D_VERSION_LENGTH, 1, file);
    version[PEAR3D_VERSION_LENGTH] = '\0';
    if (strcmp(version, pear3d_version) != 0) {
        printf("pear3d: file %s is not the correct version! version %s expected, found version %s\n", filename, pear3d_version, version);
        *success = false;
        return image;
    }

    // read size
    char size_magic;
    fread(&size_magic, sizeof(char), 1, file);
    if (size_magic != PEAR3D_SIZE) {
        printf("pear3d: wrong file layout! expected size (\"%c\"), found \"%c\" instead\n", pear3d_size, size_magic);
        *success = false;
        return image;
    }
    
    fread(&(image.width), sizeof(uint32_t), 1, file);
    fread(&(image.height), sizeof(uint32_t), 1, file);

    // read channels
    char channels_magic;
    fread(&channels_magic, sizeof(char), 1, file);
    if (channels_magic != PEAR3D_CHANNELS) {
        printf("pear3d: wrong file layout! expected channels (\"%c\"), found \"%c\" instead\n", pear3d_channels, channels_magic);
        *success = false;
        return image;
    }
    
    fread(&(image.num_channels), sizeof(uint32_t), 1, file);

    // read data
    char data_magic;
    fread(&data_magic, sizeof(char), 1, file);
    if (data_magic != PEAR3D_DATA) {
        printf("pear3d: wrong file layout! expected size (\"%c\"), found \"%c\" instead\n", pear3d_data, data_magic);
        *success = false;
        return image;
    }

    fread(&(image.size), sizeof(uint32_t), 1, file);
    image.data = (uint8_t*)malloc(sizeof(uint8_t) * image.size);
    fread(image.data, sizeof(uint8_t), image.size, file);

    fclose(file);

    return image;
}

void pear3d_delete_image(Pear3D_Image image) {
    free(image.data);
}
