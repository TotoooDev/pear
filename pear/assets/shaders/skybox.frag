#version 330 core

out vec4 frag_color;

in vec3 texture_coords;

uniform samplerCube u_cubemap;

void main() {
    frag_color = texture(u_cubemap, texture_coords);
}
