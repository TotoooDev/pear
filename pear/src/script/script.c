#include <script/script.h>
#include <util/filesystem.h>
#include <core/log.h>
#include <core/alloc.h>
#include <script/vendor/lua.h>
#include <script/vendor/lauxlib.h>
#include <script/vendor/lualib.h>
#include <string.h>

// for these macros, we are under the assumption that the script will always be called "script"
// this may or may not be stupid
#define PEAR_ERROR_LUA() PEAR_ERROR("[LUA] %s", lua_tostring(script->state, -1))
#define PEAR_CALL_LUA(ret) if (ret != LUA_OK) { PEAR_ERROR_LUA(); }

typedef struct script_t {
    lua_State* state;
    bool has_started;
} script_t;

script_t* script_new(const char* script_str) {
    script_t* script = (script_t*)PEAR_MALLOC(sizeof(script_t));

    script->has_started = false;
    script->state = luaL_newstate();
    luaL_openlibs(script->state);

    if (luaL_dostring(script->state, script_str) != LUA_OK) {
        PEAR_ERROR_LUA();
        script_delete(script);
        return NULL;
    }

    lua_pcall(script->state, 0, 0, 0);

    return script;
}

script_t* script_new_from_file(const char* filename) {
    char* script_str = filesystem_read_file(filename);
    script_t* script = script_new(script_str);
    PEAR_FREE(script_str);
    return script;
}

void script_delete(script_t* script) {
    lua_close(script->state);
    PEAR_FREE(script);
}

void script_on_start(script_t* script) {
    lua_getglobal(script->state, "on_start");
    PEAR_CALL_LUA(lua_pcall(script->state, 0, 0, 0));
    script->has_started = true;
}

void script_on_update(script_t* script, f64 timestep) {
    lua_getglobal(script->state, "on_update");
    lua_pushnumber(script->state, timestep);
    PEAR_CALL_LUA(lua_pcall(script->state, 1, 0, 0));
}

void script_on_destroy(script_t* script) {
    lua_getglobal(script->state, "on_destroy");
    PEAR_CALL_LUA(lua_pcall(script->state, 0, 0, 0));
}

void script_set_number(script_t* script, f64 number, const char* name) {
    lua_pushnumber(script->state, number);
    lua_setglobal(script->state, name);
}
