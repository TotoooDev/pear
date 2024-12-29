#ifdef PEAR_PLATFORM_OPENGL

#include <graphics/platform/opengl/shader.h>
#include <util/filesystem.h>
#include <core/log.h>
#include <GL/glew.h>
#include <core/alloc.h>

#define SHADER_INFO_LOG_LENGTH 1024

typedef struct shader_t {
    u32 id;
} shader_t;

shader_t* shader_new(const char* vertex_source, const char* fragment_source) {
    shader_t* shader = (shader_t*)PEAR_MALLOC(sizeof(shader_t));

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

shader_t* shader_new_from_file(const char* vertex_path, const char* fragment_path) {
    char* vertex_source = filesystem_read_file(vertex_path);
    char* fragment_source = filesystem_read_file(fragment_path);

    shader_t* shader = shader_new(vertex_source, fragment_source);

    PEAR_FREE(vertex_source);
    PEAR_FREE(fragment_source);

    return shader;
}

void shader_delete(shader_t* shader) {
    glDeleteProgram(shader->id);
    PEAR_FREE(shader);
}

void shader_use(shader_t* shader) {
    glUseProgram(shader->id);
}

void shader_set_u32(shader_t* shader, u32 value, const char* name) {
    glUniform1ui(glGetUniformLocation(shader->id, name), value);
}

void shader_set_i32(shader_t* shader, i32 value, const char* name) {
    glUniform1i(glGetUniformLocation(shader->id, name), value);
}

void shader_set_f32(shader_t* shader, f32 value, const char* name) {
    glUniform1f(glGetUniformLocation(shader->id, name), value);
}

void shader_set_vec3(shader_t* shader, vec3 value, const char* name) {
    glUniform3fv(glGetUniformLocation(shader->id, name), 1, value);
}

void shader_set_vec4(shader_t* shader, vec4 value, const char* name) {
    glUniform4fv(glGetUniformLocation(shader->id, name), 1, value);
}

void shader_set_mat4(shader_t* shader, mat4 value, const char* name) {
    glUniformMatrix4fv(glGetUniformLocation(shader->id, name), 1, false, (f32*)value);
}

void shader_set_ubo(shader_t* shader, ubo_t* ubo, const char* name) {
    ubo_use(ubo);
    u32 index = glGetUniformBlockIndex(shader->id, name);
    glUniformBlockBinding(shader->id, index, ubo_get_binding_point(ubo));
}

#endif
