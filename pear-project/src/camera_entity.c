#include <camera_entity.h>
#include <scene/components/transform.h>
#include <scene/components/script.h>
#include <event/event_dispatcher.h>
#include <event/keyboard.h>
#include <graphics/camera.h>

#include <graphics/platform/opengl/window.h>
#include <core/app.h>

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

entity_t* cameraentity_create(scene_t* scene) {
    entity_t* entity = scene_add_entity(scene, "camera", ENTITY_COMPONENT_TRANSFORM, ENTITY_COMPONENT_CAMERA, ENTITY_COMPONENT_SCRIPT, ENTITY_COMPONENT_END);

    script_component_t* script = (script_component_t*)entity_get_component(entity, ENTITY_COMPONENT_SCRIPT);
    script->on_start = cameraentity_on_start;
    script->on_update = cameraentity_on_update;

    event_subscribe(cameraentity_on_event, entity);

    return entity;
}
