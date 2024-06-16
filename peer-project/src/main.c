#include <core/log.h>
#include <core/app.h>
#include <scene/node.h>
#include <graphics/window.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    Node* node = node_new(NODE_TYPE_CONTAINER, NULL, "father");
    node_add_son(node, node_new(NODE_TYPE_CONTAINER, node, "son1"));
    node_add_son(node, node_new(NODE_TYPE_CONTAINER, node, "son2"));
    node_add_son(node, node_new(NODE_TYPE_CONTAINER, node, "son3"));
    node_recursive_delete(node);

    app_run();

    PEER_INFO("goodbye!");

    return 0;
}
