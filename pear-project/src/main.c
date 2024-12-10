#include <pear.h>
#include <scene/scene.h>
#include <scene/components/transform.h>
#include <scene/components/model.h>
#include <scene/components/camera.h>
#include <scene/components/script.h>
#include <graphics/window.h>
#include <graphics/gui/gui.h>
#include <graphics/gui/scene_inspector.h>
#include <graphics/gui/info.h>
#include <core/app.h>
#include <core/timer.h>
#include <core/log.h>

void on_start(entity_t* entity, f32 timestep) {
    PEAR_INFO("i am created!");
}

void on_update(entity_t* entity, f32 timestep) {
    transform_component_t* transform = (transform_component_t*)entity_get_component(entity, ENTITY_COMPONENT_TRANSFORM);
    transform->rotation[1] += 0.004f * timestep;
}

void on_end(entity_t* entity, f32 timestep) {
    PEAR_INFO("i am deleted!");
}

int main(int argc, char* argv[]) {
    pear_print_hello();
    app_init();

    scene_t* scene = app_get_scene();
    entity_t* entity = scene_add_entity(scene, "entity", ENTITY_COMPONENT_SCRIPT, ENTITY_COMPONENT_MODEL, ENTITY_COMPONENT_TRANSFORM, ENTITY_COMPONENT_END);

    script_component_t* script = (script_component_t*)entity_get_component(entity, ENTITY_COMPONENT_SCRIPT);
    script->on_start = on_start;
    script->on_update = on_update;
    script->on_end = on_end;

    bool success;
    pear_model_t model = pear_model_load("backpack/backpack.model", &success);
    if (!success) {
        PEAR_ERROR("failed to load model cube.model!");
    }

    model_component_t* model_comp = (model_component_t*)entity_get_component(entity, ENTITY_COMPONENT_MODEL);
    model_comp->model = model_new_from_pear_model(model);
    pear_model_free(&model);

    entity_t* camera = scene_add_entity(scene, "camera", ENTITY_COMPONENT_TRANSFORM, ENTITY_COMPONENT_CAMERA, ENTITY_COMPONENT_END);
    transform_component_t* transform = (transform_component_t*)entity_get_component(camera, ENTITY_COMPONENT_TRANSFORM);
    transform->pos[2] = 5.0f;
    transform->rotation[0] = -90.0f;

    gui_add(gui_info, NULL);
    gui_add(gui_scene_inspector, NULL);
    gui_scene_inspector_set_scene(scene);

    app_run();

    return 0;
}
