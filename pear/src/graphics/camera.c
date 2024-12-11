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

void camera_get_front(f32 yaw, f32 pitch, f32 roll, vec3 dest) {
    dest[0] = cos(glm_rad(yaw)) * cos(glm_rad(pitch));
    dest[1] = sin(glm_rad(pitch));
    dest[2] = sin(glm_rad(yaw)) * cos(glm_rad(pitch));
}

void camera_get_right(f32 yaw, f32 pitch, f32 roll, vec3 dest) {
    vec3 front;
    vec3 world_up = { 0.0f, 1.0f, 0.0f };
    camera_get_front(yaw, pitch, roll, front);
    glm_cross(front, world_up, dest);
    glm_normalize(dest);
}

void camera_get_up(f32 yaw, f32 pitch, f32 roll, vec3 dest) {
    vec3 front;
    vec3 right;
    camera_get_front(yaw, pitch, roll, front);
    camera_get_right(yaw, pitch, roll, right);
    glm_cross(right, front, dest);
    glm_normalize(dest);
}
