#include <scene/entity.h>
#include <scene/components/transform.h>
#include <scene/components/model.h>
#include <scene/components/camera.h>
#include <scene/components/light.h>
#include <scene/components/script.h>
#include <core/log.h>
#include <core/alloc.h>
#include <stdarg.h>

typedef struct entity_t {
    const char* name;
    u32 components_bitmask;
    void** components;
} entity_t;

entity_t* entity_new(const char* name, ...) {
    va_list args;
    va_start(args, name);

    entity_t* entity = entity_new_from_va_list(name, args);

    va_end(args);

    return entity;
}

entity_t* entity_new_from_va_list(const char* name, va_list args) {
    entity_t* entity = (entity_t*)PEAR_MALLOC(sizeof(entity_t));
    entity->name = name;
    
    entity->components_bitmask = 0;
    entity->components = (void**)PEAR_MALLOC(sizeof(void*) * (ENTITY_COMPONENT_END + 1));
    for (u32 i = 0; i < ENTITY_COMPONENT_END + 1; i++) {
        entity->components[i] = NULL;
    }

    entity_component_t current_component;
    while ((current_component = va_arg(args, entity_component_t)) != ENTITY_COMPONENT_END) {
        switch (current_component) {
        case ENTITY_COMPONENT_TRANSFORM:
            entity->components[(u32)current_component] = transformcomponent_new();
            break;

        case ENTITY_COMPONENT_MODEL:
            model_component_t* model = (model_component_t*)PEAR_MALLOC(sizeof(model_component_t));
            model->model = NULL;
            model->draw = true;
            entity->components[(u32)current_component] = model;
            break;

        case ENTITY_COMPONENT_CAMERA:
            camera_component_t* camera = (camera_component_t*)PEAR_MALLOC(sizeof(camera_component_t));
            camera->use = true;
            entity->components[(u32)current_component] = camera;
            break;

        case ENTITY_COMPONENT_LIGHT:
            light_component_t* light = (light_component_t*)PEAR_MALLOC(sizeof(light_component_t));
            light->cast = true;
            entity->components[(u32)current_component] = light;
            break;

        case ENTITY_COMPONENT_SCRIPT:
            entity->components[(u32)current_component] = scriptcomponent_new();
            break;

        default:
            PEAR_WARN("component %d is not implemented!", (u32)current_component);
            break;
        }
        entity->components_bitmask |= 1 << (u32)current_component;
    }

    return entity;
}

void entity_delete(entity_t* entity) {
    for (u32 i = 0; i < ENTITY_COMPONENT_END + 1; i++) {
        if (entity->components != NULL) {
            PEAR_FREE(entity->components[i]);
        }
    }

    PEAR_FREE(entity);
}

const char* entity_get_name(entity_t* entity) {
    return entity->name;
}

void* entity_get_component(entity_t* entity, entity_component_t component) {
    if (!entity_has_component(entity, component)) {
        PEAR_WARN("trying to access unattached component %d from entity %p!", component, entity);
        return NULL;
    }

    return entity->components[(u32)component];
}

bool entity_has_component(entity_t* entity, entity_component_t component) {
    return (entity->components_bitmask & 1 << (u32)component) != 0;
}
