#version 330 core

out vec4 frag_color;

in vec3 color;
in vec2 texture_coords;

uniform sampler2D u_texture;
uniform vec3 u_color;

void main() {
    // frag_color = vec4(u_color, 1.0);
    frag_color = texture(u_texture, texture_coords);
    // frag_color = vec4(texture_coords, 0.0, 1.0);
}
