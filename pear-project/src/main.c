#include <core/log.h>
#include <core/app.h>
#include <core/gui/gui.h>
#include <core/gui/node_hierarchy.h>
#include <core/gui/general_info.h>
#include <core/config.h>
#include <scene/node.h>
#include <scene/types/model_3d.h>
#include <event/event_dispatcher.h>
#include <event/keyboard.h>
#include <pear-3d.h>

#include <cam.h>

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

    model3d_info.pos[0] = 0.0f;
    model3d_info.pos[1] = 0.0f;
    model3d_info.pos[2] = 0.0f;

    model3d_info.rotation[0] = 0.0f;
    model3d_info.rotation[1] = 0.0f;
    model3d_info.rotation[2] = 0.0f;

    model3d_info.scale[0] = 1.0f;
    model3d_info.scale[1] = 1.0f;
    model3d_info.scale[2] = 1.0f;

    Node* parent = node_new(NODE_TYPE_CONTAINER, NULL, "parent", NULL, NULL);
    Node* model_node = node_new(NODE_TYPE_MODEL_3D, parent, "model", &model3d_info, NULL);

    node_add_son(parent, cam_new(parent));
    node_add_son(parent, model_node);

    gui_add(gui_general_info, NULL);
    gui_add(gui_node_hierarchy, (void*)parent);

    app_set_root_node(parent);
    app_run();

    node_recursive_delete(parent);

    PEAR_INFO("goodbye!");

    return 0;
}
