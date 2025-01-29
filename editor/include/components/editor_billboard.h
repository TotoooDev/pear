#ifndef PEAR_COMPONENTS_EDITOR_BILLBOARD_H_
#define PEAR_COMPONENTS_EDITOR_BILLBOARD_H_

#include <scene/component_attachment.h>
#include <graphics/texture.h>

typedef struct editor_billboard_component_t {
    texture_t* texture;
    bool draw;
} editor_billboard_component_t;

component_attachment_t editorbillboardcomponent_get_attachment();

#endif
