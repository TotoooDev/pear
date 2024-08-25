#include <scene/types/mesh_3d.h>
#include <stdlib.h>

typedef struct Mesh3D {
    Mesh* mesh;
    Transform transform;
} Mesh3D;

Mesh3D* mesh3d_new(Mesh3DCreationInfo* creation_info) {
    Mesh3D* mesh3d = (Mesh3D*)malloc(sizeof(Mesh3D));
    
    mesh3d->mesh = creation_info->mesh;
    glm_vec3_copy(creation_info->transform.pos, mesh3d->transform.pos);
    glm_vec3_copy(creation_info->transform.rotation, mesh3d->transform.rotation);
    glm_vec3_copy(creation_info->transform.scale, mesh3d->transform.scale);
    
    return mesh3d;
}

void mesh3d_delete(Mesh3D* mesh) {
    free(mesh);
}

void mesh3d_set_position(Mesh3D* mesh, vec3 pos) {
    glm_vec3_copy(pos, mesh->transform.pos);
}

void mesh3d_set_rotation(Mesh3D* mesh, vec3 rotation) {
    glm_vec3_copy(rotation, mesh->transform.rotation);
}

void mesh3d_set_scale(Mesh3D* mesh, vec3 scale) {
    glm_vec3_copy(scale, mesh->transform.scale);
}

void mesh3d_get_position(Mesh3D* mesh, vec3 dest) {
    glm_vec3_copy(mesh->transform.pos, dest);
}

void mesh3d_get_rotation(Mesh3D* mesh, vec3 dest) {
    glm_vec3_copy(mesh->transform.rotation, dest);
}

void mesh3d_get_scale(Mesh3D* mesh, vec3 dest) {
    glm_vec3_copy(mesh->transform.scale, dest);
}

void mesh3d_get_mesh_matrix(Mesh3D* mesh, mat4 dest) {
    mat4 mesh_matrix;
    glm_mat4_identity(mesh_matrix);

    glm_translate(mesh_matrix, mesh->transform.pos);
    glm_rotate(mesh_matrix, mesh->transform.rotation[0], (vec3){ 1.0f, 0.0f, 0.0f });
    glm_rotate(mesh_matrix, mesh->transform.rotation[1], (vec3){ 0.0f, 1.0f, 0.0f });
    glm_rotate(mesh_matrix, mesh->transform.rotation[2], (vec3){ 0.0f, 0.0f, 1.0f });
    glm_scale(mesh_matrix, mesh->transform.scale);

    glm_mat4_copy(mesh_matrix, dest);
}

Mesh* mesh3d_get_mesh(Mesh3D* mesh) {
    return mesh->mesh;
}
