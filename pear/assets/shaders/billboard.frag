#version 330 core

out vec4 frag_color;
  
in vec2 texture_coords;

uniform sampler2D u_texture;

void main() {
    vec4 color = texture(u_texture, texture_coords);
    if (color.a < 0.1) {
        discard;
    }
    frag_color = color;
}