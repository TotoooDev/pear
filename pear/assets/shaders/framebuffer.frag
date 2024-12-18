#version 330 core

out vec4 frag_color;
  
in vec2 texture_coords;

uniform sampler2D u_screen_texture;

void main() { 
    frag_color = texture(u_screen_texture, texture_coords);
}