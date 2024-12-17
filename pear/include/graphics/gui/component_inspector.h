#ifndef PEAR_GUI_COMPONENT_INSPECTOR_H_
#define PEAR_GUI_COMPONENT_INSPECTOR_H_

#include <graphics/gui/gui.h>
#include <scene/entity.h>

void gui_component_inspector_set_entity(entity_t* scene);
void gui_component_inspector(struct nk_context* nk_context, void* user_data);

#endif
