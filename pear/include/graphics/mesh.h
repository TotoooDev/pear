#ifndef PEAR_MESH_H
#define PEAR_MESH_H

#include <graphics/mesh_info.h>
#include <core/types.h>

typedef struct Mesh Mesh;

Mesh* mesh_new(MeshInfo* mesh_info, f32* vertices, u32* indices, u32 num_vertices, u32 num_indices);
void mesh_delete(Mesh* mesh);

u32 mesh_get_num_vertices(Mesh* mesh);
u32 mesh_get_num_indices(Mesh* mesh);

#endif
