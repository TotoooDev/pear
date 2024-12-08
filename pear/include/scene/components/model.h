#ifndef PEAR_SCENE_COMPONENTS_DRAWABLE_H_
#define PEAR_SCENE_COMPONENTS_DRAWABLE_H_

#include <graphics/model.h>

typedef struct model_component_t {
    model_t* model;
    bool draw;
} model_component_t;

#endif
