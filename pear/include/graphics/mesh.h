#ifndef PEAR_MESH_H
#define PEAR_MESH_H

#include <core/types.h>

typedef struct Mesh Mesh;

/**
 * TEMPORARY: the vertex data must be arranged this way: vec3 position / vec3 color.
 * This is going to change in the future to support a more robust vertex data system.
 */
Mesh* mesh_new(f32* vertices, u32* indices, u32 num_vertices, u32 num_indices);
void mesh_delete(Mesh* mesh);

u32 mesh_get_num_vertices(Mesh* mesh);
u32 mesh_get_num_indices(Mesh* mesh);

#endif
