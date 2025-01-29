#include <editor.h>
#include <renderers/editor_billboard_renderer.h>
#include <components/editor_billboard.h>
#include <panels/menu_bar.h>
#include <panels/general_info.h>
#include <panels/scene_inspector.h>
#include <panels/entity_inspector.h>
#include <panels/renderer_inspector.h>
#include <panels/viewport.h>
#include <scene/components/lua_script.h>
#include <event/event_dispatcher.h>
#include <graphics/editor/editor.h>
#include <loaders/image.h>
#include <util/array.h>
#include <core/app.h>
#include <string.h>

static array_t* editor_excluded_entities = NULL;
static char editor_scene_path[1024];
static bool editor_viewport_hovered = false;
static entity_t* editor_selected_entity = NULL;
static texture_t* editor_lightbulb_texture = NULL;
static texture_t* editor_camera_texture = NULL;

void editor_system(scene_t* scene, entity_t* entity, f32 timestep, void* user_data) {
    if (!scene_has_component(scene, entity, "lua_script")) {
        return;
    }

    lua_script_component_t* script = scene_get_component(scene, entity, "lua_script");
    script_begin_table(script->script, "editor");
        script_set_bool(script->script, editor_is_viewport_hovered(), "is_viewport_hovered");
    script_end_table(script->script);
}

void editor_on_light_component_added(scene_t* scene, entity_t* entity) {
    editor_billboard_component_t* billboard = scene_add_component(scene, entity, "editor_billboard");
    billboard->texture = editor_lightbulb_texture;
}

void editor_on_light_component_removed(scene_t* scene, entity_t* entity) {
    scene_remove_component(scene, entity, "editor_billboard");
}

void editor_on_camera_component_added(scene_t* scene, entity_t* entity) {
    editor_billboard_component_t* billboard = scene_add_component(scene, entity, "editor_billboard");
    billboard->texture = editor_camera_texture;
}

void editor_on_camera_component_removed(scene_t* scene, entity_t* entity) {
    scene_remove_component(scene, entity, "editor_billboard");
}

void editor_on_event(event_type_t type, void* e, void* user_data) {
    if (type != EVENT_TYPE_SCENE_NEW) {
        return;
    }

    scene_new_event_t* event = (scene_new_event_t*)e;
    scene_t* scene = event->new_scene;
    scene_register_component(scene, "editor_billboard", editorbillboardcomponent_get_attachment(), sizeof(editor_billboard_component_t));
    scene_register_system(scene, editor_system, NULL);
    scene_add_component_added_function(scene, "light", editor_on_light_component_added);
    scene_add_component_removed_function(scene, "light", editor_on_light_component_removed);
    scene_add_component_added_function(scene, "camera", editor_on_camera_component_added);
    scene_add_component_removed_function(scene, "camera", editor_on_camera_component_removed);

    entity_t* editor_camera = scene_add_entity(scene, "[editor] editor camera");
    scene_add_component(scene, editor_camera, "transform");
    scene_add_component(scene, editor_camera, "camera");
    lua_script_component_t* script = scene_add_component(scene, editor_camera, "lua_script");
    script->script = script_new_from_file("scripts/editor_camera.lua", editor_camera);

    editor_selected_entity = NULL;

    array_clear(editor_excluded_entities);
    array_add(editor_excluded_entities, editor_camera);
}

void editor_initialize() {
    event_subscribe(editor_on_event, NULL);
    event_subscribe(panel_menu_bar_on_event, NULL);

    renderer_add_renderer_interface(app_get_renderer(), editorbillboardrenderer_new(app_get_renderer()));

    editor_scene_path[0] = '\0';
    editor_excluded_entities = array_new(5);
    
    image_t* lightbulb_image = loader_load_image("images/lightbulb.png");
    editor_lightbulb_texture = texture_new_from_image(lightbulb_image, TEXTURE_WRAPPING_CLAMP, TEXTURE_FILTERING_LINEAR);
    image_delete(lightbulb_image);
    image_t* camera_image = loader_load_image("images/camera.png");
    editor_camera_texture = texture_new_from_image(camera_image, TEXTURE_WRAPPING_CLAMP, TEXTURE_FILTERING_LINEAR);
    image_delete(camera_image);

    scene_t* scene = app_get_scene();
    scene_register_component(scene, "editor_billboard", editorbillboardcomponent_get_attachment(), sizeof(editor_billboard_component_t));
    scene_register_system(scene, editor_system, NULL);
    scene_add_component_added_function(scene, "light", editor_on_light_component_added);
    scene_add_component_removed_function(scene, "light", editor_on_light_component_removed);
    scene_add_component_added_function(scene, "camera", editor_on_camera_component_added);
    scene_add_component_removed_function(scene, "camera", editor_on_camera_component_removed);

    entity_t* editor_camera = scene_add_entity(scene, "[editor] editor camera");
    scene_add_component(scene, editor_camera, "transform");
    scene_add_component(scene, editor_camera, "camera");
    lua_script_component_t* script = scene_add_component(scene, editor_camera, "lua_script");
    script->script = script_new_from_file("scripts/editor_camera.lua", editor_camera);

    array_add(editor_excluded_entities, editor_camera);

    editor_add_function(panel_menu_bar, NULL);
    editor_add_function(panel_general_info, NULL);
    editor_add_function(panel_scene_inspector, NULL);
    editor_add_function(panel_entity_inspector, NULL);
    editor_add_function(panel_renderer_inspector, NULL);
    editor_add_function(panel_viewport, NULL);
}

void editor_delete() {
    texture_delete(editor_lightbulb_texture);
    array_delete(editor_excluded_entities);
}

array_t* editor_get_excluded_entities() {
    return editor_excluded_entities;
}

void editor_set_viewport_hovered(bool hovered) {
    editor_viewport_hovered = hovered;
}

void editor_set_scene_path(const char* path) {
    strncpy(editor_scene_path, path, 1024);
}

void editor_set_selected_entity(entity_t* entity) {
    editor_selected_entity = entity;
}

bool editor_is_viewport_hovered() {
    return editor_viewport_hovered;
}

char* editor_get_scene_path() {
    return editor_scene_path;
}

entity_t* editor_get_selected_entity() {
    return editor_selected_entity;
}
