#ifndef PEAR_GUI_NODE_HIERARCHY_H
#define PEAR_GUI_NODE_HIERARCHY_H

#include <scene/node.h>
#include <core/gui/gui.h>

void gui_node_properties(Node* node, void* user_data, u32 counter);
void gui_node_hierarchy(struct nk_context* nk_context, void* user_data);

void gui_node_hierarchy_set_root_node(Node* root);

#endif
