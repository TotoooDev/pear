#include <core/log.h>
#include <core/app.h>
#include <scene/node.h>
#include <scene/types/mesh_3d.h>
#include <graphics/window.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    app_init();

    f32 vertices[] = {
         0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,
         0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, 0.0f,   0.0f, 1.0f, 1.0f
    };
    u32 indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    Mesh* mesh = mesh_new(vertices, indices, sizeof(vertices), sizeof(indices));

    Mesh3DCreationInfo info;
    info.mesh = mesh;

    Node* node = node_new(NODE_TYPE_MESH_3D, NULL, "father", &info);

    app_set_root_node(node);
    app_run();

    node_recursive_delete(node);

    PEAR_INFO("goodbye!");

    return 0;
}
