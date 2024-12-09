#ifdef PEAR_PLATFORM_OPENGL

#ifndef PEAR_GRAPHICS_OPENGL_UBO_H_
#define PEAR_GRAPHICS_OPENGL_UBO_H_

#include <graphics/platform/opengl/ubo_info.h>
#include <core/types.h>

typedef struct ubo_t ubo_t;

ubo_t* ubo_new(ubo_info_t* info, bool dynamic);
void ubo_delete(ubo_t* ubo);

void ubo_set_i32(ubo_t* ubo, u32 index, i32 value);
void ubo_set_u32(ubo_t* ubo, u32 index, u32 value);
void ubo_set_f32(ubo_t* ubo, u32 index, f32 value);
void ubo_set_f32_array(ubo_t* ubo, u32 index, f32* value, u32 num_values);
void ubo_set_vec3(ubo_t* ubo, u32 index, vec3 value);
void ubo_set_vec4(ubo_t* ubo, u32 index, vec4 value);
void ubo_set_mat4(ubo_t* ubo, u32 index, mat4 value);

u32 ubo_get_binding_point(ubo_t* ubo);
void ubo_use(ubo_t* ubo);

#endif

#endif
