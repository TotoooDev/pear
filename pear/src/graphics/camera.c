#include <graphics/camera.h>

void camera_get_view_matrix(vec3 pos, f32 yaw, f32 pitch, f32 roll, mat4 dest) {
    vec3 front;
    vec3 up;
    vec3 right;

    front[0] = cos(glm_rad(yaw)) * cos(glm_rad(pitch));
    front[1] = sin(glm_rad(pitch));
    front[2] = sin(glm_rad(yaw)) * cos(glm_rad(pitch));

    glm_vec3_normalize(front);

    vec3 world_up = { 0.0f, 1.0f, 0.0f };
    glm_cross(front, world_up, right);
    glm_normalize(right);
    
    glm_cross(right, front, up);
    glm_normalize(up);

    vec3 direction;
    glm_vec3_add(pos, front, direction);

    glm_lookat(pos, direction, up, dest);
}
