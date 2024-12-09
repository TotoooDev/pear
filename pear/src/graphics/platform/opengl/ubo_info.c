#ifdef PEAR_PLATFORM_OPENGL

#include <graphics/platform/opengl/ubo_info.h>
#include <core/alloc.h>
#include <core/log.h>

#define UBO_INFO_NUM_MAX_ELEMENTS 128

typedef struct ubo_info_t {
    u32 offsets[UBO_INFO_NUM_MAX_ELEMENTS];
    u32 num_elements;
    u32 buffer_size;
    u32 last_base_alignment;
    u32 last_aligned_offset;
} ubo_info_t;

// void uboinfo_add_element(ubo_info_t* info, u32 base_alignment) {
//     info->last_aligned_offset += info->last_base_alignment;
//     if (info->last_aligned_offset % base_alignment != 0) {
//         info->last_aligned_offset += base_alignment - ((info->last_aligned_offset) % base_alignment);
//     }

//     info->buffer_size = info->last_aligned_offset + base_alignment;
//     info->last_base_alignment = base_alignment;
    
//     info->offsets[info->num_elements] = info->last_aligned_offset;
//     info->num_elements++;
// }

void uboinfo_add_element(ubo_info_t* info, u32 base_alignment) {
    info->last_aligned_offset += info->last_base_alignment;
    if (base_alignment % 16 == 0 && info->last_aligned_offset % 16 != 0 && info->last_aligned_offset != 0 ) {
        info->last_aligned_offset += 12;
    }

    info->buffer_size = info->last_aligned_offset + base_alignment;
    info->last_base_alignment = base_alignment;
    
    info->offsets[info->num_elements] = info->last_aligned_offset;
    info->num_elements++;
}

ubo_info_t* uboinfo_new() {
    ubo_info_t* info = (ubo_info_t*)PEAR_MALLOC(sizeof(ubo_info_t));
    
    info->num_elements = 0;
    info->buffer_size = 0;
    info->last_base_alignment = 0;
    info->last_aligned_offset = 0;
    
    return info;
}

void uboinfo_delete(ubo_info_t* info) {
    PEAR_FREE(info);
}

void uboinfo_get_offsets(ubo_info_t* info, u32* offsets) {
    for (u32 i = 0; i < info->num_elements; i++) {
        offsets[i] = info->offsets[i];
    }
}

u32 uboinfo_get_num_elements(ubo_info_t* info) {
    return info->num_elements;
}

u32 uboinfo_get_buffer_size(ubo_info_t* info) {
    return info->buffer_size;
}

void uboinfo_add_i32(ubo_info_t* info) {
    uboinfo_add_element(info, 4);
}

void uboinfo_add_u32(ubo_info_t* info) {
    uboinfo_add_element(info, 4);
}

void uboinfo_add_f32(ubo_info_t* info) {
    uboinfo_add_element(info, 4);
}

void uboinfo_add_f32_array(ubo_info_t* info, u32 array_size) {
    uboinfo_add_element(info, 16 * array_size);
}

void uboinfo_add_vec3(ubo_info_t* info) {
    uboinfo_add_element(info, 16);
}

void uboinfo_add_vec4(ubo_info_t* info) {
    uboinfo_add_element(info, 16);
}

void uboinfo_add_mat4(ubo_info_t* info) {
    uboinfo_add_element(info, 16 * 4);
}

/*
{
    // type   // base alignment  // aligned offset        // size
    float     // N = 4           // 0                     // 4
    vec3      // 4N = 16         // 0 + 4 = 4 -> 16       // 32
    mat4      // 4N = 16         // 16 + 16 = 32          // 48
              // 4N = 16         // 32 + 16 = 48          // 64
              // 4N = 16         // 48 + 16 = 64          // 80
              // 4N = 16         // 64 + 16 = 80          // 96
    float[3]  // 4N = 16         // 80 + 16 = 96          // 112
              // 4N = 16         // 96 + 16 = 112         // 128
              // 4N = 16         // 112 + 16 = 128        // 144
    bool      // N = 4           // 128 + 16 = 144        // 148
    int       // N = 4           // 132 + 4 = 148         // 152
    vec3      // 4N = 16         // 148 + 4 = 152 -> 160  // 176
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
