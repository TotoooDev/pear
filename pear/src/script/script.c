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
} script_t;

static i32 script_log_info(lua_State* l) {
    const char* str = lua_tostring(l, 1);
    PEAR_INFO("[LUA] %s", str);
    return 0;
}

script_t* script_new(const char* script_str) {
    script_t* script = (script_t*)PEAR_MALLOC(sizeof(script_t));

    script->state = luaL_newstate();
    luaL_openlibs(script->state);

    if (luaL_dostring(script->state, script_str) != LUA_OK) {
        PEAR_ERROR_LUA();
        script_delete(script);
        return NULL;
    }

    static const struct luaL_Reg lib[] = {
        { "log_info", script_log_info },
        { NULL, NULL }
    };
    luaL_newlib(script->state, lib);
    lua_setglobal(script->state, "pear");

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

void script_set_string(script_t* script, const char* str, const char* name) {
    lua_pushstring(script->state, str);
    lua_setglobal(script->state, name);
}

void script_set_bool(script_t* script, bool boolean, const char* name) {
    lua_pushboolean(script->state, boolean);
    lua_setglobal(script->state, name);
}

f64 script_get_number(script_t* script, const char* name) {
    lua_getglobal(script->state, name);
    return lua_tonumber(script->state, -1);
}

const char* script_get_string(script_t* script, const char* name) {
    lua_getglobal(script->state, name);
    return lua_tostring(script->state, -1);
}

bool script_get_boolean(script_t* script, const char* name) {
    lua_getglobal(script->state, name);
    return lua_toboolean(script->state, -1);
}

void script_dump_stack(script_t* script) {
    PEAR_INFO("stack dump!");

    int i;
    int top = lua_gettop(script->state);
    for (i = 1; i <= top; i++) {  /* repeat for each level */
        int t = lua_type(script->state, i);
        switch (t) {
        case LUA_TSTRING:  /* strings */
            PEAR_INFO("  `%s'", lua_tostring(script->state, i));
            break;
    
        case LUA_TBOOLEAN:  /* booleans */
            PEAR_INFO("  %s", lua_toboolean(script->state, i) ? "true" : "false");
            break;
    
        case LUA_TNUMBER:  /* numbers */
            PEAR_INFO("  %g", lua_tonumber(script->state, i));
            break;
    
        default:  /* other values */
            PEAR_INFO("  %s", lua_typename(script->state, t));
            break;
        }
    }
}
