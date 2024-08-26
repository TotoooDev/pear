#include <core/log.h>
#include <core/app.h>
#include <core/gui/gui.h>
#include <core/gui/node_hierarchy.h>
#include <core/gui/general_info.h>
#include <core/gui/material_inspector.h>
#include <core/config.h>
#include <scene/node.h>
#include <scene/types/model_3d.h>
#include <scene/types/light_3d.h>
#include <event/event_dispatcher.h>
#include <event/keyboard.h>
#include <pear-3d.h>

#include <graphics/material.h>

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

    Model3DCreationInfo model3d_info = {
        .model = model,
        .transform = {
            .pos = { 0.0f, 0.0f, 0.0f },
            .rotation = { 0.0f, 0.0f, 0.0f },
            .scale = { 1.0f, 1.0f, 1.0f }
        }
    };

    Light light = {
        .type = LIGHT_TYPE_DIRECTIONAL,
        .ambient = { 0.1f, 0.1f, 0.1f },
        .diffuse = { 0.5f, 0.5f, 0.5f },
        .specular = { 1.0f, 1.0f, 1.0f }
    };
    Light3DCreationInfo light_info = {
        .light = light,
        .direction = { 0.0f, -1.0f, 0.0f }
    };

    Node* parent = node_new(NODE_TYPE_CONTAINER, NULL, "parent", NULL, NULL);
    Node* model_node = node_new(NODE_TYPE_MODEL_3D, parent, "model", &model3d_info, NULL);
    Node* light_node = node_new(NODE_TYPE_LIGHT_3D, parent, "light", &light_info, NULL);

    node_add_son(parent, cam_new(parent));
    node_add_son(parent, light_node);
    node_add_son(parent, model_node);

    gui_add(gui_general_info, NULL);
    gui_add(gui_node_hierarchy, (void*)parent);
    gui_add(gui_material_inspector, NULL);

    material_add(material_get_default());

    app_set_root_node(parent);
    app_run();

    node_recursive_delete(parent);

    PEAR_INFO("goodbye!");

    return 0;
}
