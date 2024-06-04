#include <scene/node.h>
#include <core/types.h>
#include <core/log.h>
#include <stdlib.h>

typedef struct Node {
    Node* parent;
    Node** sons;
    u32 num_sons;
} Node;

Node* node_new(Node* parent) {
    if (parent == NULL)
        PEER_WARN("creating an orphan node!");
    
    Node* node = (Node*)malloc(sizeof(Node));

    node->parent = parent;
    node->sons = NULL;

    return node;
}

void node_delete(Node* node) {
    if (node->num_sons > 0)
        PEER_WARN("deleting a node that has sons! this will create orphans and probably lead to memory leaks and corruption. use node_recursive_delete() to delete the sons too.");

    free(node);
}

void node_recursive_delete(Node* node) {
    if (node->num_sons == 0)
        return;

    for (u32 i = 0; i < node->num_sons; i++)
        node_recursive_delete(node->sons[i]);
}

void node_add_son(Node* node, Node* son) {
    node->sons = (Node**)realloc(node->sons, node->num_sons + 1);
    node->sons[node->num_sons] = son;
    node->num_sons++;
}
