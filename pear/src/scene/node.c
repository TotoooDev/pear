#include <scene/node.h>
#include <scene/types/model_3d.h>
#include <scene/types/mesh_3d.h>
#include <scene/types/camera_3d.h>
#include <scene/types/light_3d.h>
#include <core/types.h>
#include <core/log.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char* name;

    Node* parent;
    Node** sons;
    u32 num_sons;

    NodeUpdateFunction update_function;

    void* data;
    NodeType type;
} Node;

void* node_get_data_from_type(Node* node, NodeType type, void* creation_info) {
    switch (type) {
    case NODE_TYPE_CONTAINER:
        return NULL;
    case NODE_TYPE_MODEL_3D:
        return (void*)model3d_new((Model3DCreationInfo*)creation_info, node);
    case NODE_TYPE_MESH_3D:
        return (void*)mesh3d_new((Mesh3DCreationInfo*)creation_info);
    case NODE_TYPE_CAMERA_3D:
        return (void*)camera3d_new((Camera3DCreationInfo*)creation_info);
    case NODE_TYPE_LIGHT_3D:
        return (void*)light3d_new((Light3DCreationInfo*)creation_info);
    default:
        PEAR_ERROR("unknown node type!");
        return NULL;
    }
}

Node* node_new(NodeType type, Node* parent, const char* name, void* creation_info, NodeUpdateFunction update_function) {
    if (parent == NULL)
        PEAR_WARN("creating an orphan node!");
    if (name[0] == '\0')
        PEAR_WARN("name is empty!");
    
    Node* node = (Node*)malloc(sizeof(Node));

    node->name = (char*)malloc(sizeof(char) * strlen(name) + 1);
    node->parent = parent;
    node->sons = NULL;
    node->num_sons = 0;
    node->update_function = update_function;
    node->data = node_get_data_from_type(node, type, creation_info);
    node->type = type;

    strcpy(node->name, name);

    return node;
}

void node_delete(Node* node) {
    free(node->name);
    free(node->data);
    free(node);
}

void node_recursive_delete(Node* node) {
    node_delete(node);

    if (node->num_sons == 0)
        return;

    for (u32 i = 0; i < node->num_sons; i++)
        node_recursive_delete(node->sons[i]);
}

void node_add_son(Node* node, Node* son) {
    node->sons = (Node**)realloc(node->sons, sizeof(Node*) * node->num_sons + 1);
    node->sons[node->num_sons] = son;
    node->num_sons++;
}

Node** node_get_sons(Node* node) {
    return node->sons;
}

u32 node_get_num_sons(Node* node) {
    return node->num_sons;
}

bool node_is_leaf(Node* node) {
    return node_get_num_sons(node) == 0;
}

Node* node_get_parent(Node* node) {
    return node->parent;
}

char* node_get_name(Node* node) {
    return node->name;
}

void node_set_update_function(Node* node, NodeUpdateFunction update_function) {
    node->update_function = update_function;
}

void node_update_recursive(Node* node, f32 timestep) {
    node_update(node, timestep);

    if (node_is_leaf(node))
        return;

    Node** sons = node_get_sons(node);
    for (u32 i = 0; i < node_get_num_sons(node); i++) {
        Node* son = sons[i];
        node_update_recursive(son, timestep);
    }
}

void node_update(Node* node, f32 timestep) {
    if (node->update_function != NULL)
        node->update_function(node, timestep);
}

void node_map_recursive(Node* node, NodeMapFunction function, void* user_data) {
    function(node, user_data);

    if (node_is_leaf(node))
        return;

    Node** sons = node_get_sons(node);
    for (u32 i = 0; i < node_get_num_sons(node); i++) {
        Node* son = sons[i];
        function(son, user_data);
    }
}

NodeType node_get_type(Node* node) {
    return node->type;
}

void* node_get_data(Node* node) {
    return node->data;
}
