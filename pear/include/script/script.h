#ifndef PEAR_SCRIPT_H_
#define PEAR_SCRIPT_H_

#include <script/vendor/lua.h>
#include <scene/entity.h>
#include <core/types.h>

typedef struct script_t script_t;

script_t* script_new(const char* script_str, entity_t* entity);
script_t* script_new_from_file(const char* filename, entity_t* entity);
void script_delete(script_t* script);

void script_set_path(script_t* script, const char* path);
char* script_get_path(script_t* script);

void script_on_start(script_t* script);
void script_on_update(script_t* script, f64 timestep);
void script_on_destroy(script_t* script);

void script_set_number(script_t* script, f64 number, const char* name);
void script_set_string(script_t* script, const char* str, const char* name);
void script_set_bool(script_t* script, bool boolean, const char* name);
void script_set_function(script_t* script, lua_CFunction function, const char* name);
void script_set_pointer(script_t* script, void* ptr, const char* name);
void script_set_vec3(script_t* script, vec3 vec, const char* name);
void script_begin_table(script_t* script, const char* name);
void script_end_table(script_t* script);

f64 script_get_number(script_t* script, const char* name);
const char* script_get_string(script_t* script, const char* name);
bool script_get_bool(script_t* script, const char* name);
void* script_get_pointer(script_t* script, const char* name);
void script_get_vec3(script_t* script, const char* name, vec3 dest);
void script_get_table(script_t* script, const char* name);
void script_end_table_read(script_t* script);

void script_dump_stack(script_t* script);

lua_State* script_get_state(script_t* script);

#ifdef PEAR_ENABLE_EDITOR
void script_init_editor(script_t* script);
void script_on_editor(script_t* script);
#endif

#endif
