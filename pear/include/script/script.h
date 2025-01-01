#ifndef PEAR_SCRIPT_H_
#define PEAR_SCRIPT_H_

#include <core/types.h>

typedef struct script_t script_t;

script_t* script_new(const char* script_str);
script_t* script_new_from_file(const char* filename);
void script_delete(script_t* script);

void script_on_start(script_t* script);
void script_on_update(script_t* script, f64 timestep);
void script_on_destroy(script_t* script);

void script_set_number(script_t* script, f64 number, const char* name);

#endif
