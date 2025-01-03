#include <scene/components/transform.h>
#include <scene/components/script.h>
#include <scene/components/model.h>
#include <scene/components/light.h>
#include <scene/components/skybox.h>
#include <scene/components/lua_script.h>
#include <event/event_dispatcher.h>
#include <event/keyboard.h>
#include <graphics/camera.h>
#include <graphics/image.h>
#include <graphics/cubemap.h>
#include <graphics/platform/opengl/window.h>
#include <loaders/model.h>
#include <loaders/image.h>
#include <core/timer.h>
#include <core/app.h>
#include <core/log.h>

void cameraentity_create(scene_t* scene) {
    entity_t* entity = scene_add_entity(scene, "camera", ENTITY_COMPONENT_TRANSFORM, ENTITY_COMPONENT_CAMERA, ENTITY_COMPONENT_LUA_SCRIPT, ENTITY_COMPONENT_END);
    lua_script_component_t* script = entity_get_component(entity, ENTITY_COMPONENT_LUA_SCRIPT);
    script->script = script_new_from_file("scripts/camera.lua");
}

void modelentity_create(scene_t* scene) {
    entity_t* entity = scene_add_entity(scene, "model", ENTITY_COMPONENT_TRANSFORM, ENTITY_COMPONENT_MODEL, ENTITY_COMPONENT_LUA_SCRIPT, ENTITY_COMPONENT_END);
    model_component_t* model_comp = entity_get_component(entity, ENTITY_COMPONENT_MODEL);
    model_comp->model = loader_load_gltf("Avocado.glb");
    lua_script_component_t* script = entity_get_component(entity, ENTITY_COMPONENT_LUA_SCRIPT);
    script->script = script_new_from_file("scripts/script.lua");
}

void lightentity_create(scene_t* scene) {
    light_component_t* light;
    transform_component_t* transform;

    entity_t* directional = scene_add_entity(scene, "light", ENTITY_COMPONENT_TRANSFORM, ENTITY_COMPONENT_LIGHT, ENTITY_COMPONENT_END);
    light = (light_component_t*)entity_get_component(directional, ENTITY_COMPONENT_LIGHT);
    light->light.type = LIGHT_TYPE_DIRECTIONAL;
    transform = entity_get_component(directional, ENTITY_COMPONENT_TRANSFORM);
    transform->rotation[0] = 0.1f;
    transform->rotation[1] = -1.0f;

    // entity_t* point_1 = scene_add_entity(scene, "point 1", ENTITY_COMPONENT_TRANSFORM, ENTITY_COMPONENT_LIGHT, ENTITY_COMPONENT_END);
    // light = (light_component_t*)entity_get_component(point_1, ENTITY_COMPONENT_LIGHT);
    // light->light.type = LIGHT_TYPE_POINT;
    // transform = entity_get_component(point_1, ENTITY_COMPONENT_TRANSFORM);
    // transform->pos[0] = 9.0f;
    // transform->pos[1] = 3.0f;
    // transform->pos[2] = -3.0f;
// 
    // entity_t* point_2 = scene_add_entity(scene, "point 2", ENTITY_COMPONENT_TRANSFORM, ENTITY_COMPONENT_LIGHT, ENTITY_COMPONENT_END);
    // light = (light_component_t*)entity_get_component(point_2, ENTITY_COMPONENT_LIGHT);
    // light->light.type = LIGHT_TYPE_POINT;
    // transform = entity_get_component(point_2, ENTITY_COMPONENT_TRANSFORM);
    // transform->pos[0] = -7.0f;
    // transform->pos[1] = 2.0f;
    // transform->pos[2] = -3.0f;
// 
    // entity_t* point_3 = scene_add_entity(scene, "point 3", ENTITY_COMPONENT_TRANSFORM, ENTITY_COMPONENT_LIGHT, ENTITY_COMPONENT_END);
    // light = (light_component_t*)entity_get_component(point_3, ENTITY_COMPONENT_LIGHT);
    // light->light.type = LIGHT_TYPE_POINT;
    // transform = entity_get_component(point_3, ENTITY_COMPONENT_TRANSFORM);
    // transform->pos[0] = 3.0f;
    // transform->pos[1] = 4.0f;
    // transform->pos[2] = 4.0f;
}

void skyboxentity_create(scene_t* scene) {
    entity_t* skybox_entity = scene_add_entity(scene, "skybox", ENTITY_COMPONENT_SKYBOX, ENTITY_COMPONENT_END);

    image_t* images[] = {
        loader_load_image("skybox/right.jpg"),
        loader_load_image("skybox/left.jpg"),
        loader_load_image("skybox/top.jpg"),
        loader_load_image("skybox/bottom.jpg"),
        loader_load_image("skybox/front.jpg"),
        loader_load_image("skybox/back.jpg")
    };

    skybox_component_t* skybox = entity_get_component(skybox_entity, ENTITY_COMPONENT_SKYBOX);
    skybox->cubemap = cubemap_new_from_images(images, TEXTURE_WRAPPING_CLAMP, TEXTURE_FILTERING_LINEAR);

    for (u32 i = 0; i < 6; i++) {
        image_delete(images[i]);
    }
}
