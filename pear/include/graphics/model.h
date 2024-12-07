#ifndef PEAR_GRAPHICS_MODEL_H_
#define PEAR_GRAPHICS_MODEL_H_

#include <graphics/mesh.h>
#include <graphics/material.h>
#include <pear_model.h>

typedef struct model_t model_t;

model_t* model_new(mesh_t** meshes, material_t* materials, u32 num_meshes, u32 num_materials);
model_t* model_new_from_pear_model(pear_model_t model);
void model_delete(model_t* model);

u32 model_get_num_meshes(model_t* model);
u32 model_get_num_materials(model_t* model);
mesh_t** model_get_meshes(model_t* model);
material_t* model_get_materials(model_t* model);

#endif
