#version 330 core

out vec4 frag_color;

in vec3 color;
in vec2 texture_coords;

uniform sampler2D u_texture;

void main() {
    // frag_color = vec4(color, 1.0);
    frag_color = texture(u_texture, texture_coords);
}
