#include <scene/entity.h>
#include <core/types.h>
#include <stdlib.h>

typedef struct entity_t {
    u32 foo;
} entity_t;

entity_t* entity_new() {
    entity_t* entity = (entity_t*)malloc(sizeof(entity_t));
    return entity;
}

void entity_delete(entity_t* entity) {
    free(entity);
}
