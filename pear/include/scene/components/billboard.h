#ifndef PEAR_SCENE_COMPONENTS_BILLBOARD_COMPONENT_H_
#define PEAR_SCENE_COMPONENTS_BILLBOARD_COMPONENT_H_

#include <scene/component_attachment.h>
#include <graphics/texture.h>
#include <core/types.h>

typedef struct billboard_component_t {
    texture_t* texture;
    bool draw;
} billboard_component_t;

component_attachment_t billboardcomponent_get_attachment();

#endif
