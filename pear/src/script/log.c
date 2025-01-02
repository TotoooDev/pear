#include <script/log.h>
#include <core/log.h>

i32 script_log_info(lua_State* l) {
    const char* str = lua_tostring(l, 1);
    PEAR_INFO("[LUA] %s", str);
    return 0;
}

i32 script_log_warn(lua_State* l) {
    const char* str = lua_tostring(l, 1);
    PEAR_WARN("[LUA] %s", str);
    return 0;
}

i32 script_log_error(lua_State* l) {
    const char* str = lua_tostring(l, 1);
    PEAR_ERROR("[LUA] %s", str);
    return 0;
}
