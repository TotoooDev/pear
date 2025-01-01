#include <script/script.h>
#include <util/filesystem.h>
#include <core/types.h>
#include <core/log.h>
#include <core/alloc.h>
#include <script/vendor/lua.h>
#include <script/vendor/lauxlib.h>
#include <script/vendor/lualib.h>
#include <string.h>

#define PEAR_ERROR_LUA(script) PEAR_ERROR("[LUA] %s", lua_tostring(script->state, -1))

typedef struct script_t {
    lua_State* state;
} script_t;

script_t* script_new(const char* script_str) {
    script_t* script = (script_t*)PEAR_MALLOC(sizeof(script_t));

    script->state = luaL_newstate();
    luaL_openlibs(script->state);

    if (luaL_dostring(script->state, script_str) != LUA_OK) {
        PEAR_ERROR_LUA(script);
        script_delete(script);
        return NULL;
    }

    lua_getglobal(script->state, "foo");
    f64 foo = lua_tonumber(script->state, -1);
    PEAR_INFO("%f", foo);

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
