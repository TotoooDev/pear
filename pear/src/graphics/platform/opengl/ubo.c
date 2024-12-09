#ifdef PEAR_PLATFORM_OPENGL

#include <graphics/platform/opengl/ubo.h>
#include <core/alloc.h>
#include <GL/glew.h>

typedef struct ubo_t {
    u32 id;
    u32 binding_point;

    u32* offsets;
    u32 num_elements;
} ubo_t;

static u32 ubo_num_buffers = 0;

ubo_t* ubo_new(ubo_info_t* info, bool dynamic) {
    ubo_t* ubo = (ubo_t*)PEAR_MALLOC(sizeof(ubo_t));

    ubo->binding_point = ubo_num_buffers;
    ubo->num_elements = uboinfo_get_num_elements(info);
    ubo->offsets = (u32*)PEAR_MALLOC(sizeof(u32) * ubo->num_elements);
    uboinfo_get_offsets(info, ubo->offsets);

    GLenum usage = dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;
    glGenBuffers(1, &ubo->id);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo->id);
    glBufferData(GL_UNIFORM_BUFFER, uboinfo_get_buffer_size(info), NULL, usage);
    glBindBufferBase(GL_UNIFORM_BUFFER, ubo_num_buffers, ubo->id);

    ubo_num_buffers++;

    return ubo;
}

void ubo_delete(ubo_t* ubo) {
    PEAR_FREE(ubo->offsets);
    PEAR_FREE(ubo);
}

void ubo_set_i32(ubo_t* ubo, u32 index, i32 value) {
    glBufferSubData(GL_UNIFORM_BUFFER, ubo->offsets[index], sizeof(i32), &value);
}

void ubo_set_u32(ubo_t* ubo, u32 index, u32 value) {
    glBufferSubData(GL_UNIFORM_BUFFER, ubo->offsets[index], sizeof(u32), &value);
} 

void ubo_set_f32(ubo_t* ubo, u32 index, f32 value) {
    glBufferSubData(GL_UNIFORM_BUFFER, ubo->offsets[index], sizeof(f32), &value);
}

void ubo_set_f32_array(ubo_t* ubo, u32 index, f32* value, u32 num_values) {
    glBufferSubData(GL_UNIFORM_BUFFER, ubo->offsets[index], sizeof(f32) * num_values, value);
}

void ubo_set_vec3(ubo_t* ubo, u32 index, vec3 value) {
    glBufferSubData(GL_UNIFORM_BUFFER, ubo->offsets[index], sizeof(vec3), value);
}

void ubo_set_vec4(ubo_t* ubo, u32 index, vec4 value) {
    glBufferSubData(GL_UNIFORM_BUFFER, ubo->offsets[index], sizeof(vec4), value);
}

void ubo_set_mat4(ubo_t* ubo, u32 index, mat4 value) {
    // glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(mat4), value);
    glBufferSubData(GL_UNIFORM_BUFFER, ubo->offsets[index], sizeof(mat4), value);
}

u32 ubo_get_binding_point(ubo_t* ubo) {
    return ubo->binding_point;
}

void ubo_use(ubo_t* ubo) {
    glBindBuffer(GL_UNIFORM_BUFFER, ubo->id);
}

#endif
