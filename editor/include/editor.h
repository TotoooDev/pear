#ifndef PEAR_EDITOR_H_
#define PEAR_EDITOR_H_

#include <scene/scene.h>

void editor_initialize();
void editor_delete();
array_t* editor_get_excluded_entities();

void editor_set_viewport_hovered(bool hovered);
void editor_set_scene_path(const char* path);
void editor_set_selected_entity(entity_t* entity);

bool editor_is_viewport_hovered();
char* editor_get_scene_path();
entity_t* editor_get_selected_entity();

#endif
