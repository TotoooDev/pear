#include <core/gui/node_hierarchy.h>
#include <scene/types/container.h>
#include <scene/types/camera_3d.h>
#include <scene/types/mesh_3d.h>

void gui_model_3d(struct nk_context* nk_context, Model3D* data) {
    vec3 pos, rotation, scale;
    model3d_get_position(data, pos);
    model3d_get_rotation(data, rotation);
    model3d_get_scale(data, scale);

    nk_label(nk_context, "type: model_3d", NK_TEXT_ALIGN_LEFT);
    nk_labelf(nk_context, NK_TEXT_ALIGN_LEFT, "position: %f, %f, %f", pos[0], pos[1], pos[2]);
    nk_labelf(nk_context, NK_TEXT_ALIGN_LEFT, "rotation: %f, %f, %f", rotation[0], rotation[1], rotation[2]);
    nk_labelf(nk_context, NK_TEXT_ALIGN_LEFT, "scale: %f, %f, %f", scale[0], scale[1], scale[2]);
}

void gui_camera_3d(struct nk_context* nk_context, Camera3D* data) {
    vec3 pos;
    camera3d_get_pos(data, pos);

    nk_label(nk_context, "type: camera_3d", NK_TEXT_ALIGN_LEFT);
    nk_labelf(nk_context, NK_TEXT_ALIGN_LEFT, "pos: %f, %f, %f", pos[0], pos[1], pos[2]);
    nk_labelf(nk_context, NK_TEXT_ALIGN_LEFT, "yaw: %f", camera3d_get_yaw(data));
    nk_labelf(nk_context, NK_TEXT_ALIGN_LEFT, "pitch: %f", camera3d_get_pitch(data));
    nk_labelf(nk_context, NK_TEXT_ALIGN_LEFT, "roll: %f", camera3d_get_roll(data));
}

void gui_node_properties(Node* node, void* user_data) {
    struct nk_context* nk_context = (struct nk_context*) user_data;

    if (nk_tree_push(nk_context, NK_TREE_TAB, node_get_name(node), NK_MAXIMIZED)) {
        nk_layout_row_dynamic(nk_context, 16, 1);

        nk_labelf(nk_context, NK_TEXT_ALIGN_LEFT, "%d sons", node_get_num_sons(node));

        switch (node_get_type(node)) {
        case NODE_TYPE_CONTAINER:
            nk_label(nk_context, "type: container", NK_TEXT_ALIGN_LEFT);
            break;

        case NODE_TYPE_MODEL_3D:
            Model3D* model_data = (Model3D*)node_get_data(node);
            gui_model_3d(nk_context, model_data);
            break;
        
        case NODE_TYPE_CAMERA_3D:
            Camera3D* cam_data = (Camera3D*)node_get_data(node);
            gui_camera_3d(nk_context, cam_data);
            break;

        default:
            nk_label(nk_context, "type: unknown :O", NK_TEXT_ALIGN_LEFT);
            break;
        }

        for (u32 i = 0; i < node_get_num_sons(node); i++) {
            gui_node_properties(node_get_sons(node)[i], user_data);
        }

        nk_tree_pop(nk_context);
    }
}

void gui_node_hierarchy(struct nk_context* nk_context, void* user_data) {
    Node* root = (Node*)user_data;

    if (nk_begin(nk_context, "node hierarchy", nk_rect(200, 50, 250, 250), gui_default_window_flags)) {
        gui_node_properties(root, nk_context);
    }
    nk_end(nk_context);
}