#include <script/script.h>
#include <script/vec3.h>
#include <script/log.h>
#include <event/event_dispatcher.h>
#include <event/keyboard.h>
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
    f64 timestep;
} script_t;

void script_on_event(event_type_t type, void* e, void* user_data) {
    script_t* script = (script_t*)user_data;

    if (type == EVENT_TYPE_KEY_DOWN) {
        key_down_event_t* event = (key_down_event_t*)e;
        script_begin_table(script, "key");
            script_set_string(script, event_get_key_string(event->key), event_get_key_string(event->key));
            script_begin_table(script, "state");
                script_set_bool(script, true, event_get_key_string(event->key));
            script_end_table(script);
        script_end_table(script);

        lua_getglobal(script->state, "on_key_press");
        if (lua_isfunction(script->state, -1)) {
            lua_pushstring(script->state, event_get_key_string(event->key));
            PEAR_CALL_LUA(lua_pcall(script->state, 1, 0, 0));
        }
        else {
            lua_pop(script->state, -1);
        }
    }

    if (type == EVENT_TYPE_KEY_UP) {
        key_up_event_t* event = (key_up_event_t*)e;
        script_begin_table(script, "key");
            script_begin_table(script, "state");
                script_set_bool(script, false, event_get_key_string(event->key));
            script_end_table(script);
        script_end_table(script);

        lua_getglobal(script->state, "on_key_release");
        if (lua_isfunction(script->state, -1)) {
            lua_pushstring(script->state, event_get_key_string(event->key));
            PEAR_CALL_LUA(lua_pcall(script->state, 1, 0, 0));
        }
        else {
            lua_pop(script->state, -1);
        }
    }

    if (type == EVENT_TYPE_MOUSE_MOVED) {
        mouse_moved_event_t* event = (mouse_moved_event_t*)e;
        vec3 relative = { event->rel_x, event->rel_y, 0.0f };
        vec3 pos = { event->x, event->y, 0.0f };

        script_begin_table(script, "mouse");
            script_set_vec3(script, pos, "pos");
            script_set_vec3(script, relative, "relative");
        script_end_table(script);

        lua_getglobal(script->state, "on_mouse_movement");
        if (lua_isfunction(script->state, -1)) {
            PEAR_CALL_LUA(lua_pcall(script->state, 0, 0, 0));
        }
        else {
            lua_pop(script->state, -1);
        }
    }
}

void script_dump_stack_state(lua_State* l) {
    PEAR_INFO("stack dump!");

    int i;
    int top = lua_gettop(l);
    for (i = 1; i <= top; i++) {  /* repeat for each level */
        int t = lua_type(l, i);
        switch (t) {
        case LUA_TSTRING:  /* strings */
            PEAR_INFO("  `%s'", lua_tostring(l, i));
            break;
    
        case LUA_TBOOLEAN:  /* booleans */
            PEAR_INFO("  %s", lua_toboolean(l, i) ? "true" : "false");
            break;
    
        case LUA_TNUMBER:  /* numbers */
            PEAR_INFO("  %g", lua_tonumber(l, i));
            break;
    
        default:  /* other values */
            PEAR_INFO("  %s", lua_typename(l, t));
            break;
        }
    }
}

script_t* script_new(const char* script_str) {
    script_t* script = (script_t*)PEAR_MALLOC(sizeof(script_t));

    script->in_table_creation = false;
    script->in_table_read = false;
    script->table_creation_depth = 0;
    script->table_read_depth = 0;
    script->timestep = 0.0f;

    script->state = luaL_newstate();
    luaL_openlibs(script->state);

    if (luaL_dostring(script->state, script_str) != LUA_OK) {
        PEAR_ERROR_LUA();
        script_delete(script);
        return NULL;
    }

    lua_newtable(script->state);
    lua_setglobal(script->state, "pear");

    script_init_vec3(script);

    script_begin_table(script, "log");
        script_set_function(script, script_log_info, "info");
        script_set_function(script, script_log_warn, "warn");
        script_set_function(script, script_log_error, "error");
    script_end_table(script);

    script_begin_table(script, "key");
    script_end_table(script);

    script_begin_table(script, "mouse");
        script_set_vec3(script, (vec3){ 0.0f, 0.0f, 0.0f }, "pos");
        script_set_vec3(script, (vec3){ 0.0f, 0.0f, 0.0f }, "relative");
    script_end_table(script);

    #ifdef PEAR_ENABLE_EDITOR
        script_init_editor(script);
    #else
        script_begin_table(script, "editor");
            script_set_bool(script, false, "enabled");
        script_end_table(script);
    #endif

    event_subscribe(script_on_event, script);

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
    else {
        lua_pop(script->state, -1);
    }
}

void script_on_update(script_t* script, f64 timestep) {
    script->timestep = timestep;
    
    lua_getglobal(script->state, "on_update");
    if (lua_isfunction(script->state, -1)) {
        lua_pushnumber(script->state, script->timestep);
        PEAR_CALL_LUA(lua_pcall(script->state, 1, 0, 0));
    }
    else {
        lua_pop(script->state, -1);
    }

    script_begin_table(script, "mouse");
        script_set_vec3(script, (vec3){ 0.0f, 0.0f, 0.0f }, "relative");
    script_end_table(script);
}

void script_on_destroy(script_t* script) {
    lua_getglobal(script->state, "on_destroy");
    if (lua_isfunction(script->state, -1)) {
        PEAR_CALL_LUA(lua_pcall(script->state, 0, 0, 0));
    }
    else {
        lua_pop(script->state, -1);
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

void script_set_vec3(script_t* script, vec3 vec, const char* name) {
    if (!script->in_table_creation) {
        lua_getglobal(script->state, "pear");
    }

    lua_pushstring(script->state, name);

    f32* udata;
    
    lua_getglobal(script->state, name);
    if (!lua_isuserdata(script->state, -1)) {
        lua_pop(script->state, 1);
        udata = (f32*)lua_newuserdata(script->state, sizeof(vec3));
        luaL_getmetatable(script->state, "pear.vec3");
        lua_setmetatable(script->state, -2);
    }
    else {
        udata = (f32*)lua_touserdata(script->state, -1);
    }
    
    udata[0] = vec[0];
    udata[1] = vec[1];
    udata[2] = vec[2];
    
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

bool script_get_bool(script_t* script, const char* name) {
    PEAR_GET_VALUE(script, name, bool, lua_toboolean);
}

void script_get_vec3(script_t* script, const char* name, vec3 dest) {
    if (!script->in_table_read) {
        lua_getglobal(script->state, "pear");
    }

    lua_getfield(script->state, -1, name);
    f32* value = luaL_checkudata(script->state, -1, "pear.vec3");
    dest[0] = value[0];
    dest[1] = value[1];
    dest[2] = value[2];

    if (script->in_table_read) {
        lua_pop(script->state, 1);
    }
    else {
        lua_pop(script->state, 2);
    }
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
    script_dump_stack_state(script->state);
}

lua_State* script_get_state(script_t* script) {
    return script->state;
}

#ifdef PEAR_ENABLE_EDITOR
#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <graphics/editor/vendor/cimgui/cimgui.h>

i32 script_checkbox_item(lua_State* l) {
    const char* label = lua_tostring(l, 1);
    bool value = lua_toboolean(l, 2);
    igCheckbox(label, &value);
    lua_pushboolean(l, value);
    return 1;
}

i32 script_drag_number_item(lua_State* l) {
    const char* label = lua_tostring(l, 1);
    f32 value = lua_tonumber(l, 2);
    f64 speed = lua_tonumber(l, 3);
    f32 min = lua_tonumber(l, 4);
    f32 max = lua_tonumber(l, 5);
    igDragFloat(label, &value, speed, min, max, "%.3f", ImGuiSliderFlags_None);
    lua_pushnumber(l, value);
    return 1;
}

void script_init_editor(script_t* script) {
    script_begin_table(script, "editor");
        script_set_bool(script, true, "enabled");
        script_set_function(script, script_checkbox_item, "checkbox");
        script_set_function(script, script_drag_number_item, "drag_number");
    script_end_table(script);
}

void script_on_editor(script_t* script) {
    igSeparator();

    lua_getglobal(script->state, "on_editor");
    if (lua_isfunction(script->state, -1)) {
        PEAR_CALL_LUA(lua_pcall(script->state, 0, 0, 0));
    }
    else {
        lua_pop(script->state, -1);
    }
}
#endif
