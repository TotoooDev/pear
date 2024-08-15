#ifndef CAM_H
#define CAM_H

#include <scene/node.h>
#include <scene/types/camera_3d.h>
#include <event/event_dispatcher.h>
#include <event/keyboard.h>

void cam_on_event(EventType type, void* e, void* user_data) {
    Node* node = (Node*)user_data;
    Camera3D* cam = (Camera3D*)node_get_data(node);

    vec3 pos;
    camera3d_get_pos(cam, pos);

    if (type == EVENT_TYPE_KEY_DOWN) {
        KeyDownEvent* event = (KeyDownEvent*)e;
        if (event->key == PEAR_KEY_DOWN) {
            pos[2]++;
            camera3d_set_pos(cam, pos);
        }
        if (event->key == PEAR_KEY_UP) {
            pos[2]--;
            camera3d_set_pos(cam, pos);
        }
    }

    if (type == EVENT_TYPE_MOUSE_MOVED) {
        MouseMovedEvent* event = (MouseMovedEvent*)e;
        // PEAR_INFO("mouse pos: %f, %f", event->x, event->y);
    }
}

Node* cam_new(Node* parent) {
    Camera3DCreationInfo cam3d_info;
    cam3d_info.pos[0] = 0.0f;
    cam3d_info.pos[1] = 0.0f;
    cam3d_info.pos[2] = 4.0f;
    cam3d_info.yaw = -90.0f;
    cam3d_info.pitch = 0.0f;
    cam3d_info.roll = 0.0f;

    Node* cam = node_new(NODE_TYPE_CAMERA_3D, parent, "cam", &cam3d_info, NULL);
    event_subscribe(cam_on_event, cam);

    return cam;
}

#endif
