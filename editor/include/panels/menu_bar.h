#ifndef PEAR_GRAPHICS_EDITOR_MENU_BAR_H_
#define PEAR_GRAPHICS_EDITOR_MENU_BAR_H_

#include <scene/entity.h>
#include <core/types.h>

void panel_menu_bar_init();
void panel_menu_bar_free();
void panel_menu_bar_exclude_entity(entity_t* entity);
void panel_menu_bar_clear_excluded_entities();
void panel_menu_bar();

#endif
