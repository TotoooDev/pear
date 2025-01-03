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

#define PEAR_SET_VALUE(script, value, name, lua_func) \
    if (!script->in_table_creation) { \
        lua_getglobal(script->state, "pear"); \
    } \
    lua_pushstring(script->state, name); \
    lua_func(script->state, value); \
    lua_settable(script->state, -3); \
    if (!script->in_table_creation) { \
        lua_setglobal(script->state, "pear"); \
    }

#define PEAR_GET_VALUE(script, name, type, lua_func) \
    if (!script->in_table_read) { \
        lua_getglobal(script->state, "pear"); \
    } \
    lua_getfield(script->state, -1, name); \
    type number = lua_func(script->state, -1); \
    if (script->in_table_read) { \
        lua_pop(script->state, 1); \
    } \
    else { \
        lua_pop(script->state, 3); \
    } \
    return number

typedef struct script_t {
    lua_State* state;
    bool in_table_creation;
    bool in_table_read;
    u32 table_creation_depth;
    u32 table_read_depth;
} script_t;

script_t* script_new(const char* script_str) {
    script_t* script = (script_t*)PEAR_MALLOC(sizeof(script_t));

    script->in_table_creation = false;
    script->in_table_read = false;
    script->table_creation_depth = 0;
    script->table_read_depth = 0;

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
    if (lua_isfunction(script->state, -1)) {
        PEAR_CALL_LUA(lua_pcall(script->state, 0, 0, 0));
    }
}

void script_on_update(script_t* script, f64 timestep) {
    lua_getglobal(script->state, "on_update");
    if (lua_isfunction(script->state, -1)) {
        lua_pushnumber(script->state, timestep);
        PEAR_CALL_LUA(lua_pcall(script->state, 1, 0, 0));
    }
}

void script_on_destroy(script_t* script) {
    lua_getglobal(script->state, "on_destroy");
    if (lua_isfunction(script->state, -1)) {
        PEAR_CALL_LUA(lua_pcall(script->state, 0, 0, 0));
    }
}

void script_set_number(script_t* script, f64 number, const char* name) {
    PEAR_SET_VALUE(script, number, name, lua_pushnumber);
}

void script_set_string(script_t* script, const char* str, const char* name) {
    PEAR_SET_VALUE(script, str, name, lua_pushstring);
}

void script_set_bool(script_t* script, bool boolean, const char* name) {
    PEAR_SET_VALUE(script, boolean, name, lua_pushboolean);
}

void script_set_function(script_t* script, lua_CFunction function, const char* name) {
    PEAR_SET_VALUE(script, function, name, lua_pushcfunction);
}

void script_begin_table(script_t* script, const char* name) {
    if (!script->in_table_creation) {
        script->in_table_creation = true;
        lua_getglobal(script->state, "pear");
    }
    script->table_creation_depth++;
    
    lua_pushstring(script->state, name);
    lua_newtable(script->state);
}

void script_end_table(script_t* script) {
    if (!script->in_table_creation) {
        PEAR_WARN("trying to call script_end_table without calling script_begin_table!");
    }

    lua_settable(script->state, -3);
    script->table_creation_depth--;
    script->in_table_creation = script->table_creation_depth > 0;

    if (!script->in_table_creation) {
        lua_setglobal(script->state, "pear");
    }
}

f64 script_get_number(script_t* script, const char* name) {
    PEAR_GET_VALUE(script, name, f64, lua_tonumber);
}

const char* script_get_string(script_t* script, const char* name) {
    PEAR_GET_VALUE(script, name, const char*, lua_tostring);
}

bool script_get_boolean(script_t* script, const char* name) {
    PEAR_GET_VALUE(script, name, bool, lua_toboolean);
}

void script_set_vec3(script_t* script, vec3 vec, const char* name) {
    script_begin_table(script, name);
        script_set_number(script, vec[0], "x");
        script_set_number(script, vec[1], "y");
        script_set_number(script, vec[2], "z");
    script_end_table(script);
}

void script_get_vec3(script_t* script, const char* name, vec3 dest) {
    script_get_table(script, name);
        dest[0] = script_get_number(script, "x");
        dest[1] = script_get_number(script, "y");
        dest[2] = script_get_number(script, "z");
    script_end_table_read(script);
}

void script_get_table(script_t* script, const char* name) {
    if (!script->in_table_read) {
        script->in_table_read = true;
        lua_getglobal(script->state, "pear");
    }
    script->table_read_depth++;

    lua_getfield(script->state, -1, name);
}

void script_end_table_read(script_t* script) {
    if (!script->in_table_read) {
        PEAR_WARN("trying to call script_end_table_read without calling script_get_table!");
        return;
    }

    lua_pop(script->state, 1);
    script->table_read_depth--;

    if (script->table_read_depth < 1) {
        lua_pop(script->state, 1);
    }

    script->in_table_read = script->table_read_depth > 0;
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
