#ifndef PEAR_MESH_H
#define PEAR_MESH_H

#include <graphics/mesh_info.h>
#include <core/types.h>
#include <pear-3d.h>

typedef struct Mesh Mesh;

Mesh* mesh_new(MeshInfo* mesh_info, u32 material_index, u32* indices, u32 num_indices);
void mesh_delete(Mesh* mesh);

u32 mesh_get_material_index(Mesh* mesh);
u32 mesh_get_num_indices(Mesh* mesh);

#endif
