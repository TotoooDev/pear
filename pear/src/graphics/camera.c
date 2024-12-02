#include <graphics/camera.h>

camera_t camera_new_at_origin() {
    return (camera_t){
        .pos = { 0.0f, 0.0f, 0.0f },
        .yaw = -90.0f,
        .pitch = 0.0f
    };
}

void camera_get_view_matrix(camera_t camera, mat4 dest) {
    vec3 front;
    vec3 up;
    vec3 right;

    front[0] = cos(glm_rad(camera.yaw)) * cos(glm_rad(camera.pitch));
    front[1] = sin(glm_rad(camera.pitch));
    front[2] = sin(glm_rad(camera.yaw)) * cos(glm_rad(camera.pitch));

    glm_vec3_normalize(front);

    vec3 world_up = { 0.0f, 1.0f, 0.0f };
    glm_cross(front, world_up, right);
    glm_normalize(right);
    
    glm_cross(right, front, up);
    glm_normalize(up);

    vec3 direction;
    glm_vec3_add(camera.pos, front, direction);

    glm_lookat(camera.pos, direction, up, dest);
}
