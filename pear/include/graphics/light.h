#ifndef PEAR_GRAPHICS_LIGHT_H_
#define PEAR_GRAPHICS_LIGHT_H_

#include <core/types.h>

typedef enum light_type_t {
    LIGHT_TYPE_DIRECTIONAL,
    LIGHT_TYPE_POINT,
    LIGHT_TYPE_SPOT
} light_type_t;

typedef struct light_t {
    light_type_t type;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    f32 constant;
    f32 linear;
    f32 quadratic;

    f32 cutoff;
    f32 outer_cutoff;
} light_t;

#endif
