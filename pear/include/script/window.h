#ifndef SCRIPT_WINDOW_H_
#define SCRIPT_WINDOW_H_

#include <lua.h>
#include <lauxlib.h>
#include <graphics/window.h>
#include <core/app.h>
#include <core/types.h>

i32 script_window_lock_mouse(lua_State* l) {
    bool lock = lua_toboolean(l, 1);
    window_lock_mouse(app_get_window(), lock);
    return 0;
}

#endif
