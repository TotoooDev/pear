#include <scene/scene.h>
#include <graphics/gui/gui.h>
#include <graphics/gui/scene_inspector.h>
#include <graphics/gui/component_inspector.h>
#include <graphics/gui/renderer.h>
#include <graphics/gui/info.h>
#include <core/app.h>

#include <model_entity.h>
#include <camera_entity.h>

#include <scene/components/light.h>
#include <scene/components/transform.h>

int main(int argc, char* argv[]) {
    app_init();

    scene_t* scene = app_get_scene();
    modelentity_create(scene);
    cameraentity_create(scene);

    entity_t* entity = scene_add_entity(scene, "light", ENTITY_COMPONENT_TRANSFORM, ENTITY_COMPONENT_LIGHT, ENTITY_COMPONENT_END);

    light_component_t* light = (light_component_t*)entity_get_component(entity, ENTITY_COMPONENT_LIGHT);
    light->light.type = LIGHT_TYPE_DIRECTIONAL;

    transform_component_t* transform = entity_get_component(entity, ENTITY_COMPONENT_TRANSFORM);
    transform->rotation[0] = 0.1f;
    transform->rotation[1] = -1.0f;

    gui_add(gui_info, NULL);
    gui_add(gui_renderer, NULL);
    gui_add(gui_component_inspector, NULL);
    gui_add(gui_scene_inspector, NULL);
    gui_scene_inspector_set_scene(scene);

    app_run();

    return 0;
}
