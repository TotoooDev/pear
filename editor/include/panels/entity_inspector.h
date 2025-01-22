#ifndef PEAR_GRAPHICS_EDITOR_ENTITY_INSPECTOR_H_
#define PEAR_GRAPHICS_EDITOR_ENTITY_INSPECTOR_H_

#include <scene/scene.h>
#include <scene/entity.h>
#include <core/types.h>

void panel_entity_inspector_set_scene(scene_t* scene);
void panel_entity_inspector_set_entity(entity_t* entity);
void panel_entity_inspector();

#endif
