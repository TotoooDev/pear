#ifndef PEAR_SCENE_H_
#define PEAR_SCENE_H_

#include <scene/entity.h>
#include <util/array.h>

#ifdef PEAR_ENABLE_EDITOR
    #define SCENE_EDITOR_CAMERA_ID 0xFFFF
    #define SCENE_EDITOR_SCRIPT_ID 0xFFFE
#endif

typedef struct scene_t scene_t;

scene_t* scene_new();
void scene_delete(scene_t* scene);

entity_t* scene_add_entity(scene_t* scene, const char* name, ...);
void scene_add_entity_ptr(scene_t* scene, entity_t* entity);
void scene_remove_entity(scene_t* scene, entity_t* entity);

void scene_update(scene_t* scene, f32 timestep);

array_t* scene_get_entities(scene_t* scene);
u32 scene_get_next_entity_id(scene_t* scene);

#ifdef PEAR_ENABLE_EDITOR
void scene_setup_editor_scene(scene_t* scene);
void scene_display_component_billboards(scene_t* scene, bool display);
#endif

#endif
