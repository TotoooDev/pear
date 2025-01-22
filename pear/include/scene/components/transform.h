#ifndef PEAR_SCENE_COMPONENTS_TRANSFORM_COMPONENT_H_
#define PEAR_SCENE_COMPONENTS_TRANSFORM_COMPONENT_H_

#include <scene/component_attachment.h>
#include <core/types.h>

typedef struct transform_component_t {
    vec3 pos;
    vec3 rotation;
    vec3 scale;
} transform_component_t;

component_attachment_t transformcomponent_get_attachment();
void transformcomponent_get_model_matrix(transform_component_t* transform, mat4 dest);

#endif
