#include <pear.h>
#include <scene/scene.h>
#include <graphics/gui/gui.h>
#include <graphics/gui/scene_inspector.h>
#include <graphics/gui/info.h>
#include <core/app.h>

#include <model_entity.h>
#include <camera_entity.h>

int main(int argc, char* argv[]) {
    app_init();

    scene_t* scene = app_get_scene();
    modelentity_create(scene);
    cameraentity_create(scene);

    gui_add(gui_info, NULL);
    gui_add(gui_scene_inspector, NULL);
    gui_scene_inspector_set_scene(scene);

    app_run();

    return 0;
}
