#ifndef PEAR_SCENE_COMPONENTS_LIGHT_COMPONENT_H_
#define PEAR_SCENE_COMPONENTS_LIGHT_COMPONENT_H_

#include <scene/component_attachment.h>
#include <graphics/light.h>

typedef struct light_component_t {
    light_t light;
    bool shadow_caster;
    bool cast;
} light_component_t;

component_attachment_t lightcomponent_get_attachment();

#endif
