#include <pear.h>
#include <scene/scene.h>
#include <scene/components/script.h>
#include <graphics/window.h>
#include <core/app.h>
#include <core/timer.h>
#include <core/log.h>

void on_start(entity_t* entity, f32 timestep) {
    PEAR_INFO("i am created!");
}

void on_update(entity_t* entity, f32 timestep) {
    PEAR_INFO("i am updated!");
}

void on_end(entity_t* entity, f32 timestep) {
    PEAR_INFO("i am deleted!");
}

int main(int argc, char* argv[]) {
    pear_print_hello();
    app_init();

    scene_t* scene = app_get_scene();
    entity_t* entity = scene_add_entity(scene, ENTITY_COMPONENT_SCRIPT, ENTITY_COMPONENT_END);
    script_component_t* script = (script_component_t*)entity_get_component(entity, ENTITY_COMPONENT_SCRIPT);
    script->on_start = on_start;
    script->on_update = on_update;
    script->on_end = on_end;

    app_run();

    return 0;
}
