#version 330 core

out vec4 frag_color;
  
in vec2 texture_coords;

uniform sampler2D u_screen_texture;

void main() { 
    frag_color = texture(u_screen_texture, texture_coords);

    // gamma correction
    float gamma = 2.2;
    frag_color.rgb = pow(frag_color.rgb, vec3(1.0 / gamma));
}