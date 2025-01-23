#include <core/app.h>
#include <panels/menu_bar.h>
#include <panels/general_info.h>
#include <panels/scene_inspector.h>
#include <panels/entity_inspector.h>
#include <panels/viewport.h>
#include <graphics/editor/editor.h>
#include <scene/components/lua_script.h>
#include <event/event_dispatcher.h>

void on_event(event_type_t type, void* e, void* user_data) {
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

    panel_scene_inspector_clear_excluded_entities();
    panel_menu_bar_clear_excluded_entities();

    panel_scene_inspector_exclude_entity(editor_camera);
    panel_menu_bar_exclude_entity(editor_camera);
}

int main(int argc, char* argv[]) {
    app_init();

    event_subscribe(on_event, NULL);

    scene_t* scene = app_get_scene();
    entity_t* editor_camera = scene_add_entity(scene, "[editor] editor camera");
    scene_add_component(scene, editor_camera, "transform");
    scene_add_component(scene, editor_camera, "camera");
    lua_script_component_t* script = scene_add_component(scene, editor_camera, "lua_script");
    script->script = script_new_from_file("scripts/editor_camera.lua", editor_camera);

    panel_scene_inspector_init();
    panel_scene_inspector_exclude_entity(editor_camera);
    panel_menu_bar_init();
    panel_menu_bar_exclude_entity(editor_camera);

    panel_scene_inspector_set_scene(scene);
    panel_entity_inspector_set_scene(scene);

    editor_add_function(panel_menu_bar, NULL);
    editor_add_function(panel_general_info, NULL);
    editor_add_function(panel_scene_inspector, NULL);
    editor_add_function(panel_entity_inspector, NULL);
    editor_add_function(panel_viewport, NULL);
    

    app_run();

    panel_scene_inspector_free();

    return 0;
}
