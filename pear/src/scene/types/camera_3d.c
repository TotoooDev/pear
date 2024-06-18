#include <scene/types/camera_3d.h>
#include <graphics/camera.h>
#include <stdlib.h>

typedef struct Camera3D {
    vec3 pos;
    f32 yaw;
    f32 pitch;
    f32 roll;
} Camera3D;

Camera3D* camera3d_new(Camera3DCreationInfo* creation_info) {
    Camera3D* cam = (Camera3D*)malloc(sizeof(Camera3D));
    
    glm_vec3_copy(creation_info->pos, cam->pos);
    cam->yaw = creation_info->yaw;
    cam->pitch = creation_info->pitch;
    cam->roll = creation_info->roll;

    return cam;
}

void camera3d_delete(Camera3D* cam) {
    free(cam);
}

void camera3d_set_pos(Camera3D* cam, vec3 pos) {
    glm_vec3_copy(pos, cam->pos);
}

void camera3d_set_yaw(Camera3D* cam, f32 yaw) {
    cam->yaw = yaw;
}

void camera3d_set_pitch(Camera3D* cam, f32 pitch) {
    cam->pitch = pitch;

}

void camera3d_set_roll(Camera3D* cam, f32 roll) {
    cam->roll = roll;
}

void camera3d_get_pos(Camera3D* cam, vec3 dest) {
    glm_vec3_copy(cam->pos, dest);
}

f32 camera3d_get_yaw(Camera3D* cam) {
    return cam->yaw;
}

f32 camera3d_get_pitch(Camera3D* cam) {
    return cam->pitch;
}

f32 camera3d_get_roll(Camera3D* cam) {
    return cam->roll;
}

void camera3d_get_view_matrix(Camera3D* cam, mat4 dest) {
    camera_get_view_matrix(cam->pos, cam->yaw, cam->pitch, cam->roll, dest);
}
