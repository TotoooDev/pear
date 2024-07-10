#include <pear-3d.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PEAR3D_MAGIC_LENGTH 6
#define PEAR3D_VERSION_LENGTH 3
#define PEAR3D_MAGIC "pear3d"
#define PEAR3D_VERSION "0.1"
#define PEAR3D_VERTICES 'v'
#define PEAR3D_MESHES 'm'
#define PEAR3D_MATERIALS 'a'

static const char* pear3d_magic = PEAR3D_MAGIC;
static const char* pear3d_version = PEAR3D_VERSION;
static const char pear3d_vertices = PEAR3D_VERTICES;
static const char pear3d_meshes = PEAR3D_MESHES;
static const char pear3d_materials = PEAR3D_MATERIALS;

void pear3d_save(const char* filename, Pear3D model) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("pear3d: failed to open file %s!\n", filename);
        return;
    }

    fwrite(pear3d_magic, sizeof(char) * PEAR3D_MAGIC_LENGTH, 1, file);
    fwrite(pear3d_version, sizeof(char) * PEAR3D_VERSION_LENGTH, 1, file);

    // write vertices
    fwrite(&pear3d_vertices, sizeof(char), 1, file);
    fwrite(&(model.num_vertices), sizeof(unsigned int), 1, file);
    for (unsigned int i = 0; i < model.num_vertices; i++) {
        fwrite(model.vertices[i].position, sizeof(float), 3, file);
        fwrite(model.vertices[i].texture_coords, sizeof(float), 2, file);
        fwrite(model.vertices[i].normal, sizeof(float), 3, file);
    }

    // write materials
    fwrite(&pear3d_materials, sizeof(char), 1, file);
    fwrite(&(model.num_materials), sizeof(unsigned int), 1, file);
    for (unsigned int i = 0; i < model.num_materials; i++) {
        char* diffuse = model.materials[i].texture_diffuse;
        char* specular = model.materials[i].texture_specular;
        char* normal = model.materials[i].texture_normal;

        unsigned int diffuse_length = strlen(diffuse);
        unsigned int specular_length = strlen(specular);
        unsigned int normal_length = strlen(normal);

        fwrite(&diffuse_length, sizeof(unsigned int), 1, file);
        fwrite(diffuse, sizeof(char) * diffuse_length, 1, file);

        fwrite(&specular_length, sizeof(unsigned int), 1, file);
        fwrite(specular, sizeof(char) * specular_length, 1, file);

        fwrite(&normal_length, sizeof(unsigned int), 1, file);
        fwrite(normal, sizeof(char) * normal_length, 1, file);
    }

    // write meshes
    fwrite(&pear3d_meshes, sizeof(char), 1, file);
    fwrite(&(model.num_meshes), sizeof(unsigned int), 1, file);
    for (unsigned int i = 0; i < model.num_meshes; i++) {
        fwrite(&(model.meshes[i].num_faces), sizeof(unsigned int), 1, file);
        fwrite(&(model.meshes[i].material_index), sizeof(unsigned int), 1, file);
        for (unsigned int j = 0; j < model.meshes[i].num_faces; j++) {
            fwrite(model.meshes[i].faces[j].vertex_indices, sizeof(unsigned int), 3, file);
        }
    }

    fclose(file);
}

Pear3D pear3d_load(const char* filename, bool* success)  {
    Pear3D model;
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("pear3d: failed to open file %s!", filename);
        *success = false;
    }

    char magic[PEAR3D_MAGIC_LENGTH];
    fread(magic, sizeof(char) * PEAR3D_MAGIC_LENGTH, 1, file);
    if (strcmp(magic, pear3d_magic) != 0) {
        printf("pear3d: file %s is not a pear3d file!", filename);
        *success = false;
    }

    char version[PEAR3D_VERSION_LENGTH];
    fread(version, sizeof(char) * PEAR3D_VERSION_LENGTH, 1, file);
    if (strcmp(version, pear3d_version) != 0) {
        printf("pear3d: file %s is not the correct version! version %s expected, found version %s", filename, pear3d_version, version);
        *success = false;
    }

    fclose(file);

    return model;
}

void pear3d_delete(Pear3D model)  {
    for (unsigned int i = 0; i < model.num_meshes; i++) {
        free(model.meshes[i].faces);
    }

    free(model.vertices);
    free(model.materials);
    free(model.meshes);
}
