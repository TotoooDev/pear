#version 330 core

out vec4 frag_color;

in VS_OUT {
    vec2 texture_coords;
} fs_in;

uniform sampler2D u_texture;

void main() {
    // frag_color = texture(u_texture, fs_in.texture_coords);
    frag_color = vec4(fs_in.texture_coords, 0.0, 1.0);
}
