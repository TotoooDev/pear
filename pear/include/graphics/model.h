#ifndef PEAR_MODEL_H
#define PEAR_MODEL_H

#include <graphics/mesh.h>
#include <pear-3d.h>

typedef struct Model Model;

Model* model_new(Mesh** meshes, u32 num_meshes);
Model* model_new_from_pear3d(Pear3D pear3d);
void model_delete(Model* model);

Mesh** model_get_meshes(Model* model);
u32 model_get_num_meshes(Model* model);

#endif
