#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_color;
layout (location = 2) in vec2 a_texture_coords;

out vec3 color;
out vec2 texture_coords;

uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_model;

void main() {
    gl_Position = u_projection * u_view * u_model * vec4(a_pos, 1.0f);
    color = a_color;
    texture_coords = a_texture_coords;
}
