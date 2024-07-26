#include <scene/types/mesh_3d.h>
#include <stdlib.h>

typedef struct Model3D {
    Model* model;
    vec3 pos;
    vec3 rotation;
    vec3 scale;
} Model3D;

Model3D* model3d_new(Model3DCreationInfo* creation_info) {
    Model3D* mesh3d = (Model3D*)malloc(sizeof(Model3D));
    
    mesh3d->model = creation_info->model;
    glm_vec3_copy(creation_info->pos, mesh3d->pos);
    glm_vec3_copy(creation_info->rotation, mesh3d->rotation);
    glm_vec3_copy(creation_info->scale, mesh3d->scale);
    
    return mesh3d;
}

void model3d_delete(Model3D* mesh) {
    free(mesh);
}

void model3d_set_position(Model3D* mesh, vec3 pos) {
    glm_vec3_copy(pos, mesh->pos);
}

void model3d_set_rotation(Model3D* mesh, vec3 rotation) {
    glm_vec3_copy(rotation, mesh->rotation);
}

void model3d_set_scale(Model3D* mesh, vec3 scale) {
    glm_vec3_copy(scale, mesh->scale);
}

void model3d_get_position(Model3D* mesh, vec3 dest) {
    glm_vec3_copy(mesh->pos, dest);
}

void model3d_get_rotation(Model3D* mesh, vec3 dest) {
    glm_vec3_copy(mesh->rotation, dest);
}

void model3d_get_scale(Model3D* mesh, vec3 dest) {
    glm_vec3_copy(mesh->scale, dest);
}

Model* model3d_get_model(Model3D* mesh) {
    return mesh->model;
}
