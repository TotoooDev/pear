#ifdef PEAR_PLATFORM_OPENGL

#ifndef PEAR_GRAPHICS_OPENGL_UBO_INFO_H_
#define PEAR_GRAPHICS_OPENGL_UBO_INFO_H_

#include <core/types.h>

typedef struct ubo_info_t ubo_info_t;

ubo_info_t* uboinfo_new();
void uboinfo_delete(ubo_info_t* info);

void uboinfo_add_i32(ubo_info_t* info, i32 data);

#endif

#endif
