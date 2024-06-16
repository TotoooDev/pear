#include <scene/types/mesh_3d.h>
#include <stdlib.h>

typedef struct Mesh3D {
    Mesh* mesh;
    vec3 pos;
    vec3 rotation;
    vec3 scale;
} Mesh3D;

Mesh3D* mesh3d_new(Mesh3DCreationInfo* creation_info) {
    Mesh3D* mesh3d = (Mesh3D*)malloc(sizeof(Mesh3D));
    
    mesh3d->mesh = creation_info->mesh;
    glm_vec3_copy(creation_info->pos, mesh3d->pos);
    glm_vec3_copy(creation_info->rotation, mesh3d->rotation);
    glm_vec3_copy(creation_info->scale, mesh3d->scale);
    
    return mesh3d;
}

void mesh3d_delete(Mesh3D* mesh) {
    free(mesh);
}

void mesh3d_set_position(Mesh3D* mesh, vec3 pos) {
    glm_vec3_copy(pos, mesh->pos);
}

void mesh3d_set_rotation(Mesh3D* mesh, vec3 rotation) {
    glm_vec3_copy(rotation, mesh->rotation);
}

void mesh3d_set_scale(Mesh3D* mesh, vec3 scale) {
    glm_vec3_copy(scale, mesh->scale);
}

void mesh3d_get_position(Mesh3D* mesh, vec3 dest) {
    glm_vec3_copy(mesh->pos, dest);
}

void mesh3d_get_rotation(Mesh3D* mesh, vec3 dest) {
    glm_vec3_copy(mesh->rotation, dest);
}

void mesh3d_get_scale(Mesh3D* mesh, vec3 dest) {
    glm_vec3_copy(mesh->scale, dest);
}

Mesh* mesh3d_get_mesh(Mesh3D* mesh) {
    return mesh->mesh;
}
