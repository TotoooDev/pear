#include <script/component.h>
#include <script/vendor/lua.h>
#include <script/vendor/lauxlib.h>
#include <scene/entity.h>

i32 script_component_add(lua_State* l) {
    const char* component_name = luaL_checkstring(l, 1);

    lua_getglobal(l, "pear");

    lua_getfield(l, -1, "entity");
    entity_t* entity = (entity_t*)lua_touserdata(l, -1);
    lua_getfield(l, -1, "scene");
    scene_t* scene = (scene_t*)lua_touserdata(l, -1);

    scene_add_component(scene, entity, component_name);

    lua_pop(l, 3);

    return 0;
}

i32 script_component_remove(lua_State* l) {
    const char* component_name = luaL_checkstring(l, 1);

    lua_getglobal(l, "pear");

    lua_getfield(l, -1, "entity");
    entity_t* entity = (entity_t*)lua_touserdata(l, -1);
    lua_getfield(l, -1, "scene");
    scene_t* scene = (scene_t*)lua_touserdata(l, -1);

    scene_remove_component(scene, entity, component_name);

    lua_pop(l, 3);

    return 0;
}

void script_init_component(script_t* script) {
    script_begin_table(script, "component");
        script_set_function(script, script_component_add, "add");
        script_set_function(script, script_component_remove, "remove");
    script_end_table(script);
}
