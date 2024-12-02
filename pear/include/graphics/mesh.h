#ifndef PEAR_MESH_H
#define PEAR_MESH_H

#include <graphics/mesh_info.h>

typedef struct mesh_t mesh_t;

mesh_t* mesh_new(mesh_info_t* mesh_info);
void mesh_delete(mesh_t* mesh);

#endif
