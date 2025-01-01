#include <script/script.h>
#include <util/filesystem.h>
#include <core/alloc.h>
#include <script/vendor/lua.h>
#include <script/vendor/lauxlib.h>
#include <script/vendor/lualib.h>
#include <string.h>

typedef struct script_t {
    lua_State* state;
} script_t;

script_t* script_new(const char* script_str) {
    script_t* script = (script_t*)PEAR_MALLOC(sizeof(script_t));

    script->state = luaL_newstate();
    luaL_openlibs(script->state);

    // luaL_loadbuffer(script->state, script_str, strlen(script_str), "cool lua script");
    luaL_dostring(script->state, script_str);

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
