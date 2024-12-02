#include <pear.h>
#include <scene/scene.h>
#include <scene/components/transform.h>
#include <graphics/window.h>
#include <core/app.h>
#include <core/timer.h>
#include <core/log.h>

int main(int argc, char* argv[]) {
    pear_print_hello();
    app_init();

    scene_t* scene = app_get_scene();
    entity_t* entity = scene_add_entity(scene, ENTITY_COMPONENT_TRANSFORM, ENTITY_COMPONENT_DRAWABLE, ENTITY_COMPONENT_END);
    PEAR_INFO("%d", entity_has_component(entity, ENTITY_COMPONENT_END));
    PEAR_INFO("%d", entity_has_component(entity, ENTITY_COMPONENT_TRANSFORM));
    PEAR_INFO("%d", entity_has_component(entity, ENTITY_COMPONENT_DRAWABLE));
    PEAR_INFO("%d", entity_has_component(entity, ENTITY_COMPONENT_CAMERA));

    transform_component_t* transform = entity_get_component(entity, ENTITY_COMPONENT_TRANSFORM);
    PEAR_INFO("%f, %f, %f", transform->pos[0], transform->pos[1], transform->pos[2]);

    app_run();

    return 0;
}
