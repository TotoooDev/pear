#include <scene/entity.h>
#include <core/alloc.h>
#include <string.h>

typedef struct entity_t {
    u32 id;
    char name[ENTITY_NAME_MAX_LENGTH];

    hashmap_t* components;
} entity_t;

entity_t* entity_new(u32 id, const char* name) {
    entity_t* entity = (entity_t*)PEAR_MALLOC(sizeof(entity_t));
    entity->id = id;
    memcpy(entity->name, name, sizeof(char) * ENTITY_NAME_MAX_LENGTH);
    entity->components = hashmap_new();
    return entity;
}

void entity_delete(entity_t* entity) {
    hashmap_delete(entity->components);
    PEAR_FREE(entity);
}

u32 entity_get_id(entity_t* entity) {
    return entity->id;
}

const char* entity_get_name(entity_t* entity) {
    return entity->name;
}

hashmap_t* entity_get_hashmap(entity_t* entity) {
    return entity->components;
}
