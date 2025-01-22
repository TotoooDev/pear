#ifndef PEAR_SCENE_COMPONENTS_SKYBOX_COMPONENT_H_
#define PEAR_SCENE_COMPONENTS_SKYBOX_COMPONENT_H_

#include <scene/component_attachment.h>
#include <graphics/cubemap.h>
#include <core/types.h>

typedef struct skybox_component_t {
    cubemap_t* cubemap;
    bool draw;
} skybox_component_t;

component_attachment_t skyboxcomponent_get_attachment();

#endif
