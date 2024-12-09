#ifdef PEAR_PLATFORM_OPENGL

#ifndef PEAR_GRAPHICS_OPENGL_UBO_INFO_H_
#define PEAR_GRAPHICS_OPENGL_UBO_INFO_H_

#include <core/types.h>

typedef struct ubo_info_t ubo_info_t;

ubo_info_t* uboinfo_new();
void uboinfo_delete(ubo_info_t* info);

void uboinfo_get_offsets(ubo_info_t* info, u32* offsets);
u32 uboinfo_get_num_elements(ubo_info_t* info);
u32 uboinfo_get_buffer_size(ubo_info_t* info);

void uboinfo_add_i32(ubo_info_t* info);
void uboinfo_add_u32(ubo_info_t* info);
void uboinfo_add_f32(ubo_info_t* info);
void uboinfo_add_f32_array(ubo_info_t* info, u32 array_size);
void uboinfo_add_vec3(ubo_info_t* info);
void uboinfo_add_vec4(ubo_info_t* info);
void uboinfo_add_mat4(ubo_info_t* info);

#endif

#endif
