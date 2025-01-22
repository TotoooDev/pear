#include <scene/components/lua_script.h>

void luascriptcomponent_constructor(void* data, entity_t* entity) {
    lua_script_component_t* lua_script = (lua_script_component_t*)data;

    lua_script->script = NULL;
    lua_script->has_started = false;
    lua_script->run = true;
}

void luascriptcomponent_destructor(void* data, entity_t* entity) {
    lua_script_component_t* lua_script = (lua_script_component_t*)data;

    if (lua_script->script != NULL) {
        script_delete(lua_script->script);
    }
}

component_attachment_t luascriptcomponent_get_attachment() {
    return (component_attachment_t){
        .creator = luascriptcomponent_constructor,
        .destructor = luascriptcomponent_destructor
    };
}
