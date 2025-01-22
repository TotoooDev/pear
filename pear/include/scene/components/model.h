#ifndef PEAR_SCENE_COMPONENTS_MODEL_COMPONENT_H_
#define PEAR_SCENE_COMPONENTS_MODEL_COMPONENT_H_

#include <scene/component_attachment.h>
#include <graphics/model.h>
#include <core/types.h>

typedef struct model_component_t {
    model_t* model;
    bool draw;
    bool shadow_caster;
} model_component_t;

component_attachment_t modelcomponent_get_attachment();

#endif
