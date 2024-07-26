#version 330 core
out vec4 frag_color;

in vec2 texture_coords;

uniform sampler2D u_albedo;

void main() {
    frag_color = texture(u_albedo, texture_coords);
}
