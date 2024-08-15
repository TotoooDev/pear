#include <core/log.h>
#include <core/app.h>
#include <core/gui.h>
#include <core/config.h>
#include <scene/node.h>
#include <scene/types/mesh_3d.h>
#include <scene/types/camera_3d.h>
#include <event/event_dispatcher.h>
#include <event/keyboard.h>
#include <pear-3d.h>

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

void general_info_gui(struct nk_context* nk_context, void* user_data) {
    if (nk_begin(nk_context, "general info", nk_rect(100, 50, 250, 250), gui_default_window_flags)) {
        nk_layout_row_dynamic(nk_context, 0, 1);
        nk_label(nk_context, "platform: " PEAR_PLATFORM_NAME, NK_TEXT_ALIGN_LEFT);
        nk_label(nk_context, "graphics api: " PEAR_RENDERER_API_NAME, NK_TEXT_ALIGN_LEFT);
        nk_labelf(nk_context, NK_TEXT_ALIGN_LEFT, "debug mode: %s", PEAR_IS_DEBUG == true ? "true" : "false");
        nk_labelf(nk_context, NK_TEXT_ALIGN_LEFT, "timestep: %f", app_get_timestep());
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

    gui_add(general_info_gui, NULL);

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
