#include <pear.h>
#include <scene/scene.h>
#include <scene/types/foo.h>
#include <scene/types/bar.h>
#include <graphics/window.h>
#include <core/app.h>
#include <core/timer.h>
#include <core/log.h>

int main(int argc, char* argv[]) {
    pear_print_hello();
    app_init();

    foo_creation_data_t foo_data = { .value = 1 };
    bar_creation_data_t bar_data = { .value = 2 };

    scene_t* scene = app_get_scene();
    // entity_t* entity1 = scene_add_entity(scene, ENTITY_TYPE_FOO, &foo_data);
    // entity_t* entity2 = scene_add_entity(scene, ENTITY_TYPE_BAR, &bar_data);

    app_run();

    return 0;
}
