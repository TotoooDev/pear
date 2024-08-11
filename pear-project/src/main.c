#include <core/log.h>
#include <core/app.h>
#include <scene/node.h>
#include <scene/types/mesh_3d.h>
#include <scene/types/camera_3d.h>
#include <graphics/window.h>
#include <graphics/image.h>
#include <graphics/texture.h>
#include <graphics/framebuffer.h>
#include <event/event_dispatcher.h>
#include <event/keyboard.h>
#include <fs/loaders/image_loader.h>
#include <pear-3d.h>
#include <stdlib.h>

#define NK_NO_STB_RECT_PACK_IMPLEMENTATION
#define NK_NO_STB_TRUETYPE_IMPLEMENTATION
#define NK_IMPLEMENTATION
#include <graphics/renderer.h>

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

void nk(Renderer* renderer, struct nk_context* nk_context, void* user_data) {
    if (nk_begin(nk_context, "coucou", nk_rect(50, 50, 250, 250), renderer_nk_default_window_flags)) {
        nk_layout_row_dynamic(nk_context, 0, 1);
        if (nk_button_label(nk_context, "click me")) {
            PEAR_INFO("coucou!");
        }
    }
    nk_end(nk_context);
}

int main(int argc, char* argv[]) {
    app_init();

    bool success;
    Pear3D pear3d = pear3d_load("backpack.pear3d", &success);
    if (!success) {
        PEAR_ERROR("failed to load model!");
        return 1;
    }

    Model* model = model_new_from_pear3d(pear3d);

    Model3DCreationInfo model3d_info;
    model3d_info.model = model;

    renderer_nk_add_gui(nk, NULL);

    Camera3DCreationInfo cam3d_info;
    cam3d_info.pos[0] = 0.0f;
    cam3d_info.pos[1] = 0.0f;
    cam3d_info.pos[2] = 4.0f;
    cam3d_info.yaw = -90.0f;
    cam3d_info.pitch = 0.0f;
    cam3d_info.roll = 0.0f;

    Node* parent = node_new(NODE_TYPE_CONTAINER, NULL, "parent", NULL, NULL);
    Node* cam = node_new(NODE_TYPE_CAMERA_3D, parent, "cam", &cam3d_info, NULL);
    event_subscribe(cam_on_event, cam);
    Node* model_node = node_new(NODE_TYPE_MODEL_3D, parent, "model", &model3d_info, NULL);

    node_add_son(parent, cam);
    node_add_son(parent, model_node);

    app_set_root_node(parent);
    app_run();

    node_recursive_delete(parent);

    PEAR_INFO("goodbye!");

    return 0;
}
