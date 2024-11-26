#include <pear.h>
#include <scene/scene.h>
#include <scene/types/foo.h>
#include <scene/types/bar.h>
#include <graphics/window.h>

int main(int argc, char* argv[]) {
    pear_print_hello();

    foo_creation_data_t foo_data = { .value = 1 };
    bar_creation_data_t bar_data = { .value = 2 };

    scene_t* scene = scene_new();
    entity_t* entity1 = scene_add_entity(scene, ENTITY_TYPE_FOO, &foo_data);
    entity_t* entity2 = scene_add_entity(scene, ENTITY_TYPE_BAR, &bar_data);

    scene_update(scene, 0.0f);

    foo_set_value((foo_t*)entity_get_data(entity1), 3);
    bar_set_value((bar_t*)entity_get_data(entity2), 4);

    scene_update(scene, 0.0f);

    scene_remove_entity(scene, entity1);
    scene_remove_entity(scene, entity2);
    scene_delete(scene);

    window_t* window = window_new("pear engine", 1080, 720);
    window_update(window);
    window_delete(window);

    return 0;
}
