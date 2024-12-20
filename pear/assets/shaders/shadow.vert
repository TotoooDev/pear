#version 330 core
layout (location = 0) in vec3 a_pos;

layout (std140) uniform ubo_matrices {
    mat4 u_model;
    mat4 u_view;
    mat4 u_projection;
    mat4 u_model_transpose_inverse;
    mat4 u_light_space_transform;
};

void main() {
    gl_Position = u_light_space_transform * u_model * vec4(a_pos, 1.0);
}
