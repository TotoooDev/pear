#ifndef PEER_NODE_H
#define PEER_NODE_H

typedef struct Node Node;

Node* node_new(Node* parent);
void node_delete(Node* node);
void node_recursive_delete(Node* node);

void node_add_son(Node* node, Node* son);

#endif
