#include <scene/types/foo.h>
#include <core/log.h>
#include <core/alloc.h>

typedef struct foo_t {
    u32 value;
} foo_t;

foo_t* foo_new(foo_creation_data_t* creation_data) {
    foo_t* foo = (foo_t*)PEAR_MALLOC(sizeof(foo_t));
    foo->value = creation_data->value;
    return foo;
}

void foo_delete(foo_t* foo) {
    PEAR_FREE(foo);
}

void foo_update(foo_t* foo, f32 timestep) {
    PEAR_INFO("foo: %f", timestep);
}

void foo_set_value(foo_t* foo, u32 value) {
    foo->value = value;
}

u32 foo_get_value(foo_t* foo) {
    return foo->value;
}
