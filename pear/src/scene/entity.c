#include <scene/entity.h>
#include <core/types.h>
#include <core/alloc.h>

typedef struct entity_t {
    entity_type_t type;
    void* data;
} entity_t;

entity_t* entity_new(entity_type_t type, void* data) {
    entity_t* entity = (entity_t*)PEAR_MALLOC(sizeof(entity_t));
    entity->type = type;
    entity->data = data;
    return entity;
}

void entity_delete(entity_t* entity) {
    PEAR_FREE(entity);
}

entity_type_t entity_get_type(entity_t* entity) {
    return entity->type;
}

void* entity_get_data(entity_t* entity) {
    return entity->data;
}
