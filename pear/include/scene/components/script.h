#ifndef PEAR_SCENE_COMPONENTS_SCRIPT_H_
#define PEAR_SCENE_COMPONENTS_SCRIPT_H_

#include <scene/entity.h>

typedef void(*script_function_t)(entity_t*, f32);

typedef struct script_component_t {
    script_function_t on_start;
    script_function_t on_update;
    script_function_t on_end;

    bool has_started;
    bool run;
} script_component_t;

script_component_t* scriptcomponent_new();

#endif
