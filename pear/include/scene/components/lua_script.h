#ifndef PEAR_SCENE_COMPONENTS_LUA_SCRIPT_COMPONENT_H_
#define PEAR_SCENE_COMPONENTS_LUA_SCRIPT_COMPONENT_H_

#include <scene/component_attachment.h>
#include <script/script.h>
#include <core/types.h>

typedef struct lua_script_component_t {
    script_t* script;
    bool has_started;
    bool run;
} lua_script_component_t;

component_attachment_t luascriptcomponent_get_attachment();

#endif
