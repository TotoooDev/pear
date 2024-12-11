#ifndef PEAR_GRAPHICS_CAMERA_H_
#define PEAR_GRAPHICS_CAMERA_H_

#include <core/types.h>

void camera_get_view_matrix(vec3 pos, f32 yaw, f32 pitch, f32 roll, mat4 dest);
void camera_get_front(f32 yaw, f32 pitch, f32 roll, vec3 dest);
void camera_get_right(f32 yaw, f32 pitch, f32 roll, vec3 dest);
void camera_get_up(f32 yaw, f32 pitch, f32 roll, vec3 dest);

#endif
