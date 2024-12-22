#version 330 core

layout (location = 0) in vec3 a_pos;

out vec3 texture_coords;

layout (std140) uniform ubo_matrices {
    mat4 u_model;
    mat4 u_view;
    mat4 u_projection;
    mat4 u_model_transpose_inverse;
    mat4 u_light_space_transform;
};

void main() {
    texture_coords = a_pos;
    
    mat4 view = mat4(mat3(u_view));
    vec4 pos = u_projection * view * vec4(a_pos, 1.0);
    gl_Position = pos.xyww;
}
