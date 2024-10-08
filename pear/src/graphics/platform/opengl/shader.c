#include <graphics/platform/opengl/shader.h>
#include <graphics/platform/opengl/texture.h>
#include <core/log.h>
#include <GL/glew.h>
#include <stdlib.h>

#define SHADER_INFO_LOG_LENGTH 1024

typedef struct Shader {
    u32 id;
} Shader;

Shader* shader_new(const char* vertex_source, const char* fragment_source) {
    Shader* shader = (Shader*)malloc(sizeof(Shader));

    u32 success;
    char info_log[SHADER_INFO_LOG_LENGTH];
    u32 vertex_shader, fragment_shader;
    
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_source, NULL);
    glCompileShader(vertex_shader);
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex_shader, SHADER_INFO_LOG_LENGTH, NULL, info_log);
        PEAR_ERROR("failed to compile vertex shader! opengl error: %s", info_log);
    }

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_source, NULL);
    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment_shader, SHADER_INFO_LOG_LENGTH, NULL, info_log);
        PEAR_ERROR("failed to compile fragment shader! opengl error: %s", info_log);
    }

    shader->id = glCreateProgram();
    glAttachShader(shader->id, vertex_shader);
    glAttachShader(shader->id, fragment_shader);
    glLinkProgram(shader->id);
    glGetProgramiv(shader->id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader->id, SHADER_INFO_LOG_LENGTH, NULL, info_log);
        PEAR_ERROR("failed to link shader! opengl error: %s", info_log);
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return shader;
}

void shader_delete(Shader* shader) {
    glDeleteProgram(shader->id);
    free(shader);
}

void shader_use(Shader* shader) {
    glUseProgram(shader->id);
}

void shader_set_u32(Shader* shader, u32 value, const char* name) {
    glUniform1ui(glGetUniformLocation(shader->id, name), value);
}

void shader_set_i32(Shader* shader, i32 value, const char* name) {
    glUniform1i(glGetUniformLocation(shader->id, name), value);
}

void shader_set_f32(Shader* shader, f32 value, const char* name) {
    glUniform1f(glGetUniformLocation(shader->id, name), value);
}

void shader_set_vec3(Shader* shader, vec3 value, const char* name) {
    glUniform3fv(glGetUniformLocation(shader->id, name), 1, value);
}

void shader_set_vec4(Shader* shader, vec4 value, const char* name) {
    glUniform4fv(glGetUniformLocation(shader->id, name), 1, value);
}

void shader_set_mat4(Shader* shader, mat4 value, const char* name) {
    glUniformMatrix4fv(glGetUniformLocation(shader->id, name), 1, false, (f32*)value);
}
