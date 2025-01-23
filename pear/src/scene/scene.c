#include <scene/scene.h>
#include <scene/components/transform.h>
#include <scene/components/camera.h>
#include <scene/components/model.h>
#include <scene/components/skybox.h>
#include <scene/components/billboard.h>
#include <scene/components/light.h>
#include <scene/components/lua_script.h>
#include <util/array.h>
#include <util/hashmap.h>
#include <core/types.h>
#include <core/alloc.h>
#include <core/log.h>

#define SCENE_BASE_POOL_SIZE 512

typedef struct scene_t {
    hashmap_t* components;
    array_t* systems;
    array_t* user_datas;
    array_t* entities;

    u32 next_entity_id;
} scene_t;

typedef struct component_t {
    component_attachment_t attachment;
    u32 component_data_size;
} component_t;

scene_t* scene_new() {
    scene_t* scene = (scene_t*)PEAR_MALLOC(sizeof(scene_t));

    scene->components = hashmap_new();
    scene->systems = array_new(5);
    scene->user_datas = array_new(5);
    scene->entities = array_new(10);
    scene->next_entity_id = 0;

    scene_register_component(scene, "transform", transformcomponent_get_attachment(), sizeof(transform_component_t));
    scene_register_component(scene, "camera", cameracomponent_get_attachment(), sizeof(camera_component_t));
    scene_register_component(scene, "model", modelcomponent_get_attachment(), sizeof(model_component_t));
    scene_register_component(scene, "skybox", skyboxcomponent_get_attachment(), sizeof(skybox_component_t));
    scene_register_component(scene, "billboard", billboardcomponent_get_attachment(), sizeof(billboard_component_t));
    scene_register_component(scene, "light", lightcomponent_get_attachment(), sizeof(light_component_t));
    scene_register_component(scene, "lua_script", luascriptcomponent_get_attachment(), sizeof(lua_script_component_t));

    return scene;
}

void scene_delete(scene_t* scene) {
    hashmap_delete(scene->components);
    array_delete(scene->systems);
    array_delete(scene->user_datas);
    array_delete(scene->entities);
    PEAR_FREE(scene);
}

void scene_register_component(scene_t* scene, const char* name, component_attachment_t attachment, u32 component_data_size) {
    component_t* component = (component_t*)PEAR_MALLOC(sizeof(component_t));
    component->attachment = attachment;
    component->component_data_size = component_data_size;
    hashmap_set(scene->components, name, component);
}

void scene_register_system(scene_t* scene, scene_system_t system, void* user_data) {
    array_add(scene->systems, system);
    array_add(scene->user_datas, user_data);
}

entity_t* scene_add_entity(scene_t* scene, const char* name) {
    entity_t* entity = entity_new(scene->next_entity_id, name);
    array_add(scene->entities, entity);
    scene->next_entity_id++;
    return entity;
}

void scene_remove_entity(scene_t* scene, entity_t* entity) {
    array_remove(scene->entities, entity);
    entity_delete(entity);
}

bool scene_entity_exists(scene_t* scene, entity_t* entity) {
    for (u32 i = 0; i < array_get_length(scene->entities); i++) {
        if (entity_get_id(entity) == entity_get_id(array_get(scene->entities, i))) {
            return true;
        }
    }

    return false;
}

array_t* scene_get_entities(scene_t* scene) {
    return scene->entities;
}

void* scene_add_component(scene_t* scene, entity_t* entity, const char* name) {
    component_t* component = hashmap_get(scene->components, name);

    hashmap_t* hashmap = entity_get_hashmap(entity);
    void* component_data;
    if (!hashmap_exists(hashmap, name)) {
        component_data = PEAR_MALLOC(component->component_data_size);
    }
    else {
        component_data = hashmap_get(hashmap, name);
    }

    if (component->attachment.creator != NULL) {
        component->attachment.creator(component_data, entity);
    }

    hashmap_set(hashmap, name, component_data);

    return component_data;
}

void* scene_get_component(scene_t* scene, entity_t* entity, const char* name) {
    return hashmap_get(entity_get_hashmap(entity), name);
}

void scene_remove_component(scene_t* scene, entity_t* entity, const char* name) {
    component_t* component = hashmap_get(scene->components, name);
    if (component->attachment.destructor != NULL) {
        component->attachment.destructor(entity_get_hashmap(entity), entity);
    }
    hashmap_remove(entity_get_hashmap(entity), name);
}

bool scene_has_component(scene_t* scene, entity_t* entity, const char* name) {
    return hashmap_exists(entity_get_hashmap(entity), name);
}

void scene_update(scene_t* scene, f32 timestep) {
    for (u32 i = 0; i < array_get_length(scene->entities); i++) {
        entity_t* entity = array_get(scene->entities, i);

        for (u32 j = 0; j < array_get_length(scene->systems); j++) {
            scene_system_t system = array_get(scene->systems, j);
            void* user_data = array_get(scene->user_datas, j);
            system(scene, entity, timestep, user_data);
        }
    }
}
