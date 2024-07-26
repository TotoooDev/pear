#ifndef PEAR_MESH_H
#define PEAR_MESH_H

#include <graphics/mesh_info.h>
#include <graphics/material.h>
#include <core/types.h>
#include <pear-3d.h>

typedef struct Mesh Mesh;

Mesh* mesh_new(MeshInfo* mesh_info, Material material, u32* indices, u32 num_indices);
void mesh_delete(Mesh* mesh);

Material* mesh_get_material(Mesh* mesh);
u32 mesh_get_num_indices(Mesh* mesh);

#endif
