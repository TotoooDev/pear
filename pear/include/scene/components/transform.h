#ifndef PEAR_SCENE_COMPONENTS_H_
#define PEAR_SCENE_COMPONENTS_H_

#include <core/types.h>

typedef struct transform_component_t {
    vec3 pos;
    vec3 rotation;
    vec3 scale;
} transform_component_t;

transform_component_t* transformcomponent_create();

#endif
