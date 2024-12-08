#ifndef PEAR_MESH_H
#define PEAR_MESH_H

#include <graphics/mesh_info.h>

typedef struct mesh_t mesh_t;

mesh_t* mesh_new(mesh_info_t* mesh_info, u32 material_index);
void mesh_delete(mesh_t* mesh);

u32 mesh_get_material_index(mesh_t* mesh);

#endif
