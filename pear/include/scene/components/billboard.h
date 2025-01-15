#ifndef PEAR_SCENE_COMPONENTS_BILLBOARD_H_
#define PEAR_SCENE_COMPONENTS_BILLBOARD_H_

#include <graphics/texture.h>

typedef struct billboard_component_t {
    texture_t* texture;
    bool on_top;
    bool apply_perspective;
    bool draw;
} billboard_component_t;

#endif
