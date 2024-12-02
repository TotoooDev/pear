#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_color;
layout (location = 2) in vec2 a_texture_coords;

out vec3 color;
out vec2 texture_coords;

void main() {
    color = a_color;
    texture_coords = a_texture_coords;
    gl_Position = vec4(a_pos, 1.0f);
}
