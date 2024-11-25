#ifndef PEAR_SCENE_ENTITY_H_
#define PEAR_SCENE_ENTITY_H_

#include <core/types.h>

typedef struct entity_t entity_t;

entity_t* entity_new();
void entity_delete(entity_t* entity);

#endif
