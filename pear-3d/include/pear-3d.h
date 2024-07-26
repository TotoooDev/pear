#ifndef PEAR3D_H
#define PEAR3D_H

#ifdef __cplusplus
extern "C" {
#endif

#include <cglm/cglm.h>
#include <stdbool.h>

typedef struct Pear3D_Vertex {
    vec3 position;
    vec2 texture_coords;
    vec3 normal;
} Pear3D_Vertex;

typedef struct Pear3D_Material {
    const char* texture_diffuse;
    const char* texture_specular;
    const char* texture_normal;

    vec4 color;
} Pear3D_Material;

typedef struct Pear3D_Mesh {
    Pear3D_Vertex* vertices;
    uint32_t* indices;
    uint32_t material_index;

    uint32_t num_vertices;
    uint32_t num_indices;
} Pear3D_Mesh;

typedef struct Pear3D {
    Pear3D_Material* materials;
    Pear3D_Mesh* meshes;

    uint32_t num_materials;
    uint32_t num_meshes;
} Pear3D;

void pear3d_save(const char* filename, Pear3D model);
Pear3D pear3d_load(const char* filename, bool* success);
void pear3d_delete(Pear3D model);

#ifdef __cplusplus
}
#endif

#endif
