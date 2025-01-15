#include <script/component.h>
#include <script/vendor/lua.h>
#include <script/vendor/lauxlib.h>
#include <scene/entity.h>

i32 script_component_add(lua_State* l) {
    i32 component_index = luaL_checkinteger(l, 1);

    lua_getglobal(l, "pear");
    lua_getfield(l, -1, "entity");

    entity_t* entity = (entity_t*)lua_touserdata(l, -1);
    entity_add_component(entity, (entity_component_t)component_index);

    lua_pop(l, 3);

    return 0;
}

i32 script_component_remove(lua_State* l) {
    i32 component_index = luaL_checkinteger(l, 1);

    lua_getglobal(l, "pear");
    lua_getfield(l, -1, "entity");

    entity_t* entity = (entity_t*)lua_touserdata(l, -1);
    entity_remove_component(entity, (entity_component_t)component_index);

    lua_pop(l, 3);
    
    return 0;
}

void script_init_component(script_t* script) {
    script_begin_table(script, "component");
        script_set_number(script, ENTITY_COMPONENT_TRANSFORM, "TRANSFORM");
        script_set_number(script, ENTITY_COMPONENT_MODEL, "MODEL");
        script_set_number(script, ENTITY_COMPONENT_BILLBOARD, "BILLBOARD");
        script_set_number(script, ENTITY_COMPONENT_CAMERA, "CAMERA");
        script_set_number(script, ENTITY_COMPONENT_LUA_SCRIPT, "SCRIPT");
        script_set_number(script, ENTITY_COMPONENT_LIGHT, "LIGHT");
        script_set_number(script, ENTITY_COMPONENT_SKYBOX, "SKYBOX");
        script_set_function(script, script_component_add, "add");
        script_set_function(script, script_component_remove, "remove");
    script_end_table(script);
}
