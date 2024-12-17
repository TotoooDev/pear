#ifndef PEAR_FORMATS_MODEL_H
#define PEAR_FORMATS_MODEL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <cglm/cglm.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct pear_vertex_t {
    vec3 pos;
    vec2 texture_coords;
    vec3 normal;
} pear_vertex_t;

typedef struct pear_mesh_t {
    pear_vertex_t* vertices;
    uint32_t* indices;

    uint32_t num_vertices;
    uint32_t num_indices;

    uint32_t material_index;
} pear_mesh_t;

typedef struct pear_material_t {
    vec4 color;
    float shininess;

    char* diffuse_path;
    char* specular_path;
    char* normal_path;
} pear_material_t;

typedef struct pear_model_t {
    pear_mesh_t* meshes;
    pear_material_t* materials;

    uint32_t num_meshes;
    uint32_t num_materials;
} pear_model_t;

pear_model_t pear_model_load(const char* filename, bool* success);
void pear_model_write(pear_model_t model, const char* filename);
void pear_model_free(pear_model_t* model);

#ifdef __cplusplus
}
#endif

#endif
