#include <scene/components/script.h>
#include <core/alloc.h>

script_component_t* scriptcomponent_new() {
    script_component_t* script = (script_component_t*)PEAR_MALLOC(sizeof(script_component_t));
    
    script->on_start = NULL;
    script->on_update = NULL;
    script->on_end = NULL;
    script->has_started = false;
    script->run = true;

    return script;
}
