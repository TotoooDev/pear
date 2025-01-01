#include <scene/scene.h>
#include <graphics/editor/editor.h>
#include <graphics/editor/scene_inspector.h>
#include <core/app.h>

#include <entities.h> 

#include <script/script.h>

int main(int argc, char* argv[]) {
    script_t* script = script_new_from_file("scripts/script.lua");
    script_delete(script);

    app_init();

    scene_t* scene = app_get_scene();

    modelentity_create(scene);
    cameraentity_create(scene);
    lightentity_create(scene);
    skyboxentity_create(scene);

    #ifdef PEAR_ENABLE_EDITOR
        // editor_enable_demo(true);
        editor_set_scene(scene);
    #endif

    app_run();

    return 0;
}
