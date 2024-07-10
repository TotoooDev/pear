#ifndef PEAR3D_H
#define PEAR3D_H

#ifdef __cplusplus
extern "C" {
#endif

#include <cglm/cglm.h>
#include <stdbool.h>

typedef struct Vertex {
    vec3 position;
    vec2 texture_coords;
    vec3 normal;
} Vertex;

typedef struct Material {
    char* texture_diffuse;
    char* texture_specular;
    char* texture_normal;

    vec4 color;
} Material;

typedef struct Face {
    unsigned int vertex_indices[3];
} Face;

typedef struct Mesh {
    Face* faces;
    unsigned int num_faces;
    unsigned int material_index;
} Mesh;

typedef struct Pear3D {
    Vertex* vertices;
    Material* materials;
    Mesh* meshes;

    unsigned int num_vertices;
    unsigned int num_materials;
    unsigned int num_meshes;
} Pear3D;

void pear3d_save(const char* filename, Pear3D model);
Pear3D pear3d_load(const char* filename, bool* success);
void pear3d_delete(Pear3D model);

#ifdef __cplusplus
}
#endif

#endif
