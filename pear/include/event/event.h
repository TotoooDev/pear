#ifndef PEAR_EVENT_H_
#define PEAR_EVENT_H_

#include <scene/scene.h>
#include <core/types.h>

typedef enum event_type_t {
    EVENT_TYPE_QUIT,
    EVENT_TYPE_WINDOW_RESIZED,
    EVENT_TYPE_WINDOW_SCALE_CHANGED,
    EVENT_TYPE_KEY_DOWN,
    EVENT_TYPE_KEY_UP,
    EVENT_TYPE_BUTTON_DOWN,
    EVENT_TYPE_BUTTON_UP,
    EVENT_TYPE_MOUSE_MOVED,
    EVENT_TYPE_SCENE_NEW
} event_type_t;

typedef struct window_resized_event_t {
    u32 width;
    u32 height;
} window_resized_event_t;

typedef struct window_scale_changed_event_t {
    f32 scale_x;
    f32 scale_y;
} window_scale_changed_event_t;

typedef struct key_down_event_t {
    u32 key;
} key_down_event_t;

typedef struct key_up_event_t {
    u32 key;
} key_up_event_t;

typedef struct button_down_event_t {
    u32 button;
} button_down_event_t;

typedef struct button_up_event_t {
    u32 button;
} button_up_event_t;

typedef struct mouse_moved_event_t {
    f32 x;
    f32 y;
    f32 rel_x;
    f32 rel_y;
} mouse_moved_event_t;

typedef struct scene_new_event_t {
    scene_t* old_scene;
    scene_t* new_scene;
} scene_new_event_t;

#endif
