#include <core/app.h>
#include <core/log.h>
#include <panels/menu_bar.h>
#include <panels/general_info.h>
#include <panels/scene_inspector.h>
#include <panels/entity_inspector.h>
#include <panels/viewport.h>
#include <scene/components/transform.h>
#include <scene/components/model.h>
#include <scene/components/billboard.h>
#include <scene/components/camera.h>
#include <scene/components/script.h>
#include <scene/components/lua_script.h>
#include <scene/components/light.h>
#include <scene/components/skybox.h>
#include <loaders/model.h>
#include <graphics/editor/editor.h>
#include <string.h>

scene_t* editor_scene = NULL;

void editor_script_on_update(entity_t* entity, f32 timestep) {
    size_t component_sizes[] = {
        sizeof(transform_component_t),
        sizeof(model_component_t),
        sizeof(billboard_component_t),
        sizeof(camera_component_t),
        sizeof(script_component_t),
        sizeof(lua_script_component_t),
        sizeof(light_component_t),
        sizeof(skybox_component_t)
    };

    scene_t* viewport_scene = app_get_scene();
    
    array_t* editor_entities = scene_get_entities(editor_scene);
    array_t* viewport_entities = scene_get_entities(viewport_scene);

    for (u32 i = 0; i < array_get_length(editor_entities); i++) {
        bool add_entity_to_viewport = true;
        entity_t* editor_entity = array_get(editor_entities, i);

        for (u32 j = 0; j < array_get_length(viewport_entities); j++) {
            entity_t* viewport_entity = array_get(viewport_entities, j);

            if (entity_get_id(viewport_entity) == SCENE_EDITOR_CAMERA_ID || entity_get_id(viewport_entity) == SCENE_EDITOR_SCRIPT_ID) {
                continue;
            }

            if (entity_get_id(viewport_entity) == entity_get_id(editor_entity)) {
                memcpy(entity_get_name(viewport_entity), entity_get_name(editor_entity), sizeof(char) * ENTITY_NAME_MAX_LENGTH);
                for (u32 k = 0; k < (u32)ENTITY_COMPONENT_END; k++) {
                    if (!entity_has_component(editor_entity, (entity_component_t)k)) {
                        if (entity_has_component(viewport_entity, (entity_component_t)k)) {
                            if (entity_has_component(viewport_entity, ENTITY_COMPONENT_MODEL)) {
                                model_component_t* model_comp = entity_get_component(viewport_entity, ENTITY_COMPONENT_MODEL);
                                model_comp->model = NULL;
                            }
                            if (entity_has_component(viewport_entity, ENTITY_COMPONENT_BILLBOARD)) {
                                billboard_component_t* billboard_comp = entity_get_component(viewport_entity, ENTITY_COMPONENT_BILLBOARD);
                                billboard_comp->texture = NULL;
                            }
                            if (entity_has_component(viewport_entity, ENTITY_COMPONENT_LUA_SCRIPT)) {
                                lua_script_component_t* script_comp = entity_get_component(viewport_entity, ENTITY_COMPONENT_LUA_SCRIPT);
                                script_comp->script = NULL;
                            }
                            if (entity_has_component(viewport_entity, ENTITY_COMPONENT_SKYBOX)) {
                                skybox_component_t* skybox_comp = entity_get_component(viewport_entity, ENTITY_COMPONENT_SKYBOX);
                                skybox_comp->cubemap = NULL;
                            }
                            
                            entity_remove_component(viewport_entity, (entity_component_t)k);
                        }
                        continue;
                    }

                    if (!entity_has_component(viewport_entity, (entity_component_t)k)) {
                        entity_add_component(viewport_entity, (entity_component_t)k);
                    }
                    
                    void* src_component = entity_get_component(editor_entity, (entity_component_t)k);
                    void* dest_component = entity_get_component(viewport_entity, (entity_component_t)k);
                    memcpy(dest_component, src_component, component_sizes[k]);
                }

                add_entity_to_viewport = false;
                break;
            }
        }

        if (add_entity_to_viewport) {
            entity_t* new_entity = entity_new(entity_get_id(editor_entity), entity_get_name(editor_entity), ENTITY_COMPONENT_END);
            scene_add_entity_ptr(viewport_scene, new_entity);
        }
    }

    for (u32 i = 0; i < array_get_length(viewport_entities); i++) {
        bool remove_entity_from_viewport = true;
        entity_t* viewport_entity = array_get(viewport_entities, i);

        if (entity_get_id(viewport_entity) == SCENE_EDITOR_CAMERA_ID || entity_get_id(viewport_entity) == SCENE_EDITOR_SCRIPT_ID) {
            continue;
        }

        for (u32 j = 0; j < array_get_length(editor_entities); j++) {
            entity_t* editor_entity = array_get(editor_entities, j);

            if (entity_get_id(viewport_entity) == entity_get_id(editor_entity)) {
                remove_entity_from_viewport = false;
                break;
            }
        }

        if (remove_entity_from_viewport) {
            if (entity_has_component(viewport_entity, ENTITY_COMPONENT_MODEL)) {
                model_component_t* model_comp = entity_get_component(viewport_entity, ENTITY_COMPONENT_MODEL);
                model_comp->model = NULL;
            }
            if (entity_has_component(viewport_entity, ENTITY_COMPONENT_BILLBOARD)) {
                billboard_component_t* billboard_comp = entity_get_component(viewport_entity, ENTITY_COMPONENT_BILLBOARD);
                billboard_comp->texture = NULL;
            }
            if (entity_has_component(viewport_entity, ENTITY_COMPONENT_LUA_SCRIPT)) {
                lua_script_component_t* script_comp = entity_get_component(viewport_entity, ENTITY_COMPONENT_LUA_SCRIPT);
                script_comp->script = NULL;
            }
            if (entity_has_component(viewport_entity, ENTITY_COMPONENT_SKYBOX)) {
                skybox_component_t* skybox_comp = entity_get_component(viewport_entity, ENTITY_COMPONENT_SKYBOX);
                skybox_comp->cubemap = NULL;
            }

            scene_remove_entity(viewport_scene, viewport_entity);
        }
    }
}

int main(int argc, char* argv[]) {
    PEAR_INFO("hello editor!");

    app_init();

    scene_t* viewport_scene = app_get_scene();

    editor_scene = scene_new();

    entity_t* editor_script = entity_new(SCENE_EDITOR_SCRIPT_ID, "[EDITOR] script", ENTITY_COMPONENT_SCRIPT, ENTITY_COMPONENT_END);
    entity_t* viewport_camera = entity_new(SCENE_EDITOR_CAMERA_ID, "[EDITOR] camera", ENTITY_COMPONENT_TRANSFORM, ENTITY_COMPONENT_CAMERA, ENTITY_COMPONENT_SCRIPT, ENTITY_COMPONENT_END);
    
    script_component_t* script_comp = entity_get_component(editor_script, ENTITY_COMPONENT_SCRIPT);
    script_comp->on_update = editor_script_on_update;

    scene_add_entity_ptr(viewport_scene, editor_script);
    scene_add_entity_ptr(viewport_scene, viewport_camera);

    panel_scene_inspector_set_scene(editor_scene);

    editor_add_function(panel_menu_bar, NULL);
    editor_add_function(panel_general_info, NULL);
    editor_add_function(panel_scene_inspector, NULL);
    editor_add_function(panel_entity_inspector, NULL);
    editor_add_function(panel_viewport, NULL);
    
    app_run();

    return 0;
}
