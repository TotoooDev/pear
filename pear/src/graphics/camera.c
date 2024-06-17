#include <graphics/camera.h>

static vec3 camera_world_up = { 0.0f, 1.0f, 0.0f };

void camera_get_view_matrix(vec3 pos, f32 yaw, f32 pitch, f32 roll, mat4 dest) {
    vec3 front;
    vec3 up;
    vec3 right;

    front[0] = cos(glm_rad(yaw)) * cos(glm_rad(pitch));
    front[1] = sin(glm_rad(pitch));
    front[2] = sin(glm_rad(yaw)) * cos(glm_rad(pitch));

    glm_vec3_normalize(front);

    glm_cross(front, camera_world_up, right);
    glm_normalize(right);
    
    glm_cross(right, front, up);
    glm_normalize(up);

    mat4 roll_matrix;
    mat4 identity;
    glm_mat4_identity(identity);
    glm_rotate(identity, glm_rad(roll), front);
    glm_vec3_rotate_m4(roll_matrix, up, up);

    vec3 direction;
    glm_vec3_add(pos, front, direction);

    glm_lookat(pos, direction, up, dest);
}
