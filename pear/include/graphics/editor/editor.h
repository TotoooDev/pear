#ifdef PEAR_ENABLE_EDITOR

#ifndef PEAR_GRAPHICS_EDITOR_H_
#define PEAR_GRAPHICS_EDITOR_H_

#include <core/types.h>

void editor_init();
void editor_free();

void editor_clear();
void editor_render();

void editor_enable_menu_bar(bool enable);
void editor_enable_general_info(bool enable);
void editor_enable_scene_inspector(bool enable);
void editor_enable_entity_inspector(bool enable);
void editor_enable_renderer_inspector(bool enable);
void editor_enable_demo(bool enable);

#endif

#endif
