#include <scene/scene.h>
#include <scene/components/script.h>
#include <util/array.h>
#include <core/log.h>
#include <core/types.h>
#include <core/alloc.h>
#include <stdarg.h>

#define SCENE_NUM_MAX_ENTITIES 512

typedef struct scene_t {
    array_t* entities;
} scene_t;

void scene_update_script_component(scene_t* scene, entity_t* entity, f32 timestep) {
    script_component_t* script = (script_component_t*)entity_get_component(entity, ENTITY_COMPONENT_SCRIPT);
    
    if (!script->run) {
        return;
    }

    if (!script->has_started) {
        if (script->on_start != NULL) {
            script->on_start(entity, timestep);
        }
        script->has_started = true;
    }

    if (script->on_update != NULL) {
        script->on_update(entity, timestep);
    }
}

scene_t* scene_new() {
    scene_t* scene = (scene_t*)PEAR_MALLOC(sizeof(scene_t));
    scene->entities = array_new(SCENE_NUM_MAX_ENTITIES);
    return scene;
}

void scene_delete(scene_t* scene) {
    for (u32 i = 0; i < array_get_length(scene->entities); i++) {
        entity_t* entity = array_get(scene->entities, i);
        scene_remove_entity(scene, entity);
    }

    array_delete(scene->entities);
    PEAR_FREE(scene);
}

entity_t* scene_add_entity(scene_t* scene, ...) {
    va_list args;
    va_start(args, scene);

    entity_t* entity = entity_new_from_va_list(args);
    
    va_end(args);
    
    array_add(scene->entities, entity);
    return entity;
}

void scene_remove_entity(scene_t* scene, entity_t* entity) {
    if (entity_has_component(entity, ENTITY_COMPONENT_SCRIPT)) {
        script_component_t* script = (script_component_t*)entity_get_component(entity, ENTITY_COMPONENT_SCRIPT);
        if (script->run && script->on_end != NULL) {
            script->on_end(entity, 0.0f);
        }
    }

    array_remove(scene->entities, entity);
    entity_delete(entity);
}

void scene_update(scene_t* scene, f32 timestep) {
    for (u32 i = 0; i < array_get_length(scene->entities); i++) {
        entity_t* entity = array_get(scene->entities, i);

        if (entity_has_component(entity, ENTITY_COMPONENT_SCRIPT)) {
            scene_update_script_component(scene, entity, timestep);
        }
    }
}
