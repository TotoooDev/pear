#include <pear.h>
#include <scene/scene.h>

int main(int argc, char* argv[]) {
    pear_print_hello();

    scene_t* scene = scene_new();
    entity_t* entity1 = scene_add_entity(scene, ENTITY_TYPE_FOO);
    entity_t* entity2 = scene_add_entity(scene, ENTITY_TYPE_BAR);

    scene_update(scene, 0.0f);

    scene_remove_entity(scene, entity1);
    scene_remove_entity(scene, entity2);
    scene_delete(scene);

    return 0;
}
