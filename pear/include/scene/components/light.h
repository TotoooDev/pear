#ifndef PEAR_SCENE_COMPONENT_LIGHT_H_
#define PEAR_SCENE_COMPONENT_LIGHT_H_

#include <graphics/light.h>

typedef struct light_component_t {
    light_t light;
    bool cast;
    bool shadow_caster;
} light_component_t;

light_component_t* lightcomponent_new();

#endif
