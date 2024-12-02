#ifndef PEAR_GRAPHICS_CAMERA_H_
#define PEAR_GRAPHICS_CAMERA_H_

#include <core/types.h>

typedef struct camera_t {
    vec3 pos;
    f32 yaw;
    f32 pitch;
} camera_t;

camera_t camera_new_at_origin();
void camera_get_view_matrix(camera_t camera, mat4 dest);

#endif
