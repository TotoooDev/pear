#ifndef PEAR_SCENE_TYPE_FOO_H_
#define PEAR_SCENE_TYPE_FOO_H_

#include <core/types.h>

typedef struct foo_creation_data_t {
    u32 value;
} foo_creation_data_t;

typedef struct foo_t foo_t;

foo_t* foo_new(foo_creation_data_t* creation_data);
void foo_delete(foo_t* foo);

void foo_update(foo_t* foo, f32 timestep);
void foo_set_value(foo_t* foo, u32 value);
u32 foo_get_value(foo_t* foo);

#endif
