#ifndef PEAR_SCENE_H_
#define PEAR_SCENE_H_

#include <scene/entity.h>
#include <scene/component_attachment.h>
#include <util/array.h>
#include <core/types.h>

typedef struct scene_t scene_t;

typedef void(*scene_system_t)(scene_t*, entity_t*, f32, void*);
typedef void(*scene_component_added_function_t)(scene_t*, entity_t*);
typedef void(*scene_component_removed_function_t)(scene_t*, entity_t*);

scene_t* scene_new();
void scene_delete(scene_t* scene);

void scene_register_component(scene_t* scene, const char* name, component_attachment_t attachment, u32 component_data_size);
void scene_register_system(scene_t* scene, scene_system_t system, void* user_data);
void scene_add_component_added_function(scene_t* scene, const char* name, scene_component_added_function_t function);
void scene_add_component_removed_function(scene_t* scene, const char* name, scene_component_removed_function_t function);

entity_t* scene_add_entity(scene_t* scene, const char* name);
void scene_remove_entity(scene_t* scene, entity_t* entity);
bool scene_entity_exists(scene_t* scene, entity_t* entity);
array_t* scene_get_entities(scene_t* scene);

void* scene_add_component(scene_t* scene, entity_t* entity, const char* name);
void* scene_get_component(scene_t* scene, entity_t* entity, const char* name);
void scene_remove_component(scene_t* scene, entity_t* entity, const char* name);
bool scene_has_component(scene_t* scene, entity_t* entity, const char* name);

void scene_update(scene_t* scene, f32 timestep);

#endif
