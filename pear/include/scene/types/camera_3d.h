#ifndef PEAR_NODE_CAMERA3D_H
#define PEAR_NODE_CAMERA3D_H

#include <core/types.h>

typedef struct Camera3DCreationInfo {
    vec3 pos;
    f32 yaw;
    f32 pitch;
    f32 roll;
} Camera3DCreationInfo;

typedef struct Camera3D Camera3D;

Camera3D* camera3d_new(Camera3DCreationInfo* creation_info);
void camera3d_delete(Camera3D* cam);

void camera3d_set_pos(Camera3D* cam, vec3 pos);
void camera3d_set_yaw(Camera3D* cam, f32 yaw);
void camera3d_set_pitch(Camera3D* cam, f32 pitch);
void camera3d_set_roll(Camera3D* cam, f32 roll);
void camera3d_get_view_matrix(Camera3D* cam, mat4 dest);

#endif
