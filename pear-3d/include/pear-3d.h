#ifndef PEAR3D_H
#define PEAR3D_H

#include <cglm/cglm.h>

typedef struct ModelData ModelData;

ModelData* pear3d_load_model(const char* filename);
void pear3d_save_model(const char* filename, ModelData* model_data);

ModelData* pear3d_new(vec3* vertices, unsigned int* indices, vec2* texture_coords, unsigned int num_vertices, unsigned int num_indices, unsigned int num_texture_coords);
void pear3d_delete(ModelData* model_data);

vec3* pear3d_get_vertices(ModelData* model_data);
unsigned int* pear3d_get_indices(ModelData* model_data);
vec2* pear3d_get_texture_coords(ModelData* model_data);

unsigned int pear3d_get_num_vertices(ModelData* model_data);
unsigned int pear3d_get_num_indices(ModelData* model_data);
unsigned int pear3d_get_num_texture_coords(ModelData* model_data);

#endif
