#version 330 core
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_texture_coords;
layout (location = 2) in vec3 a_normals;

uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_model;
uniform mat4 u_model_transpose_inverse;

out vec2 texture_coords;
out vec3 normal;
out vec3 frag_pos;

void main() {
    gl_Position = u_projection * u_view * u_model * vec4(a_pos, 1.0);

    frag_pos = vec3(u_model * vec4(a_pos, 1.0));
    normal = mat3(u_model_transpose_inverse) * a_normals;
}
