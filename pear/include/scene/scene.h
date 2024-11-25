#ifndef PEAR_SCENE_H_
#define PEAR_SCENE_H_

#include <scene/entity.h>

typedef struct scene_t scene_t;

scene_t* scene_new();
void scene_delete(scene_t* scene);

entity_t* scene_add_entity(scene_t* scene);
void scene_remove_entity(scene_t* scene, entity_t* entity);

#endif
