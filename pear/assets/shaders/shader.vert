#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_texture_coords;
layout (location = 2) in vec3 a_normal;

out VS_OUT {
    vec3 frag_pos;
    vec2 texture_coords;
    vec3 normal;
    vec4 frag_pos_light_space;
} vs_out;

layout (std140) uniform ubo_matrices {
    mat4 u_model;
    mat4 u_view;
    mat4 u_projection;
    mat4 u_model_transpose_inverse;
    mat4 u_light_space_transform;
};

void main() {
    vs_out.frag_pos = vec3(u_model * vec4(a_pos, 1.0));
    vs_out.texture_coords = a_texture_coords;
    vs_out.normal = mat3(u_model_transpose_inverse) * a_normal;
    vs_out.frag_pos_light_space = u_light_space_transform * vec4(vs_out.frag_pos, 1.0);

    gl_Position = u_projection * u_view * u_model * vec4(a_pos, 1.0f);
}
