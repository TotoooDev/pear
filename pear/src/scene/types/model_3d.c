#include <scene/types/model_3d.h>
#include <stdlib.h>

typedef struct Model3D {
    Model* model;
    vec3 pos;
    vec3 rotation;
    vec3 scale;
} Model3D;

Model3D* model3d_new(Model3DCreationInfo* creation_info) {
    Model3D* model3d = (Model3D*)malloc(sizeof(Model3D));
    
    model3d->model = creation_info->model;
    glm_vec3_copy(creation_info->pos, model3d->pos);
    glm_vec3_copy(creation_info->rotation, model3d->rotation);
    glm_vec3_copy(creation_info->scale, model3d->scale);
    
    return model3d;
}

void model3d_delete(Model3D* model) {
    free(model);
}

void model3d_set_position(Model3D* model, vec3 pos) {
    glm_vec3_copy(pos, model->pos);
}

void model3d_set_rotation(Model3D* model, vec3 rotation) {
    glm_vec3_copy(rotation, model->rotation);
}

void model3d_set_scale(Model3D* model, vec3 scale) {
    glm_vec3_copy(scale, model->scale);
}

void model3d_get_position(Model3D* model, vec3 dest) {
    glm_vec3_copy(model->pos, dest);
}

void model3d_get_rotation(Model3D* model, vec3 dest) {
    glm_vec3_copy(model->rotation, dest);
}

void model3d_get_scale(Model3D* model, vec3 dest) {
    glm_vec3_copy(model->scale, dest);
}

void model3d_get_model_matrix(Model3D* model, mat4 dest) {
    mat4 model_matrix;
    glm_mat4_identity(model_matrix);

    glm_translate(model_matrix, model->pos);
    glm_rotate(model_matrix, model->rotation[0], (vec3){ 1.0f, 0.0f, 0.0f });
    glm_rotate(model_matrix, model->rotation[1], (vec3){ 0.0f, 1.0f, 0.0f });
    glm_rotate(model_matrix, model->rotation[2], (vec3){ 0.0f, 0.0f, 1.0f });
    glm_scale(model_matrix, model->scale);

    glm_mat4_copy(model_matrix, dest);
}

Model* model3d_get_model(Model3D* model) {
    return model->model;
}
