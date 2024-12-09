#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_texture_coords;

out VS_OUT {
    vec2 texture_coords;
} vs_out;

layout (std140) uniform ubo_matrices {
    mat4 u_model;
    mat4 u_view;
    mat4 u_projection;
};

void main() {
    gl_Position = u_projection * u_view * u_model * vec4(a_pos, 1.0f);
    vs_out.texture_coords = a_texture_coords;
}
