#include <scene/types/model_3d.h>
#include <scene/types/mesh_3d.h>
#include <stdlib.h>

typedef struct Model3D {
    Model* model;
    Transform transform;
} Model3D;

Model3D* model3d_new(Model3DCreationInfo* creation_info, Node* node) {
    Model3D* model3d = (Model3D*)malloc(sizeof(Model3D));
    
    model3d->model = creation_info->model;
    glm_vec3_copy(creation_info->transform.pos, model3d->transform.pos);
    glm_vec3_copy(creation_info->transform.rotation, model3d->transform.rotation);
    glm_vec3_copy(creation_info->transform.scale, model3d->transform.scale);

    for (u32 i = 0; i < model_get_num_meshes(model3d->model); i++) {
        Mesh* mesh = model_get_meshes(model3d->model)[i];

        // i love this way of initializing structs
        Mesh3DCreationInfo mesh_creation_info = {
            .mesh = mesh,
            .transform = {
                .pos = { 0.0f, 0.0f, 0.0f },
                .rotation = { 0.0f, 0.0f, 0.0f },
                .scale = { 1.0f, 1.0f, 1.0f }
            }
        };
        Node* son_mesh = node_new(NODE_TYPE_MESH_3D, node, "mesh", &mesh_creation_info, NULL);
        node_add_son(node, son_mesh);
    }
    
    return model3d;
}

void model3d_delete(Model3D* model) {
    free(model);
}

void model3d_set_position(Model3D* model, vec3 pos) {
    glm_vec3_copy(pos, model->transform.pos);
}

void model3d_set_rotation(Model3D* model, vec3 rotation) {
    glm_vec3_copy(rotation, model->transform.rotation);
}

void model3d_set_scale(Model3D* model, vec3 scale) {
    glm_vec3_copy(scale, model->transform.scale);
}

void model3d_get_position(Model3D* model, vec3 dest) {
    glm_vec3_copy(model->transform.pos, dest);
}

void model3d_get_rotation(Model3D* model, vec3 dest) {
    glm_vec3_copy(model->transform.rotation, dest);
}

void model3d_get_scale(Model3D* model, vec3 dest) {
    glm_vec3_copy(model->transform.scale, dest);
}

void model3d_get_model_matrix(Model3D* model, mat4 dest) {
    mat4 model_matrix;
    glm_mat4_identity(model_matrix);

    glm_translate(model_matrix, model->transform.pos);
    glm_rotate(model_matrix, model->transform.rotation[0], (vec3){ 1.0f, 0.0f, 0.0f });
    glm_rotate(model_matrix, model->transform.rotation[1], (vec3){ 0.0f, 1.0f, 0.0f });
    glm_rotate(model_matrix, model->transform.rotation[2], (vec3){ 0.0f, 0.0f, 1.0f });
    glm_scale(model_matrix, model->transform.scale);

    glm_mat4_copy(model_matrix, dest);
}

Model* model3d_get_model(Model3D* model) {
    return model->model;
}
