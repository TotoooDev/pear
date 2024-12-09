#ifdef PEAR_PLATFORM_OPENGL

#ifndef PEAR_GRAPHICS_OPENGL_SHADER_H_
#define PEAR_GRAPHICS_OPENGL_SHADER_H_

#include <graphics/platform/opengl/ubo.h>
#include <core/types.h>

typedef struct shader_t shader_t;

shader_t* shader_new(const char* vertex_source, const char* fragment_source);
void shader_delete(shader_t* shader);

void shader_use(shader_t* shader);
void shader_set_u32(shader_t* shader, u32 value, const char* name);
void shader_set_i32(shader_t* shader, i32 value, const char* name);
void shader_set_f32(shader_t* shader, f32 value, const char* name);
void shader_set_vec3(shader_t* shader, vec3 value, const char* name);
void shader_set_vec4(shader_t* shader, vec4 value, const char* name);
void shader_set_mat4(shader_t* shader, mat4 value, const char* name);
void shader_set_ubo(shader_t* shader, ubo_t* ubo, const char* name);

#endif

#endif
