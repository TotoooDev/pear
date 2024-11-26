#ifndef PEAR_SCENE_ENTITY_H_
#define PEAR_SCENE_ENTITY_H_

#include <scene/entity_type.h>
#include <core/types.h>

typedef struct entity_t entity_t;

entity_t* entity_new(entity_type_t type);
void entity_delete(entity_t* entity);

entity_type_t entity_get_type(entity_t* entity);

#endif
