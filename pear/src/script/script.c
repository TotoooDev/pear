#include <script/script.h>
#include <script/log.h>
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
    bool in_table_creation;
    u32 table_depth;
} script_t;

script_t* script_new(const char* script_str) {
    script_t* script = (script_t*)PEAR_MALLOC(sizeof(script_t));

    script->in_table_creation = false;
    script->table_depth = 0;

    script->state = luaL_newstate();
    luaL_openlibs(script->state);

    if (luaL_dostring(script->state, script_str) != LUA_OK) {
        PEAR_ERROR_LUA();
        script_delete(script);
        return NULL;
    }

    lua_newtable(script->state);
    lua_setglobal(script->state, "pear");

    script_begin_table(script, "log");
        script_set_function(script, script_log_info, "info");
        script_set_function(script, script_log_warn, "warn");
        script_set_function(script, script_log_error, "error");
    script_end_table(script);

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
    if (!script->in_table_creation) {
        lua_getglobal(script->state, "pear");
    }
    
    lua_pushstring(script->state, name);
    lua_pushnumber(script->state, number);
    lua_settable(script->state, -3);

    if (!script->in_table_creation) {
        lua_setglobal(script->state, "pear");
    }
}

void script_set_string(script_t* script, const char* str, const char* name) {
    if (!script->in_table_creation) {
        lua_getglobal(script->state, "pear");
    }
    
    lua_pushstring(script->state, name);
    lua_pushstring(script->state, str);
    lua_settable(script->state, -3);

    if (!script->in_table_creation) {
        lua_setglobal(script->state, "pear");
    }
}

void script_set_bool(script_t* script, bool boolean, const char* name) {
    if (!script->in_table_creation) {
        lua_getglobal(script->state, "pear");
    }
    
    lua_pushstring(script->state, name);
    lua_pushboolean(script->state, boolean);
    lua_settable(script->state, -3);

    if (!script->in_table_creation) {
        lua_setglobal(script->state, "pear");
    }
}

void script_set_function(script_t* script, lua_CFunction function, const char* name) {
    if (!script->in_table_creation) {
        lua_getglobal(script->state, "pear");
    }
    
    lua_pushstring(script->state, name);
    lua_pushcfunction(script->state, function);
    lua_settable(script->state, -3);

    if (!script->in_table_creation) {
        lua_setglobal(script->state, "pear");
    }
}

void script_begin_table(script_t* script, const char* name) {
    if (!script->in_table_creation) {
        script->in_table_creation = true;
        lua_getglobal(script->state, "pear");
    }
    script->table_depth++;
    
    lua_pushstring(script->state, name);
    lua_newtable(script->state);
}

void script_end_table(script_t* script) {
    if (script->in_table_creation) {
        lua_settable(script->state, -3);
        script->table_depth--;
        script->in_table_creation = script->table_depth > 0;

        if (!script->in_table_creation) {
            lua_setglobal(script->state, "pear");
        }
    }
    else {
        PEAR_WARN("trying to call script_end_table without calling script_begin_table!");
    }
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
