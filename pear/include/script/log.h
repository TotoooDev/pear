#ifndef PEAR_SCRIPT_LOG_H_
#define PEAR_SCRIPT_LOG_H_

#include <core/types.h>
#include <lua.h>

i32 script_log_info(lua_State* l);
i32 script_log_warn(lua_State* l);
i32 script_log_error(lua_State* l);

#endif
