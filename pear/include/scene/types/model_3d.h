#ifndef PEAR_NODE_MODEl3D_H
#define PEAR_NODE_MODEl3D_H

#include <graphics/model.h>
#include <cglm/cglm.h>

typedef struct Model3DCreationInfo {
    Model* model;
    vec3 pos;
    vec3 rotation;
    vec3 scale;
} Model3DCreationInfo;

typedef struct Model3D Model3D;

Model3D* model3d_new(Model3DCreationInfo* creation_info);
void model3d_delete(Model3D* model);

void model3d_set_position(Model3D* model, vec3 pos);
void model3d_set_rotation(Model3D* model, vec3 rotation);
void model3d_set_scale(Model3D* model, vec3 scale);

void model3d_get_position(Model3D* model, vec3 dest);
void model3d_get_rotation(Model3D* model, vec3 dest);
void model3d_get_scale(Model3D* model, vec3 dest);
void model3d_get_model_matrix(Model3D* model, mat4 dest);
Model* model3d_get_model(Model3D* model);

#endif
