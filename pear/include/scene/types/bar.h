#ifndef PEAR_SCENE_TYPE_BAR_H_
#define PEAR_SCENE_TYPE_BAR_H_

#include <core/types.h>

typedef struct bar_creation_data_t {
    u32 value;
} bar_creation_data_t;

typedef struct bar_t bar_t;

bar_t* bar_new(bar_creation_data_t* creation_data);
void bar_delete(bar_t* bar);

void bar_update(bar_t* bar, f32 timestep);
void bar_set_value(bar_t* bar, u32 value);
u32 bar_get_value(bar_t* bar);

#endif
