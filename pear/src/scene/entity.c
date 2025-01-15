#include <scene/entity.h>
#include <scene/components/transform.h>
#include <scene/components/model.h>
#include <scene/components/billboard.h>
#include <scene/components/camera.h>
#include <scene/components/light.h>
#include <scene/components/script.h>
#include <scene/components/lua_script.h>
#include <scene/components/skybox.h>
#include <core/log.h>
#include <core/alloc.h>
#include <string.h>
#include <stdarg.h>

typedef struct entity_t {
    char* name;
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

    entity->name = PEAR_MALLOC(sizeof(char) * ENTITY_NAME_MAX_LENGTH);
    strcpy(entity->name, name);
    
    entity->components_bitmask = 0;
    entity->components = (void**)PEAR_MALLOC(sizeof(void*) * (ENTITY_COMPONENT_END + 1));
    for (u32 i = 0; i < ENTITY_COMPONENT_END + 1; i++) {
        entity->components[i] = NULL;
    }

    entity_component_t current_component;
    while ((current_component = va_arg(args, entity_component_t)) != ENTITY_COMPONENT_END) {
        entity_add_component(entity, current_component);
    }

    return entity;
}

void entity_delete(entity_t* entity) {
    for (u32 i = 0; i < ENTITY_COMPONENT_END + 1; i++) {
        if (entity_has_component(entity, (entity_component_t)i)) {
            entity_remove_component(entity, (entity_component_t)i);
        }
    }
    PEAR_FREE(entity->components);

    PEAR_FREE(entity->name);
    PEAR_FREE(entity);
}

void* entity_add_component(entity_t* entity, entity_component_t component) {
    if (entity_has_component(entity, component)) {
        PEAR_WARN("entity %s already has component %d!", entity->name, component);
        return entity_get_component(entity, component);
    }

    switch (component) {
    case ENTITY_COMPONENT_TRANSFORM:
        entity->components[(u32)component] = transformcomponent_new();
        break;

    case ENTITY_COMPONENT_MODEL:
        model_component_t* model = (model_component_t*)PEAR_MALLOC(sizeof(model_component_t));
        model->model = NULL;
        model->draw = true;
        model->shadow_caster = true;
        entity->components[(u32)component] = model;
        break;

    case ENTITY_COMPONENT_BILLBOARD:
        billboard_component_t* billboard = (billboard_component_t*)PEAR_MALLOC(sizeof(billboard_component_t));
        billboard->texture = NULL;
        billboard->on_top = false;
        billboard->apply_perspective = false;
        billboard->draw = true;
        entity->components[(u32)component] = billboard;
        break;

    case ENTITY_COMPONENT_CAMERA:
        camera_component_t* camera = (camera_component_t*)PEAR_MALLOC(sizeof(camera_component_t));
        camera->use = true;
        entity->components[(u32)component] = camera;
        break;

    case ENTITY_COMPONENT_LIGHT:
        entity->components[(u32)component] = lightcomponent_new();
        break;

    case ENTITY_COMPONENT_SCRIPT:
        entity->components[(u32)component] = scriptcomponent_new();
        break;

    case ENTITY_COMPONENT_LUA_SCRIPT:
        lua_script_component_t* script = (lua_script_component_t*)PEAR_MALLOC(sizeof(lua_script_component_t));
        script->run = true;
        script->has_started = false;
        script->script = NULL;
        entity->components[(u32)component] = script;
        break;

    case ENTITY_COMPONENT_SKYBOX:
        skybox_component_t* skybox = (skybox_component_t*)PEAR_MALLOC(sizeof(skybox_component_t));
        skybox->cubemap = NULL;
        skybox->draw = true;
        entity->components[(u32)component] = skybox;
        break;

    default:
        PEAR_WARN("component %d is not implemented!", (u32)component);
        return NULL;
    }

    entity->components_bitmask |= 1 << (u32)component;

    return entity->components[(u32)component];
}

void entity_remove_component(entity_t* entity, entity_component_t component) {
    if (!entity_has_component(entity, component)) {
        PEAR_WARN("entity %s has no component %d!", entity->name, component);
        return;
    }

    switch (component) {
    case ENTITY_COMPONENT_MODEL:
        model_component_t* model = (model_component_t*)entity_get_component(entity, ENTITY_COMPONENT_MODEL);
        model_delete(model->model);
        break;

    case ENTITY_COMPONENT_BILLBOARD:
        billboard_component_t* billboard = (billboard_component_t*)entity_get_component(entity, ENTITY_COMPONENT_BILLBOARD);
        texture_delete(billboard->texture);
        break;

    case ENTITY_COMPONENT_LUA_SCRIPT:
        lua_script_component_t* script = (lua_script_component_t*)entity_get_component(entity, ENTITY_COMPONENT_LUA_SCRIPT);
        script_delete(script->script);
        break;

    case ENTITY_COMPONENT_SKYBOX:
        skybox_component_t* skybox = (skybox_component_t*)entity_get_component(entity, ENTITY_COMPONENT_SKYBOX);
        cubemap_delete(skybox->cubemap);
        break;

    default:
        break;
    }

    PEAR_FREE(entity->components[(u32)component]);

    entity->components_bitmask ^= 1 << (u32)component;
}

char* entity_get_name(entity_t* entity) {
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
