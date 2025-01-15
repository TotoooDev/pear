#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_texture_coords;

out vec2 texture_coords;

layout (std140) uniform ubo_matrices {
    mat4 u_model;
    mat4 u_view;
    mat4 u_projection;
    mat4 u_model_transpose_inverse;
    mat4 u_light_space_transform;
};

uniform vec3 u_billboard_pos;
uniform vec3 u_billboard_size;
uniform vec3 u_camera_right;
uniform vec3 u_camera_up;

void main() {
    texture_coords = a_texture_coords;

    vec3 billboard_pos_world_space =
        u_billboard_pos +
        u_camera_right * a_pos.x * u_billboard_size.x +
        u_camera_up * a_pos.y * u_billboard_size.y;

    gl_Position = u_projection * u_view * u_model * vec4(billboard_pos_world_space, 1.0);
}
