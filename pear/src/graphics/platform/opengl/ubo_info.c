#ifdef PEAR_PLATFORM_OPENGL

#include <graphics/platform/opengl/ubo_info.h>
#include <core/alloc.h>
#include <core/log.h>

#define UBO_INFO_NUM_MAX_ELEMENTS 4096

typedef struct ubo_info_t {
    u32 offsets[UBO_INFO_NUM_MAX_ELEMENTS];
    u32 num_elements;
    u32 buffer_size;

    u32 aligned_offset;
} ubo_info_t;

void uboinfo_add_element(ubo_info_t* info, u32 base_alignment, u32 data_size) {
    bool first_element = info->aligned_offset == 0;
    bool is_aligned = info->aligned_offset % 16 == 0;
    bool must_align = base_alignment == 16;

    if (!first_element && !is_aligned && must_align) {
        info->aligned_offset += 16 - info->aligned_offset % 16;
    }

    info->offsets[info->num_elements] = info->aligned_offset;
    info->num_elements++;

    info->aligned_offset += data_size;
    info->buffer_size = info->aligned_offset;
}

ubo_info_t* uboinfo_new() {
    ubo_info_t* info = (ubo_info_t*)PEAR_MALLOC(sizeof(ubo_info_t));
    
    info->num_elements = 0;
    info->buffer_size = 0;
    info->aligned_offset = 0;
    
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
    uboinfo_add_element(info, 4, 4);
}

void uboinfo_add_u32(ubo_info_t* info) {
    uboinfo_add_element(info, 4, 4);
}

void uboinfo_add_f32(ubo_info_t* info) {
    uboinfo_add_element(info, 4, 4);
}

void uboinfo_add_vec3(ubo_info_t* info) {
    uboinfo_add_element(info, 16, 12);
}

void uboinfo_add_vec4(ubo_info_t* info) {
    uboinfo_add_element(info, 16, 16);
}

void uboinfo_add_mat4(ubo_info_t* info) {
    uboinfo_add_element(info, 16, 16 * 4);
}

void uboinfo_pad_to_16_alignment(ubo_info_t* info) {
    bool is_aligned = info->aligned_offset % 16 == 0;
    if (!is_aligned) {
        info->aligned_offset += 16 - info->aligned_offset % 16;
    }

    info->buffer_size = info->aligned_offset;
}

#endif
