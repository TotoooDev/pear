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

int main(int argc, char* argv[]) {
    app_init();
    
    ModelData* model_data = pear3d_load_model("model.pear3d");

    MeshInfo* mesh_info = meshinfo_new();
    meshinfo_add_attribute_vec3(mesh_info, false, pear3d_get_vertices(model_data), pear3d_get_num_vertices(model_data));
    meshinfo_add_attribute_vec2(mesh_info, false, pear3d_get_texture_coords(model_data), pear3d_get_num_texture_coords(model_data));

    Image* image = imageloader_from_file("wall.jpg");
    Texture* texture = texture_new_from_image(image, TEXTURE_WRAPPING_NONE, TEXTURE_FILTERING_NEAREST);
    image_delete(image);

    Mesh* mesh = mesh_new(mesh_info, (Material){ texture }, pear3d_get_indices(model_data), pear3d_get_num_indices(model_data));

    meshinfo_delete(mesh_info);
    pear3d_delete(model_data);

    Mesh3DCreationInfo mesh3d_info;
    mesh3d_info.mesh = mesh;

    Camera3DCreationInfo cam3d_info;
    cam3d_info.pos[0] = 0.0f;
    cam3d_info.pos[1] = 0.0f;
    cam3d_info.pos[2] = 0.0f;
    cam3d_info.yaw = -90.0f;
    cam3d_info.pitch = 0.0f;
    cam3d_info.roll = 0.0f;

    Node* parent = node_new(NODE_TYPE_CONTAINER, NULL, "parent", NULL, NULL);
    Node* cam = node_new(NODE_TYPE_CAMERA_3D, parent, "cam", &cam3d_info, NULL);
    event_subscribe(cam_on_event, cam);
    Node* mesh_node = node_new(NODE_TYPE_MESH_3D, parent, "mesh", &mesh3d_info, NULL);

    node_add_son(parent, cam);
    node_add_son(parent, mesh_node);

    app_set_root_node(parent);
    app_run();

    node_recursive_delete(parent);
    texture_delete(texture);

    PEAR_INFO("goodbye!");

    return 0;
}
