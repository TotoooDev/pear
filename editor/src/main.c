#include <core/app.h>
#include <core/log.h>
#include <panels/general_info.h>
#include <panels/scene_inspector.h>
#include <panels/entity_inspector.h>
#include <panels/viewport.h>
#include <graphics/editor/editor.h>

int main(int argc, char* argv[]) {
    PEAR_INFO("hello editor!");

    app_init();

    panel_scene_inspector_set_scene(app_get_scene());

    editor_add_function(panel_general_info, NULL);
    editor_add_function(panel_scene_inspector, NULL);
    editor_add_function(panel_entity_inspector, NULL);
    editor_add_function(panel_viewport, NULL);
    
    app_run();

    return 0;
}
