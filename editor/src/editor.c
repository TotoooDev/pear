#include <editor.h>
#include <panels/menu_bar.h>
#include <panels/general_info.h>
#include <panels/scene_inspector.h>
#include <panels/entity_inspector.h>
#include <panels/viewport.h>
#include <scene/components/lua_script.h>
#include <event/event_dispatcher.h>
#include <graphics/editor/editor.h>
#include <util/array.h>
#include <core/app.h>
#include <string.h>

static array_t* editor_excluded_entities = NULL;
static char editor_scene_path[1024];

void editor_on_event(event_type_t type, void* e, void* user_data) {
    if (type != EVENT_TYPE_SCENE_NEW) {
        return;
    }

    scene_new_event_t* event = (scene_new_event_t*)e;
    scene_t* scene = event->new_scene;

    entity_t* editor_camera = scene_add_entity(scene, "[editor] editor camera");
    scene_add_component(scene, editor_camera, "transform");
    scene_add_component(scene, editor_camera, "camera");
    lua_script_component_t* script = scene_add_component(scene, editor_camera, "lua_script");
    script->script = script_new_from_file("scripts/editor_camera.lua", editor_camera);

    array_clear(editor_excluded_entities);
    array_add(editor_excluded_entities, editor_camera);

    panel_scene_inspector_set_scene(scene);
    panel_entity_inspector_set_scene(scene);
    panel_entity_inspector_set_entity(NULL);
}

void editor_initialize() {
    event_subscribe(editor_on_event, NULL);

    editor_scene_path[0] = '\0';
    editor_excluded_entities = array_new(5);

    scene_t* scene = app_get_scene();
    entity_t* editor_camera = scene_add_entity(scene, "[editor] editor camera");
    scene_add_component(scene, editor_camera, "transform");
    scene_add_component(scene, editor_camera, "camera");
    lua_script_component_t* script = scene_add_component(scene, editor_camera, "lua_script");
    script->script = script_new_from_file("scripts/editor_camera.lua", editor_camera);

    array_add(editor_excluded_entities, editor_camera);

    panel_scene_inspector_set_scene(scene);
    panel_entity_inspector_set_scene(scene);

    editor_add_function(panel_menu_bar, NULL);
    editor_add_function(panel_general_info, NULL);
    editor_add_function(panel_scene_inspector, NULL);
    editor_add_function(panel_entity_inspector, NULL);
    editor_add_function(panel_viewport, NULL);
}

void editor_delete() {
    array_delete(editor_excluded_entities);
}

array_t* editor_get_excluded_entities() {
    return editor_excluded_entities;
}

void editor_set_scene_path(const char* path) {
    strncpy(editor_scene_path, path, 1024);
}

char* editor_get_scene_path() {
    return editor_scene_path;
}
