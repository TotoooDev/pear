#include <scene/scene.h>
#include <util/array.h>
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
    free(scene);
}

entity_t* scene_add_entity(scene_t* scene) {
    entity_t* entity = entity_new();
    array_add(scene->entities, entity);
    return entity;
}

void scene_remove_entity(scene_t* scene, entity_t* entity) {
    array_remove(scene->entities, entity);
}
