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
#include <scene/components/skybox.h>
#include <pear_image.h>

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

    entity_t* skybox_entity = scene_add_entity(scene, "skybox", ENTITY_COMPONENT_SKYBOX, ENTITY_COMPONENT_END);

    bool success;
    pear_image_t right = pear_image_load("skybox/right.image", &success);
    pear_image_t left = pear_image_load("skybox/left.image", &success);
    pear_image_t top = pear_image_load("skybox/top.image", &success);
    pear_image_t bottom = pear_image_load("skybox/bottom.image", &success);
    pear_image_t front = pear_image_load("skybox/front.image", &success);
    pear_image_t back = pear_image_load("skybox/back.image", &success);

    image_t* images[] = {
        image_new_from_pear_image(right),
        image_new_from_pear_image(left),
        image_new_from_pear_image(top),
        image_new_from_pear_image(bottom),
        image_new_from_pear_image(front),
        image_new_from_pear_image(back)
    };

    skybox_component_t* skybox = entity_get_component(skybox_entity, ENTITY_COMPONENT_SKYBOX);
    skybox->cubemap = cubemap_new_from_images(images, TEXTURE_WRAPPING_CLAMP, TEXTURE_FILTERING_LINEAR);

    gui_add(gui_info, NULL);
    gui_add(gui_renderer, NULL);
    gui_add(gui_component_inspector, NULL);
    gui_add(gui_scene_inspector, NULL);
    gui_scene_inspector_set_scene(scene);

    app_run();

    return 0;
}
