#ifndef PEAR_LIGHT_H
#define PEAR_LIGHT_H

#include <core/types.h>

typedef enum LightType {
    LIGHT_TYPE_DIRECTIONAL,
    LIGHT_TYPE_POINT,
    LIGHT_TYPE_SPOT
} LightType;

typedef struct Light {
    LightType type;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    f32 constant;
    f32 linear;
    f32 quadratic;

    f32 cut_off;
    f32 outer_cut_off;
} Light;

#endif
