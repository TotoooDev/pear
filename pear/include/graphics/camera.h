#ifndef PEAR_CAMERA_H
#define PEAR_CAMERA_H

#include <core/types.h>

void camera_get_view_matrix(vec3 pos, f32 yaw, f32 pitch, f32 roll, mat4 dest);

#endif
