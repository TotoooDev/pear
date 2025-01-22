#ifndef PEAR_SCENE_COMPONENTS_CAMERA_COMPONENT_H_
#define PEAR_SCENE_COMPONENTS_CAMERA_COMPONENT_H_

#include <scene/component_attachment.h>
#include <core/types.h>

typedef struct camera_component_t {
    bool use;
} camera_component_t;

component_attachment_t cameracomponent_get_attachment();

#endif
