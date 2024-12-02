#ifndef PEAR_SCENE_COMPONENTS_H_
#define PEAR_SCENE_COMPONENTS_H_

#include <core/types.h>

typedef struct transform_component_t {
    vec3 pos;
    vec3 rotation;
    vec3 scale;
} transform_component_t;

transform_component_t* transformcomponent_new();
void transformcomponent_get_model_matrix(transform_component_t* transform, mat4 dest);

#endif
