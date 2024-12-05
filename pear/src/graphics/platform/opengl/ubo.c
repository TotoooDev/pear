#ifdef PEAR_PLATFORM_OPENGL

#include <graphics/platform/opengl/ubo.h>
#include <core/alloc.h>

typedef struct ubo_t {
    u32 foo;
} ubo_t;

ubo_t* ubo_new(void* data, u32 data_size) {
    ubo_t* ubo = (ubo_t*)PEAR_MALLOC(sizeof(ubo_t));
    return ubo;
}

void ubo_delete(ubo_t* ubo) {
    PEAR_FREE(ubo);
}

#endif
