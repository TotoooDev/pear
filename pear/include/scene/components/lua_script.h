#ifndef PEAR_SCENE_COMPONENT_LUA_SCRIPT_H_
#define PEAR_SCENE_COMPONENT_LUA_SCRIPT_H_

#include <script/script.h>

typedef struct lua_script_component_t {
    script_t* script;
    bool has_started;
    bool run;
} lua_script_component_t;

#endif
