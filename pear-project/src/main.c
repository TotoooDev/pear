#include <scene/scene.h>
#include <graphics/editor/editor.h>
#include <core/app.h>

#include <entities.h> 

int main(int argc, char* argv[]) {
    app_init();

    #ifdef PEAR_DEBUG
        editor_enable_demo(true);
    #endif

    scene_t* scene = app_get_scene();
    modelentity_create(scene);
    cameraentity_create(scene);
    lightentity_create(scene);
    skyboxentity_create(scene);

    app_run();

    return 0;
}
