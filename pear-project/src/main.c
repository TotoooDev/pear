#include <core/log.h>
#include <core/app.h>
#include <scene/node.h>
#include <scene/types/mesh_3d.h>
#include <scene/types/camera_3d.h>
#include <graphics/window.h>
#include <event/event_dispatcher.h>
#include <stdlib.h>

void on_event(EventType type, void* e, void* user_data) {
    if (type == EVENT_TYPE_KEY_DOWN) {
        KeyDownEvent* event = (KeyDownEvent*)e;
        PEAR_INFO("key %d down!", event->key);
    }
    if (type == EVENT_TYPE_KEY_UP) {
        KeyUpEvent* event = (KeyUpEvent*)e;
        PEAR_INFO("key %d up!", event->key);
    }
}

int main(int argc, char* argv[]) {
    app_init();

    event_subscribe(on_event, NULL);

    MeshInfo* mesh_info = meshinfo_new();
    meshinfo_add_attribute(mesh_info, MESH_DATA_TYPE_FLOAT3, false);
    meshinfo_add_attribute(mesh_info, MESH_DATA_TYPE_FLOAT3, false);

    f32 vertices[] = {
         0.5f,  0.5f, -2.0f,   1.0f, 1.0f, 0.0f,
         0.5f, -0.5f, -2.0f,   1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -2.0f,   0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, -2.0f,   0.0f, 1.0f, 1.0f
    };
    u32 indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    Mesh* mesh = mesh_new(mesh_info, vertices, indices, sizeof(vertices), sizeof(indices));

    meshinfo_delete(mesh_info);

    Mesh3DCreationInfo mesh3d_info;
    mesh3d_info.mesh = mesh;

    Camera3DCreationInfo cam3d_info;
    cam3d_info.pos[0] = 0.0f;
    cam3d_info.pos[1] = 0.0f;
    cam3d_info.pos[2] = 0.0f;
    cam3d_info.yaw = -90.0f;
    cam3d_info.pitch = 0.0f;
    cam3d_info.roll = 0.0f;

    Node* parent = node_new(NODE_TYPE_CONTAINER, NULL, "parent", NULL);
    Node* cam = node_new(NODE_TYPE_CAMERA_3D, parent, "cam", &cam3d_info);
    Node* mesh_node = node_new(NODE_TYPE_MESH_3D, parent, "mesh", &mesh3d_info);

    node_add_son(parent, cam);
    node_add_son(parent, mesh_node);

    app_set_root_node(parent);
    app_run();

    node_recursive_delete(parent);

    PEAR_INFO("goodbye!");

    return 0;
}
