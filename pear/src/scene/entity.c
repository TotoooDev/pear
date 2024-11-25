#include <scene/entity.h>
#include <core/types.h>
#include <stdlib.h>

typedef struct entity_t {
    entity_type_t type;
} entity_t;

entity_t* entity_new(entity_type_t type) {
    entity_t* entity = (entity_t*)malloc(sizeof(entity_t));
    entity->type = type;
    return entity;
}

void entity_delete(entity_t* entity) {
    free(entity);
}
