#ifndef PEAR_EDITOR_H_
#define PEAR_EDITOR_H_

#include <scene/scene.h>

void editor_initialize();
void editor_delete();
array_t* editor_get_excluded_entities();

void editor_set_viewport_hovered(bool hovered);
bool editor_is_viewport_hovered();

void editor_set_scene_path(const char* path);
char* editor_get_scene_path();

#endif
