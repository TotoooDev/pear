#include <scene/types/bar.h>
#include <core/log.h>
#include <core/alloc.h>

typedef struct bar_t {
    u32 value;
} bar_t;

bar_t* bar_new(bar_creation_data_t* creation_data) {
    bar_t* bar = (bar_t*)PEAR_MALLOC(sizeof(bar_t));
    bar->value = creation_data->value;
    return bar;
}

void bar_delete(bar_t* bar) {
    PEAR_FREE(bar);
}

void bar_update(bar_t* bar, f32 timestep) {
    PEAR_INFO("bar: %d", bar->value);
}

void bar_set_value(bar_t* bar, u32 value) {
    bar->value = value;
}

u32 bar_get_value(bar_t* bar) {
    return bar->value;
}
