#ifndef PEAR_GRAPHICS_EDITOR_SCENE_INSPECTOR_H_
#define PEAR_GRAPHICS_EDITOR_SCENE_INSPECTOR_H_

#include <scene/scene.h>

void panel_scene_inspector_init();
void panel_scene_inspector_free();
void panel_scene_inspector_set_scene(scene_t* scene);
void panel_scene_inspector_exclude_entity(entity_t* entity);
void panel_scene_inspector_clear_excluded_entities();
void panel_scene_inspector();

#endif
