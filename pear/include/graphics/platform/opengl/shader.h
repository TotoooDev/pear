#ifndef PEAR_OPENGL_SHADER_H
#define PEAR_OPENGL_SHADER_H

#include <graphics/material.h>
#include <core/types.h>

#define SHADER_TEXTURE_ID_ALBEDO 0
#define SHADER_TEXTURE_ID_SPECULAR 1
#define SHADER_TEXTURE_ID_NORMAL 2

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
void shader_set_material(Shader* shader, Material* material, const char* name);

#endif
