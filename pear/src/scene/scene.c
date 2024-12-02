#include <scene/scene.h>
#include <util/array.h>
#include <core/log.h>
#include <core/types.h>
#include <core/alloc.h>
#include <stdarg.h>

#define SCENE_NUM_MAX_ENTITIES 512

typedef struct scene_t {
    u32 num_entities;
    array_t* entities;
} scene_t;

scene_t* scene_new() {
    scene_t* scene = (scene_t*)PEAR_MALLOC(sizeof(scene_t));
    scene->num_entities = 0;
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
    array_remove(scene->entities, entity);
    entity_delete(entity);
}

void scene_update(scene_t* scene, f32 timestep) {

}
