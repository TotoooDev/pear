#include <scene/scene.h>
#include <scene/types/foo.h>
#include <scene/types/bar.h>
#include <util/array.h>
#include <core/log.h>
#include <core/types.h>
#include <stdlib.h>

#define SCENE_NUM_MAX_ENTITIES 512

typedef struct scene_t {
    u32 num_entities;
    array_t* entities;
} scene_t;

scene_t* scene_new() {
    scene_t* scene = (scene_t*)malloc(sizeof(scene_t));
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
    free(scene);
}

entity_t* scene_add_entity(scene_t* scene, entity_type_t type, void* creation_data) {
    void* data;
    switch (type) {
    case ENTITY_TYPE_FOO:
        data = foo_new(creation_data);
        break;

    case ENTITY_TYPE_BAR:
        data = bar_new(creation_data);
        break;

    default:
        data = NULL;
        break;
    }
    entity_t* entity = entity_new(type, data);

    array_add(scene->entities, entity);
    return entity;
}

void scene_remove_entity(scene_t* scene, entity_t* entity) {
    array_remove(scene->entities, entity);

    switch (entity_get_type(entity)) {
    case ENTITY_TYPE_FOO:
        foo_delete(entity_get_data(entity));
        break;

    case ENTITY_TYPE_BAR:
        bar_delete(entity_get_data(entity));
        break;

    default:
        break;
    }
    entity_delete(entity);
}

void scene_update(scene_t* scene, f32 timestep) {
    for (u32 i = 0; i < array_get_length(scene->entities); i++) {
        entity_t* entity = array_get(scene->entities, i);
        switch (entity_get_type(entity)) {
        case ENTITY_TYPE_FOO:
            foo_update(entity_get_data(entity), timestep);
            break;

        case ENTITY_TYPE_BAR:
            bar_update(entity_get_data(entity), timestep);
            break;
        
        default:
            break;
        }
    }
}
