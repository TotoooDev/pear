#include <core/gui/node_hierarchy.h>
#include <scene/types/container.h>
#include <scene/types/camera_3d.h>
#include <scene/types/model_3d.h>
#include <scene/types/mesh_3d.h>
#include <graphics/material.h>
#include <limits.h>

static Node* gui_root = NULL;

void gui_model_3d(struct nk_context* nk_context, Model3D* data) {
    vec3 pos, rotation, scale;
    model3d_get_position(data, pos);
    model3d_get_rotation(data, rotation);
    model3d_get_scale(data, scale);

    nk_layout_row_dynamic(nk_context, 16, 4);

    nk_label(nk_context, "position:", NK_TEXT_ALIGN_LEFT);
    nk_property_float(nk_context, "#x", -FLT_MAX, &(pos[0]), FLT_MAX, 1.0f, 0.01f);
    nk_property_float(nk_context, "#y", -FLT_MAX, &(pos[1]), FLT_MAX, 1.0f, 0.01f);
    nk_property_float(nk_context, "#z", -FLT_MAX, &(pos[2]), FLT_MAX, 1.0f, 0.01f);

    nk_label(nk_context, "rotation:", NK_TEXT_ALIGN_LEFT);
    nk_property_float(nk_context, "#x", -FLT_MAX, &(rotation[0]), FLT_MAX, 1.0f, 0.01f);
    nk_property_float(nk_context, "#y", -FLT_MAX, &(rotation[1]), FLT_MAX, 1.0f, 0.01f);
    nk_property_float(nk_context, "#z", -FLT_MAX, &(rotation[2]), FLT_MAX, 1.0f, 0.01f);

    nk_label(nk_context, "scale:", NK_TEXT_ALIGN_LEFT);
    nk_property_float(nk_context, "#x", -FLT_MAX, &(scale[0]), FLT_MAX, 1.0f, 0.01f);
    nk_property_float(nk_context, "#y", -FLT_MAX, &(scale[1]), FLT_MAX, 1.0f, 0.01f);
    nk_property_float(nk_context, "#z", -FLT_MAX, &(scale[2]), FLT_MAX, 1.0f, 0.01f);

    model3d_set_position(data, pos);
    model3d_set_rotation(data, rotation);
    model3d_set_scale(data, scale);
}

void gui_mesh_3d(struct nk_context* nk_context, Mesh3D* data) {
    Mesh* mesh = mesh3d_get_mesh(data);
    u32 material_index = mesh_get_material_index(mesh);

    vec3 pos, rotation, scale;
    mesh3d_get_position(data, pos);
    mesh3d_get_rotation(data, rotation);
    mesh3d_get_scale(data, scale);

    nk_layout_row_dynamic(nk_context, 16, 4);

    nk_label(nk_context, "position:", NK_TEXT_ALIGN_LEFT);
    nk_property_float(nk_context, "#x", -FLT_MAX, &(pos[0]), FLT_MAX, 1.0f, 0.01f);
    nk_property_float(nk_context, "#y", -FLT_MAX, &(pos[1]), FLT_MAX, 1.0f, 0.01f);
    nk_property_float(nk_context, "#z", -FLT_MAX, &(pos[2]), FLT_MAX, 1.0f, 0.01f);

    nk_label(nk_context, "rotation:", NK_TEXT_ALIGN_LEFT);
    nk_property_float(nk_context, "#x", -FLT_MAX, &(rotation[0]), FLT_MAX, 1.0f, 0.01f);
    nk_property_float(nk_context, "#y", -FLT_MAX, &(rotation[1]), FLT_MAX, 1.0f, 0.01f);
    nk_property_float(nk_context, "#z", -FLT_MAX, &(rotation[2]), FLT_MAX, 1.0f, 0.01f);

    nk_label(nk_context, "scale:", NK_TEXT_ALIGN_LEFT);
    nk_property_float(nk_context, "#x", -FLT_MAX, &(scale[0]), FLT_MAX, 1.0f, 0.01f);
    nk_property_float(nk_context, "#y", -FLT_MAX, &(scale[1]), FLT_MAX, 1.0f, 0.01f);
    nk_property_float(nk_context, "#z", -FLT_MAX, &(scale[2]), FLT_MAX, 1.0f, 0.01f);

    nk_layout_row_dynamic(nk_context, 16, 1);
    mesh_set_material_index(mesh, nk_propertyi(nk_context, "#material_index", 0, material_index, material_get_num_materials() - 1, 1, 0.1f));

    mesh3d_set_position(data, pos);
    mesh3d_set_rotation(data, rotation);
    mesh3d_set_scale(data, scale);
}

void gui_camera_3d(struct nk_context* nk_context, Camera3D* data) {
    vec3 pos;
    camera3d_get_pos(data, pos);

    nk_layout_row_dynamic(nk_context, 16, 4);
    nk_label(nk_context, "position:", NK_TEXT_ALIGN_LEFT);
    nk_property_float(nk_context, "x", -FLT_MAX, &(pos[0]), FLT_MAX, 1.0f, 0.01f);
    nk_property_float(nk_context, "y", -FLT_MAX, &(pos[1]), FLT_MAX, 1.0f, 0.01f);
    nk_property_float(nk_context, "z", -FLT_MAX, &(pos[2]), FLT_MAX, 1.0f, 0.01f);

    nk_layout_row_dynamic(nk_context, 16, 1);
    camera3d_set_yaw(data, nk_propertyf(nk_context, "yaw", -FLT_MAX, camera3d_get_yaw(data), FLT_MAX, 1.0f, 0.01f));
    camera3d_set_pitch(data, nk_propertyf(nk_context, "pitch", -FLT_MAX, camera3d_get_pitch(data), FLT_MAX, 1.0f, 0.01f));
    camera3d_set_roll(data, nk_propertyf(nk_context, "roll", -FLT_MAX, camera3d_get_roll(data), FLT_MAX, 1.0f, 0.01f));

    camera3d_set_pos(data, pos);
}

void gui_node_properties(Node* node, void* user_data, u32 counter) {
    struct nk_context* nk_context = (struct nk_context*) user_data;

    enum nk_collapse_states tree_state = NK_MAXIMIZED;
    if (node_get_type(node) == NODE_TYPE_MESH_3D)
        tree_state = NK_MINIMIZED;

    if (nk_tree_push_id(nk_context, NK_TREE_TAB, node_get_name(node), tree_state, counter)) {
        nk_layout_row_dynamic(nk_context, 16, 2);

        if (node_get_num_sons(node) > 0)
            nk_labelf(nk_context, NK_TEXT_ALIGN_LEFT, "%d sons", node_get_num_sons(node));
        else
            nk_label(nk_context, "leaf", NK_TEXT_ALIGN_LEFT);

        switch (node_get_type(node)) {
        case NODE_TYPE_CONTAINER:
            nk_label(nk_context, "type: container", NK_TEXT_ALIGN_LEFT);
            break;

        case NODE_TYPE_MODEL_3D:
            nk_label(nk_context, "type: model_3d", NK_TEXT_ALIGN_LEFT);
            Model3D* model_data = (Model3D*)node_get_data(node);
            gui_model_3d(nk_context, model_data);
            break;

        case NODE_TYPE_MESH_3D:
            nk_label(nk_context, "type: mesh_3d", NK_TEXT_ALIGN_LEFT);
            Mesh3D* mesh_data = (Mesh3D*)node_get_data(node);
            gui_mesh_3d(nk_context, mesh_data);
            break;
        
        case NODE_TYPE_CAMERA_3D:
            nk_label(nk_context, "type: camera_3d", NK_TEXT_ALIGN_LEFT);
            Camera3D* cam_data = (Camera3D*)node_get_data(node);
            gui_camera_3d(nk_context, cam_data);
            break;

        default:
            nk_label(nk_context, "type: unknown :O", NK_TEXT_ALIGN_LEFT);
            break;
        }

        for (u32 i = 0; i < node_get_num_sons(node); i++) {
            gui_node_properties(node_get_sons(node)[i], user_data, counter + i + 1);
        }

        nk_tree_pop(nk_context);
    }
}

void gui_node_hierarchy(struct nk_context* nk_context, void* user_data) {
    Node* root = gui_root == NULL ? (Node*)user_data : gui_root;

    if (nk_begin(nk_context, "node hierarchy", nk_rect(10, 230, 300, 300), gui_default_window_flags)) {
        gui_node_properties(root, nk_context, 0);
    }
    nk_end(nk_context);
}

void gui_node_hierarchy_set_root_node(Node* root) {
    gui_root = root;
}
