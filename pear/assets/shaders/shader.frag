#version 330 core

#define NUM_MAX_LIGHTS 128

out vec4 frag_color;

struct light_t {
    uint type;

    vec3 pos;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    float cutoff;
    float outer_cutoff;
};

in VS_OUT {
    vec2 texture_coords;
} fs_in;

layout (std140) uniform ubo_lights {
    uint u_num_lights;
    light_t u_lights[NUM_MAX_LIGHTS];
};

uniform sampler2D u_texture;

void main() {
    frag_color = texture(u_texture, fs_in.texture_coords) * vec4(u_lights[0].ambient, 1.0);
    // frag_color = vec4(fs_in.texture_coords, 0.0, 1.0);
    // frag_color = vec4(u_lights[0].pos, 1.0);
}
