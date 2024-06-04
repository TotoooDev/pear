#include <core/log.h>
#include <core/app.h>
#include <scene/node.h>
#include <graphics/window.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    Node* node = node_new(NULL);
    node_add_son(node, node_new(node));
    node_add_son(node, node_new(node));
    node_add_son(node, node_new(node));
    node_recursive_delete(node);

    app_run();

    PEER_INFO("goodbye!");

    return 0;
}
