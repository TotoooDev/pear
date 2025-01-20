#include <core/app.h>
#include <core/log.h>
#include <panels/general_info.h>
#include <panels/scene_inspector.h>
#include <graphics/editor/editor.h>

int main(int argc, char* argv[]) {
    PEAR_INFO("hello editor!");

    app_init();
    editor_add_function(panel_general_info, NULL);
    editor_add_function(panel_scene_inspector, NULL);
    app_run();

    return 0;
}
