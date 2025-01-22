#ifndef PEAR_SCENE_ENTITY_H_
#define PEAR_SCENE_ENTITY_H_

#include <scene/scene.h>
#include <util/hashmap.h>
#include <core/types.h>

#define ENTITY_NAME_MAX_LENGTH 128

typedef struct entity_t entity_t;

entity_t* entity_new(u32 id, const char* name);
void entity_delete(entity_t* entity);

u32 entity_get_id(entity_t* entity);
const char* entity_get_name(entity_t* entity);
hashmap_t* entity_get_hashmap(entity_t* entity);

#endif
