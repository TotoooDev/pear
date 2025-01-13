#include <script/vec3.h>
#include <script/vendor/lua.h>
#include <script/vendor/lauxlib.h>
#include <string.h>

i32 script_vec3_index(lua_State* l) {
    f32* vec = luaL_checkudata(l, 1, "pear.vec3");
    luaL_argcheck(l, vec != NULL, 1, "'vec3' expected");

    switch (lua_type(l, 2)) {
    case LUA_TSTRING:
        const char* index_str = luaL_checkstring(l, 2);
        luaL_argcheck(l, index_str != NULL, 2, "'string' expected");
        if (strcmp(index_str, "x") == 0) {
            lua_pushnumber(l, vec[0]);
        }
        else if (strcmp(index_str, "y") == 0) {
            lua_pushnumber(l, vec[1]);
        }
        else if (strcmp(index_str, "z") == 0) {
            lua_pushnumber(l, vec[2]);
        }
        else {
            lua_pushnil(l);
        }
        break;

    case LUA_TNUMBER:
        i32 index_int = luaL_checknumber(l, 2);
        switch (index_int) {
        case 0:
            lua_pushnumber(l, vec[0]);
            break;
        case 1:
            lua_pushnumber(l, vec[1]);
            break;
        case 2:
            lua_pushnumber(l, vec[2]);
            break;
        default:
            lua_pushnil(l);
            break;
        }
        break;

    default:
        lua_pushnil(l);
        break;
    }

    return 1;
}

i32 script_vec3_newindex(lua_State* l) {
    f32* vec = luaL_checkudata(l, 1, "pear.vec3");
    luaL_argcheck(l, vec != NULL, 1, "'vec3' expected");

    f32* ptr = NULL;
    switch (lua_type(l, 2)) {
    case LUA_TSTRING:
        const char* index_str = luaL_checkstring(l, 2);
        luaL_argcheck(l, index_str != NULL, 2, "'string' expected");
        if (strcmp(index_str, "x") == 0) {
            ptr = &vec[0];
        }
        else if (strcmp(index_str, "y") == 0) {
            ptr = &vec[1];
        }
        else if (strcmp(index_str, "z") == 0) {
            ptr = &vec[2];
        }
        break;

    case LUA_TNUMBER:
        i32 index_int = luaL_checknumber(l, 2);
        switch (index_int) {
        case 0:
            ptr = &vec[0];
            break;
        case 1:
            ptr = &vec[1];
            break;
        case 2:
            ptr = &vec[2];
            break;
        default:
            break;
        }
        break;

    default:
        break;
    }

    f64 value = luaL_checknumber(l, 3);

    *ptr = value;

    return 0;
}

i32 script_vec3_tostring(lua_State* l) {
    f32* vec = luaL_checkudata(l, 1, "pear.vec3");
    luaL_argcheck(l, vec != NULL, 1, "'vec3' expected");
    char str[33];
    sprintf(str, "{ %.6f, %6f, %6f }", vec[0], vec[1], vec[2]);
    lua_pushstring(l, str);
    return 1;
}

i32 script_vec3_add(lua_State* l) {
    f32* a = luaL_checkudata(l, 1, "pear.vec3");
    luaL_argcheck(l, a != NULL, 1, "'vec3' expected");
    f32* b = luaL_checkudata(l, 2, "pear.vec3");
    luaL_argcheck(l, b != NULL, 2, "'vec3' expected");

    f32* res = (f32*)lua_newuserdata(l, sizeof(vec3));
    luaL_getmetatable(l, "pear.vec3");
    lua_setmetatable(l, -2);
    res[0] = a[0] + b[0];
    res[1] = a[1] + b[1];
    res[2] = a[2] + b[2];

    return 1;
}

i32 script_vec3_sub(lua_State* l) {
    f32* a = luaL_checkudata(l, 1, "pear.vec3");
    luaL_argcheck(l, a != NULL, 1, "'vec3' expected");
    f32* b = luaL_checkudata(l, 2, "pear.vec3");
    luaL_argcheck(l, b != NULL, 2, "'vec3' expected");

    f32* res = (f32*)lua_newuserdata(l, sizeof(vec3));
    luaL_getmetatable(l, "pear.vec3");
    lua_setmetatable(l, -2);
    res[0] = a[0] - b[0];
    res[1] = a[1] - b[1];
    res[2] = a[2] - b[2];

    return 1;
}

i32 script_vec3_mul(lua_State* l) {
    f32* a = luaL_checkudata(l, 1, "pear.vec3");
    luaL_argcheck(l, a != NULL, 1, "'vec3' expected");
    f32 b = luaL_checknumber(l, 2);

    f32* res = (f32*)lua_newuserdata(l, sizeof(vec3));
    luaL_getmetatable(l, "pear.vec3");
    lua_setmetatable(l, -2);
    res[0] = a[0] * b;
    res[1] = a[1] * b;
    res[2] = a[2] * b;

    return 1;
}

i32 script_vec3_dot(lua_State* l) {
    f32* a = luaL_checkudata(l, 1, "pear.vec3");
    luaL_argcheck(l, a != NULL, 1, "'vec3' expected");
    f32* b = luaL_checkudata(l, 2, "pear.vec3");
    luaL_argcheck(l, b != NULL, 2, "'vec3' expected");

    f32 res = glm_vec3_dot(a, b);
    lua_pushnumber(l, res);

    return 1;
}

void script_vec3_set_metamethod(lua_State* l, const char* name, lua_CFunction func) {
    lua_pushstring(l, name);
    lua_pushcfunction(l, func);
    lua_settable(l, -3);
}

void script_init_vec3(script_t* script) {
    lua_State* l = script_get_state(script);

    luaL_newmetatable(l, "pear.vec3");

    script_vec3_set_metamethod(l, "__index", script_vec3_index);
    script_vec3_set_metamethod(l, "__newindex", script_vec3_newindex);
    script_vec3_set_metamethod(l, "__tostring", script_vec3_tostring);
    script_vec3_set_metamethod(l, "__add", script_vec3_add);
    script_vec3_set_metamethod(l, "__sub", script_vec3_sub);
    script_vec3_set_metamethod(l, "__mul", script_vec3_mul);
    script_vec3_set_metamethod(l, "dot", script_vec3_dot);

    lua_setglobal(l, "vec3");
}
