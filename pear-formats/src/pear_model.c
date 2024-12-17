#include <pear_model.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define PEAR_MODEL_ID_STRING "pear_model1.0.1"

static const char* pear_model_id_string = PEAR_MODEL_ID_STRING;
static const uint32_t pear_model_id_string_length = 16;

pear_vertex_t pear_model_read_vertex(FILE* file) {
    pear_vertex_t vertex;
    fread(vertex.pos, sizeof(float), 3, file);
    fread(vertex.texture_coords, sizeof(float), 2, file);
    fread(vertex.normal, sizeof(float), 3, file);
    return vertex;
}

pear_mesh_t pear_model_read_mesh(FILE* file) {
    pear_mesh_t mesh;

    fread(&mesh.num_vertices, sizeof(uint32_t), 1, file);
    fread(&mesh.num_indices, sizeof(uint32_t), 1, file);
    fread(&mesh.material_index, sizeof(uint32_t), 1, file);

    mesh.vertices = (pear_vertex_t*)malloc(sizeof(pear_vertex_t) * mesh.num_vertices);
    for (uint32_t i = 0; i < mesh.num_vertices; i++) {
        mesh.vertices[i] = pear_model_read_vertex(file);
    }

    mesh.indices = (uint32_t*)malloc(sizeof(uint32_t) * mesh.num_indices);
    fread(mesh.indices, sizeof(uint32_t), mesh.num_indices, file);

    return mesh;
}

pear_material_t pear_model_read_material(FILE* file) {
    pear_material_t material;
    uint32_t diffuse_length;
    uint32_t specular_length;
    uint32_t normal_length;

    fread(&diffuse_length, sizeof(uint32_t), 1, file);
    fread(&specular_length, sizeof(uint32_t), 1, file);
    fread(&normal_length, sizeof(uint32_t), 1, file);

    material.diffuse_path = (char*)malloc(sizeof(char) * (diffuse_length + 1));
    material.specular_path = (char*)malloc(sizeof(char) * (specular_length + 1));
    material.normal_path = (char*)malloc(sizeof(char) * (normal_length + 1));

    fread(material.diffuse_path, sizeof(char), diffuse_length, file);
    fread(material.specular_path, sizeof(char), specular_length, file);
    fread(material.normal_path, sizeof(char), normal_length, file);

    material.diffuse_path[diffuse_length] = '\0';
    material.specular_path[specular_length] = '\0';
    material.normal_path[normal_length] = '\0';

    fread(material.color, sizeof(float), 3, file);
    fread(&material.shininess, sizeof(float), 1, file);

    return material;
}

pear_model_t pear_model_load(const char* filename, bool* success) {
    pear_model_t model;
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("failed to open file %s!\n", filename);
        *success = false;
        return model;
    }

    char* id_string = (char*)malloc(sizeof(char) * pear_model_id_string_length);
    fread(id_string, sizeof(char), pear_model_id_string_length, file);
    id_string[pear_model_id_string_length - 1] = '\0';
    if (strcmp(id_string, pear_model_id_string) != 0) {
        printf("invalid pear_model file! expected id string %s, got %s", pear_model_id_string, id_string);
        *success = false;
        free(id_string);
        return model;
    }
    free(id_string);

    fread(&model.num_meshes, sizeof(uint32_t), 1, file);
    fread(&model.num_materials, sizeof(uint32_t), 1, file);

    model.meshes = (pear_mesh_t*)malloc(sizeof(pear_mesh_t) * model.num_meshes);
    for (uint32_t i = 0; i < model.num_meshes; i++) {
        model.meshes[i] = pear_model_read_mesh(file);
    }

    model.materials = (pear_material_t*)malloc(sizeof(pear_material_t) * model.num_materials);
    for (uint32_t i = 0; i < model.num_materials; i++) {
        model.materials[i] = pear_model_read_material(file);
    }

    *success = true;
    return model;
}

void pear_model_write_vertex(pear_vertex_t vertex, FILE* file) {
    fwrite(vertex.pos, sizeof(float), 3, file);
    fwrite(vertex.texture_coords, sizeof(float), 2, file);
    fwrite(vertex.normal, sizeof(float), 3, file);
}

void pear_model_write_mesh(pear_mesh_t mesh, FILE* file) {
    fwrite(&mesh.num_vertices, sizeof(uint32_t), 1, file);
    fwrite(&mesh.num_indices, sizeof(uint32_t), 1, file);
    fwrite(&mesh.material_index, sizeof(uint32_t), 1, file);

    for (uint32_t i = 0; i < mesh.num_vertices; i++) {
        pear_model_write_vertex(mesh.vertices[i], file);
    }

    fwrite(mesh.indices, sizeof(uint32_t), mesh.num_indices, file);
}

void pear_model_write_material(pear_material_t material, FILE* file) {
    uint32_t diffuse_length = strlen(material.diffuse_path);
    uint32_t specular_length = strlen(material.specular_path);
    uint32_t normal_length = strlen(material.normal_path);

    fwrite(&diffuse_length, sizeof(uint32_t), 1, file);
    fwrite(&specular_length, sizeof(uint32_t), 1, file);
    fwrite(&normal_length, sizeof(uint32_t), 1, file);

    fwrite(material.diffuse_path, sizeof(char), diffuse_length, file);
    fwrite(material.specular_path, sizeof(char), specular_length, file);
    fwrite(material.normal_path, sizeof(char), normal_length, file);

    fwrite(material.color, sizeof(float), 3, file);
    fwrite(&material.shininess, sizeof(float), 1, file);
}

void pear_model_write(pear_model_t model, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("failed to open file %s!\n", filename);
        return;
    }

    fwrite(pear_model_id_string, sizeof(char), pear_model_id_string_length, file);
    fwrite(&model.num_meshes, sizeof(uint32_t), 1, file);
    fwrite(&model.num_materials, sizeof(uint32_t), 1, file);

    for (uint32_t i = 0; i < model.num_meshes; i++) {
        pear_model_write_mesh(model.meshes[i], file);
    }

    for (uint32_t i = 0; i < model.num_materials; i++) {
        pear_model_write_material(model.materials[i], file);
    }

    fclose(file);
}

void pear_model_free(pear_model_t* model) {
    for (uint32_t i = 0; i < model->num_meshes; i++) {
        pear_mesh_t mesh = model->meshes[i];
        free(mesh.vertices);
        free(mesh.indices);
    }

    for (uint32_t i = 0; i < model->num_materials; i++) {
        pear_material_t material = model->materials[i];
        free(material.diffuse_path);
        free(material.specular_path);
        free(material.normal_path);
    }

    free(model->meshes);
    free(model->materials);

    model->num_meshes = 0;
    model->num_materials = 0;
}
