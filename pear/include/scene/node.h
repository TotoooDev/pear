#ifndef PEAR_NODE_H
#define PEAR_NODE_H

#include <scene/node_type.h>
#include <core/types.h>

typedef struct Node Node;

typedef void(*NodeUpdateFunction)(Node*, f32);

Node* node_new(NodeType type, Node* parent, const char* name, void* creation_info, NodeUpdateFunction update_function);
void node_delete(Node* node);
void node_recursive_delete(Node* node);

void node_add_son(Node* node, Node* son);
Node** node_get_sons(Node* node);
u32 node_get_num_sons(Node* node);
bool node_is_leaf(Node* node);
Node* node_get_parent(Node* node);
char* node_get_name(Node* node);

void node_set_update_function(Node* node, NodeUpdateFunction update_function);
void node_update_recursive(Node* node, f32 timestep);
void node_update(Node* node, f32 timestep);

NodeType node_get_type(Node* node);
void* node_get_data(Node* node);

#endif
