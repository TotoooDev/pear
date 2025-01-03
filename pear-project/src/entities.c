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

static struct {
    bool forward;
    bool backward;
    bool right;
    bool left;
} keys_down;

static bool lock_mouse = true;

void cameraentity_on_event(event_type_t type, void* e, void* user_data) {
    entity_t* entity = (entity_t*)user_data;

    switch (type) {
    case EVENT_TYPE_KEY_DOWN: {
        key_down_event_t* event = (key_down_event_t*)e;
        if (event->key == PEAR_KEY_W) {
            keys_down.forward = true;
        }
        if (event->key == PEAR_KEY_S) {
            keys_down.backward = true;
        }
        if (event->key == PEAR_KEY_A) {
            keys_down.left = true;
        }
        if (event->key == PEAR_KEY_D) {
            keys_down.right = true;
        }
        if (event->key == PEAR_KEY_LEFT_ALT || event->key == PEAR_KEY_RIGHT_ALT) {
            #ifdef PEAR_PLATFORM_GLFW
                window_lock_mouse(app_get_window(), lock_mouse);
                lock_mouse = !lock_mouse;
            #endif
        }
        break;
    }

    case EVENT_TYPE_KEY_UP: {
        key_up_event_t* event = (key_up_event_t*)e;
        if (event->key == PEAR_KEY_W) {
            keys_down.forward = false;
        }
        if (event->key == PEAR_KEY_S) {
            keys_down.backward = false;
        }
        if (event->key == PEAR_KEY_A) {
            keys_down.left = false;
        }
        if (event->key == PEAR_KEY_D) {
            keys_down.right = false;
        }
        break;
    }

    case EVENT_TYPE_MOUSE_MOVED: {
        if (!lock_mouse) {
            mouse_moved_event_t* event = (mouse_moved_event_t*)e;
            transform_component_t* transform = (transform_component_t*)entity_get_component(entity, ENTITY_COMPONENT_TRANSFORM);
            transform->rotation[0] += event->rel_x;
            transform->rotation[1] -= event->rel_y;
            if (transform->rotation[1] > 89.0f) {
                transform->rotation[1] = 89.0f;
            }
            if (transform->rotation[1] < -89.0f) {
                transform->rotation[1] = -89.0f;
            }
        }
        break;
    }
    }
}

void cameraentity_on_start(entity_t* entity, f32 timestep) {
    keys_down.forward = false;
    keys_down.backward = false;
    keys_down.right = false;
    keys_down.left = false;
}

void cameraentity_on_update(entity_t* entity, f32 timestep) {
    transform_component_t* transform = (transform_component_t*)entity_get_component(entity, ENTITY_COMPONENT_TRANSFORM);
    if (keys_down.forward) {
        vec3 front;
        camera_get_front(transform->rotation[0], transform->rotation[1], transform->rotation[2], front);
        transform->pos[0] += front[0] * timestep * 0.01f;
        transform->pos[1] += front[1] * timestep * 0.01f;
        transform->pos[2] += front[2] * timestep * 0.01f;
    }
    if (keys_down.backward) {
        vec3 front;
        camera_get_front(transform->rotation[0], transform->rotation[1], transform->rotation[2], front);
        transform->pos[0] -= front[0] * timestep * 0.01f;
        transform->pos[1] -= front[1] * timestep * 0.01f;
        transform->pos[2] -= front[2] * timestep * 0.01f;
    }
    if (keys_down.right) {
        vec3 right;
        camera_get_right(transform->rotation[0], transform->rotation[1], transform->rotation[2], right);
        transform->pos[0] += right[0] * timestep * 0.01f;
        transform->pos[1] += right[1] * timestep * 0.01f;
        transform->pos[2] += right[2] * timestep * 0.01f;
    }
    if (keys_down.left) {
        vec3 right;
        camera_get_right(transform->rotation[0], transform->rotation[1], transform->rotation[2], right);
        transform->pos[0] -= right[0] * timestep * 0.01f;
        transform->pos[1] -= right[1] * timestep * 0.01f;
        transform->pos[2] -= right[2] * timestep * 0.01f;
    }
}

void cameraentity_create(scene_t* scene) {
    entity_t* entity = scene_add_entity(scene, "camera", ENTITY_COMPONENT_TRANSFORM, ENTITY_COMPONENT_CAMERA, ENTITY_COMPONENT_SCRIPT, ENTITY_COMPONENT_END);

    transform_component_t* transform = (transform_component_t*)entity_get_component(entity, ENTITY_COMPONENT_TRANSFORM);
    transform->pos[1] = 1.0f;

    script_component_t* script = (script_component_t*)entity_get_component(entity, ENTITY_COMPONENT_SCRIPT);
    script->on_start = cameraentity_on_start;
    script->on_update = cameraentity_on_update;

    event_subscribe(cameraentity_on_event, entity);
}

void modelentity_create(scene_t* scene) {
    entity_t* entity = scene_add_entity(scene, "model", ENTITY_COMPONENT_TRANSFORM, ENTITY_COMPONENT_MODEL, ENTITY_COMPONENT_LUA_SCRIPT, ENTITY_COMPONENT_END);
    model_component_t* model_comp = entity_get_component(entity, ENTITY_COMPONENT_MODEL);
    model_comp->model = loader_load_gltf("Avocado.glb");
    transform_component_t* transform = entity_get_component(entity, ENTITY_COMPONENT_TRANSFORM);
    transform->scale[0] = 50.0f;
    transform->scale[1] = 50.0f;
    transform->scale[2] = 50.0f;
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
