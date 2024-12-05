#ifdef PEAR_PLATFORM_OPENGL

#include <graphics/platform/opengl/ubo_info.h>
#include <core/alloc.h>

typedef struct ubo_info_t {
    u32 buffer_size;
    u32 last_base_alignment;
    u32 last_aligned_offset;
} ubo_info_t;

void uboinfo_add_element(ubo_info_t* info, u32 base_alignment) {
    info->last_aligned_offset += info->last_base_alignment;
    if (info->last_aligned_offset % base_alignment != 0) {
        info->last_aligned_offset += base_alignment - ((info->last_aligned_offset) % base_alignment);
    }

    info->buffer_size = info->last_aligned_offset + base_alignment;
    info->last_base_alignment = base_alignment;
}

ubo_info_t* uboinfo_new() {
    ubo_info_t* info = (ubo_info_t*)PEAR_MALLOC(sizeof(ubo_info_t));
    
    info->buffer_size = 0;
    info->last_base_alignment = 0;
    info->last_aligned_offset = 0;
    
    return info;
}

void uboinfo_delete(ubo_info_t* info) {
    PEAR_FREE(info);
}

void uboinfo_add_i32(ubo_info_t* info, i32 data) {
    uboinfo_add_element(info, 4);
}

/*
{
    // type   // base alignment  // aligned offset
    float     // N = 4           // 0
    vec3      // 4N = 16         // 0 + 4 = 4 -> 16
    mat4      // 4N = 16         // 16 + 16 = 32
              // 4N = 16         // 32 + 16 = 48
              // 4N = 16         // 48 + 16 = 64
              // 4N = 16         // 64 + 16 = 80
    float[3]  // 4N = 16         // 80 + 16 = 96
              // 4N = 16         // 96 + 16 = 112
              // 4N = 16         // 112 + 16 = 128
    bool      // N = 4           // 128 + 16 = 144
    int       // N = 4           // 132 + 4 = 148
    vec3      // 4N = 16         // 148 + 4 = 152 -> 160
}

Last_aligned_offset = Last_aligned_offset + Last_base_alignment
If Last_aligned_offset % Base_alignment != 0
    Last_aligned_offset += (Last_aligned_offset + Base_alignment) + (Base_alignment - ((Last_aligned_offset + Base_alignment) % Base_alignment))
Size = Last_aligned_offset + Base_alignment
Last_base_alignment = Base_alignment

148 % 16 = 4
148 + (16 - (148 % 16)) = 160 // ok

152 + (16 - (152 % 16)) = 160 // ok
*/

#endif
