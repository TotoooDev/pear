#ifndef PEAR_SCENE_COMPONENTS_SKYBOX_H_
#define PEAR_SCENE_COMPONENTS_SKYBOX_H_

#include <graphics/cubemap.h>

typedef struct skybox_component_t {
    cubemap_t* cubemap;
    bool draw;
} skybox_component_t;

#endif
