#include <pear-3d.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define PEAR3D_MAGIC_LENGTH 6
#define PEAR3D_MAGIC "pear3d"
#define PEAR3D_VERTICES 'v'
#define PEAR3D_INDICES 'i'
#define PEAR3D_TEXTURE_COORDS 't'

static const char pear3d_vertices = PEAR3D_VERTICES;
static const char pear3d_indices = PEAR3D_INDICES;
static const char pear3d_texture_coords = PEAR3D_TEXTURE_COORDS;

typedef struct ModelData {
    vec3* vertices;
    unsigned int* indices;
    vec2* texture_coords;

    unsigned int num_vertices;
    unsigned int num_indices;
    unsigned int num_texture_coords;
} ModelData;

ModelData* pear3d_load_model(const char* filename) {
    ModelData* model_data = pear3d_new(NULL, NULL, NULL, 0, 0, 0);

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("pear3D: failed to open file %s!\n", filename);
        return model_data;
    }

    char magic[PEAR3D_MAGIC_LENGTH];
    fread(magic, sizeof(char) * PEAR3D_MAGIC_LENGTH, 1, file);
    if (strcmp(magic, PEAR3D_MAGIC) != 0) {
        printf("pear3d: file %s is not a pear3d file! found \"%s\", expected \"%s\".\n", filename, magic, PEAR3D_MAGIC);
        return model_data;
    }

    // vertices
    char vertices_magic;
    fread(&vertices_magic, sizeof(char), 1, file);
    if (vertices_magic != PEAR3D_VERTICES) {
        printf("pear3d: invalid vertex magic character! found \"%s\", expected \"%s\".\n", vertices_magic, PEAR3D_VERTICES);    
        return model_data;
    }

    fread(&(model_data->num_vertices), sizeof(unsigned int), 1, file);
    float* vertices = (float*)malloc(sizeof(float) * model_data->num_vertices * 3);

    fread(vertices, sizeof(float) * model_data->num_vertices * 3, 1, file);
    model_data->vertices = (vec3*)malloc(sizeof(vec3) * model_data->num_vertices);
    for (unsigned int i = 0; i < model_data->num_vertices; i++) {
        model_data->vertices[i][0] = vertices[3 * i];
        model_data->vertices[i][1] = vertices[3 * i + 1];
        model_data->vertices[i][2] = vertices[3 * i + 2];
    }

    // indices
    char indices_magic;
    fread(&indices_magic, sizeof(char), 1, file);
    if (indices_magic != PEAR3D_INDICES) {
        printf("pear3d: invalid index magic character! found \"%s\", expected \"%s\".\n", indices_magic, PEAR3D_INDICES);    
        return model_data;
    }

    fread(&(model_data->num_indices), sizeof(unsigned int), 1, file);
    model_data->indices = (unsigned int*)malloc(sizeof(unsigned int) * model_data->num_indices);
    fread(model_data->indices, sizeof(unsigned int) * model_data->num_indices, 1, file);

    // texture coords
    char texture_coords_magic;
    fread(&texture_coords_magic, sizeof(char), 1, file);
    if (texture_coords_magic != PEAR3D_TEXTURE_COORDS) {
        printf("pear3d: invalid texture coords magic character! found \"%s\", expected \"%s\".\n", texture_coords_magic, PEAR3D_TEXTURE_COORDS);    
        return model_data;
    }

    fread(&(model_data->num_texture_coords), sizeof(unsigned int), 1, file);
    float* texture_coords = (float*)malloc(sizeof(float) * model_data->num_texture_coords * 2);

    fread(texture_coords, sizeof(float) * model_data->num_texture_coords * 2, 1, file);
    model_data->texture_coords = (vec2*)malloc(sizeof(vec2) * model_data->num_texture_coords);
    for (unsigned int i = 0; i < model_data->num_texture_coords; i++) {
        model_data->texture_coords[i][0] = texture_coords[2 * i];
        model_data->texture_coords[i][1] = texture_coords[2 * i + 1];
    }

    fclose(file);

    return model_data;
}

void pear3d_save_model(const char* filename, ModelData* model_data) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("pear3d: failed to open file %s!\n", filename);
        return;
    }

    fwrite(PEAR3D_MAGIC, sizeof(char) * PEAR3D_MAGIC_LENGTH, 1, file);

    fwrite(&pear3d_vertices, sizeof(char), 1, file);
    fwrite(&(model_data->num_vertices), sizeof(unsigned int), 1, file);
    fwrite(model_data->vertices, sizeof(float) * 3, model_data->num_vertices, file);

    fwrite(&pear3d_indices, sizeof(char), 1, file);
    fwrite(&(model_data->num_indices), sizeof(unsigned int), 1, file);
    fwrite(model_data->indices, sizeof(unsigned int), model_data->num_indices, file);

    fwrite(&pear3d_texture_coords, sizeof(char), 1, file);
    fwrite(&(model_data->num_texture_coords), sizeof(unsigned int), 1, file);
    fwrite(model_data->texture_coords, sizeof(float) * 2, model_data->num_texture_coords, file);

    fclose(file);
}

ModelData* pear3d_new(vec3* vertices, unsigned int* indices, vec2* texture_coords, unsigned int num_vertices, unsigned int num_indices, unsigned int num_texture_coords) {
    ModelData* model_data = (ModelData*)malloc(sizeof(ModelData));
    
    model_data->num_vertices = num_vertices;
    model_data->num_indices = num_indices;
    model_data->num_texture_coords = num_texture_coords;
    model_data->vertices = vertices;
    model_data->indices = indices;
    model_data->texture_coords = texture_coords;
    
    return model_data;
}

void pear3d_delete(ModelData* model_data) {
    free(model_data->vertices);
    free(model_data->texture_coords);
    free(model_data->indices);
    free(model_data);
}

vec3* pear3d_get_vertices(ModelData* model_data) {
    return model_data->vertices;
}

unsigned int* pear3d_get_indices(ModelData* model_data) {
    return model_data->indices;
}

vec2* pear3d_get_texture_coords(ModelData* model_data) {
    return model_data->texture_coords;
}

unsigned int pear3d_get_num_vertices(ModelData* model_data) {
    return model_data->num_vertices;
}

unsigned int pear3d_get_num_indices(ModelData* model_data) {
    return model_data->num_indices;
}

unsigned int pear3d_get_num_texture_coords(ModelData* model_data) {
    return model_data->num_texture_coords;
}
