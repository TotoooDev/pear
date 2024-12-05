#ifdef PEAR_PLATFORM_OPENGL

#ifndef PEAR_GRAPHICS_OPENGL_UBO_H_
#define PEAR_GRAPHICS_OPENGL_UBO_H_

#include <core/types.h>

typedef struct ubo_t ubo_t;

ubo_t* ubo_new(void* data, u32 data_size);
void ubo_delete(ubo_t* ubo);

#endif

#endif
