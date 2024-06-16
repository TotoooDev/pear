#ifndef PEER_NODE_MESH3D_H
#define PEER_NODE_MESH3D_H

#include <graphics/mesh.h>
#include <cglm/cglm.h>

typedef struct Mesh3D Mesh3D;

Mesh3D* mesh3d_new(Mesh* mesh, vec3 pos, vec3 rotation, vec3 scale);
void mesh3d_delete(Mesh3D* mesh);

void mesh3d_set_position(Mesh3D* mesh, vec3 pos);
void mesh3d_set_rotation(Mesh3D* mesh, vec3 rotation);
void mesh3d_set_scale(Mesh3D* mesh, vec3 scale);

void mesh3d_get_position(Mesh3D* mesh, vec3 dest);
void mesh3d_get_rotation(Mesh3D* mesh, vec3 dest);
void mesh3d_get_scale(Mesh3D* mesh, vec3 dest);
Mesh* mesh3d_get_mesh(Mesh3D* mesh);

#endif
