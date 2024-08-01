#include <pear-3d.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

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

    // write materials
    fwrite(&pear3d_materials, sizeof(char), 1, file);
    fwrite(&(model.num_materials), sizeof(uint32_t), 1, file);
    for (uint32_t i = 0; i < model.num_materials; i++) {
        const char* diffuse = model.materials[i].texture_diffuse;
        const char* specular = model.materials[i].texture_specular;
        const char* normal = model.materials[i].texture_normal;

        uint32_t diffuse_length = strlen(diffuse);
        uint32_t specular_length = strlen(specular);
        uint32_t normal_length = strlen(normal);

        fwrite(&diffuse_length, sizeof(uint32_t), 1, file);
        fwrite(diffuse, sizeof(char) * diffuse_length, 1, file);

        fwrite(&specular_length, sizeof(uint32_t), 1, file);
        fwrite(specular, sizeof(char) * specular_length, 1, file);

        fwrite(&normal_length, sizeof(uint32_t), 1, file);
        fwrite(normal, sizeof(char) * normal_length, 1, file);

        fwrite(model.materials[i].color, sizeof(float), 4, file);
    }

    // write meshes
    fwrite(&pear3d_meshes, sizeof(char), 1, file);
    fwrite(&(model.num_meshes), sizeof(uint32_t), 1, file);
    for (uint32_t i = 0; i < model.num_meshes; i++) {
        fwrite(&(model.meshes[i].material_index), sizeof(uint32_t), 1, file);

        fwrite(&(model.meshes[i].num_vertices), sizeof(uint32_t), 1, file);
        for (uint32_t j = 0; j < model.meshes[i].num_vertices; j++) {
            fwrite(model.meshes[i].vertices[j].position, sizeof(float), 3, file);
            fwrite(model.meshes[i].vertices[j].texture_coords, sizeof(float), 2, file);
            fwrite(model.meshes[i].vertices[j].normal, sizeof(float), 3, file);
        }

        fwrite(&(model.meshes[i].num_indices), sizeof(uint32_t), 1, file);
        fwrite(model.meshes[i].indices, sizeof(uint32_t), model.meshes[i].num_indices, file);
    }

    fclose(file);
}

Pear3D pear3d_load(const char* filename, bool* success)  {
    Pear3D model;
    *success = true;
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("pear3d: failed to open file %s!\n", filename);
        *success = false;
        return model;
    }

    // read magic
    char magic[PEAR3D_MAGIC_LENGTH + 1];
    fread(magic, sizeof(char) * PEAR3D_MAGIC_LENGTH, 1, file);
    magic[PEAR3D_MAGIC_LENGTH] = '\0';
    if (strcmp(magic, pear3d_magic) != 0) {
        printf("pear3d: file %s is not a pear3d file!\n", filename);
        *success = false;
        return model;
    }

    char version[PEAR3D_VERSION_LENGTH + 1];
    fread(version, sizeof(char) * PEAR3D_VERSION_LENGTH, 1, file);
    version[PEAR3D_VERSION_LENGTH] = '\0';
    if (strcmp(version, pear3d_version) != 0) {
        printf("pear3d: file %s is not the correct version! version %s expected, found version %s\n", filename, pear3d_version, version);
        *success = false;
        return model;
    }

    // read materials
    char materials_magic;
    fread(&materials_magic, sizeof(char), 1, file);
    if (materials_magic != PEAR3D_MATERIALS) {
        printf("pear3d: wrong file layout! expected materials (\"%c\"), found \"%c\" instead\n", pear3d_materials, materials_magic);
        *success = false;
        return model;
    }

    fread(&(model.num_materials), sizeof(uint32_t), 1, file);
    model.materials = (Pear3D_Material*)malloc(sizeof(Pear3D_Material) * model.num_materials);
    for (uint32_t i = 0; i < model.num_materials; i++) {
        uint32_t diffuse_length;
        uint32_t specular_length;
        uint32_t normal_length;

        fread(&diffuse_length, sizeof(uint32_t), 1, file);
        model.materials[i].texture_diffuse = (char*)malloc(sizeof(char*) * diffuse_length);
        fread((void*)model.materials[i].texture_diffuse, sizeof(char) * diffuse_length, 1, file);

        fread(&specular_length, sizeof(uint32_t), 1, file);
        model.materials[i].texture_specular = (char*)malloc(sizeof(char*) * specular_length);
        fread((void*)model.materials[i].texture_specular, sizeof(char) * specular_length, 1, file);

        fread(&normal_length, sizeof(uint32_t), 1, file);
        model.materials[i].texture_normal = (char*)malloc(sizeof(char*) * normal_length);
        fread((void*)model.materials[i].texture_normal, sizeof(char) * normal_length, 1, file);

        fread(&(model.materials[i].color), sizeof(float), 4, file);
    }

    // read meshes
    char meshes_magic;
    fread(&meshes_magic, sizeof(char), 1, file);
    if (meshes_magic != PEAR3D_MESHES) {
        printf("pear3d: wrong file layout! expected meshes (\"%c\"), found \"%c\" instead\n", pear3d_meshes, meshes_magic);
        *success = false;
        return model;
    }

    fread(&(model.num_meshes), sizeof(uint32_t), 1, file);
    model.meshes = (Pear3D_Mesh*)malloc(sizeof(Pear3D_Mesh) * model.num_meshes);
    for (uint32_t i = 0; i < model.num_meshes; i++) {
        fread(&(model.meshes[i].material_index), sizeof(uint32_t), 1, file);

        fread(&(model.meshes[i].num_vertices), sizeof(uint32_t), 1, file);
        model.meshes[i].vertices = (Pear3D_Vertex*)malloc(sizeof(Pear3D_Vertex) * model.meshes[i].num_vertices);
        for (uint32_t j = 0; j < model.meshes[i].num_vertices; j++) {
            fread(model.meshes[i].vertices[j].position, sizeof(float), 3, file);
            fread(model.meshes[i].vertices[j].texture_coords, sizeof(float), 2, file);
            fread(model.meshes[i].vertices[j].normal, sizeof(float), 3, file);
        }

        fread(&(model.meshes[i].num_indices), sizeof(uint32_t), 1, file);
        model.meshes[i].indices = (uint32_t*)malloc(sizeof(uint32_t) * model.meshes[i].num_indices);
        fread(model.meshes[i].indices, sizeof(uint32_t), model.meshes[i].num_indices, file);
    }

    fclose(file);

    return model;
}

void pear3d_delete(Pear3D model)  {
    for (uint32_t i = 0; i < model.num_meshes; i++) {
        free(model.meshes[i].vertices);
        free(model.meshes[i].indices);
    }

    free(model.materials);
    free(model.meshes);
}
