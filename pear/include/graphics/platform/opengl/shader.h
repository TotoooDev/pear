#ifndef PEAR_OPENGL_SHADER_H
#define PEAR_OPENGL_SHADER_H

#include <graphics/texture.h>
#include <core/types.h>

typedef struct Shader Shader;

Shader* shader_new(const char* vertex_source, const char* fragment_source);
void shader_delete(Shader* shader);

void shader_use(Shader* shader);

void shader_set_u32(Shader* shader, u32 value, const char* name);
void shader_set_i32(Shader* shader, i32 value, const char* name);
void shader_set_f32(Shader* shader, f32 value, const char* name);
void shader_set_vec3(Shader* shader, vec3 value, const char* name);
void shader_set_vec4(Shader* shader, vec4 value, const char* name);
void shader_set_mat4(Shader* shader, mat4 value, const char* name);

#endif
