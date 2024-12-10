#include <model_entity.h>
#include <scene/components/transform.h>
#include <scene/components/script.h>
#include <scene/components/model.h>
#include <core/log.h>

void on_start(entity_t* entity, f32 timestep) {
    PEAR_INFO("i am created!");
}

void on_update(entity_t* entity, f32 timestep) {
    transform_component_t* transform = (transform_component_t*)entity_get_component(entity, ENTITY_COMPONENT_TRANSFORM);
    // transform->rotation[1] += 0.004f * timestep;
}

void on_end(entity_t* entity, f32 timestep) {
    PEAR_INFO("i am deleted!");
}

entity_t* modelentity_create(scene_t* scene) {
    entity_t* entity = scene_add_entity(scene, "entity", ENTITY_COMPONENT_SCRIPT, ENTITY_COMPONENT_MODEL, ENTITY_COMPONENT_TRANSFORM, ENTITY_COMPONENT_END);

    script_component_t* script = (script_component_t*)entity_get_component(entity, ENTITY_COMPONENT_SCRIPT);
    script->on_start = on_start;
    script->on_update = on_update;
    script->on_end = on_end;

    bool success;
    pear_model_t model = pear_model_load("sponza/sponza.model", &success);
    if (!success) {
        PEAR_ERROR("failed to load model cube.model!");
    }

    model_component_t* model_comp = (model_component_t*)entity_get_component(entity, ENTITY_COMPONENT_MODEL);
    model_comp->model = model_new_from_pear_model(model);
    pear_model_free(&model);

    transform_component_t* transform = (transform_component_t*)entity_get_component(entity, ENTITY_COMPONENT_TRANSFORM);
    transform->scale[0] = 0.01f;
    transform->scale[1] = 0.01f;
    transform->scale[2] = 0.01f;

    return entity;
}
