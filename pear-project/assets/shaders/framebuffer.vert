#version 330 core

layout (location = 0) in vec2 a_pos;
layout (location = 1) in vec2 a_texture_coords;

out vec2 texture_coords;

void main() {
    gl_Position = vec4(a_pos, 0.0, 1.0);
    texture_coords = a_texture_coords;
}
