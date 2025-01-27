#ifndef PEAR_GRAPHICS_EDITOR_MENU_BAR_H_
#define PEAR_GRAPHICS_EDITOR_MENU_BAR_H_

#include <scene/entity.h>
#include <event/event_dispatcher.h>
#include <core/types.h>

void panel_menu_bar_on_event(event_type_t type, void* e, void* user_pointer);
void panel_menu_bar();

#endif
