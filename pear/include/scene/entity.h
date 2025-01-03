#ifndef PEAR_SCENE_ENTITY_H_
#define PEAR_SCENE_ENTITY_H_

#include <core/types.h>
#include <stdarg.h>

#define ENTITY_NAME_MAX_LENGTH 256

typedef enum entity_component_t {
    ENTITY_COMPONENT_TRANSFORM,
    ENTITY_COMPONENT_MODEL,
    ENTITY_COMPONENT_CAMERA,
    ENTITY_COMPONENT_SCRIPT,
    ENTITY_COMPONENT_LUA_SCRIPT,
    ENTITY_COMPONENT_LIGHT,
    ENTITY_COMPONENT_SKYBOX,
    ENTITY_COMPONENT_END
} entity_component_t;

typedef struct entity_t entity_t;

entity_t* entity_new(const char* name, ...);
entity_t* entity_new_from_va_list(const char* name, va_list args);
void entity_delete(entity_t* entity);

void* entity_add_component(entity_t* entity, entity_component_t component);
void entity_remove_component(entity_t* entity, entity_component_t component);

char* entity_get_name(entity_t* entity);
void* entity_get_component(entity_t* entity, entity_component_t component);
bool entity_has_component(entity_t* entity, entity_component_t component);

#endif
