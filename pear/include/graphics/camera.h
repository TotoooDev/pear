#ifndef PEAR_GRAPHICS_CAMERA_H_
#define PEAR_GRAPHICS_CAMERA_H_

#include <core/types.h>

void camera_get_view_matrix(vec3 pos, f32 yaw, f32 pitch, f32 roll, mat4 dest);

#endif
