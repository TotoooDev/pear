#ifndef PEAR_EVENT_EVENTS
#define PEAR_EVENT_EVENTS

#include <core/types.h>

typedef enum EventType {
    EVENT_TYPE_QUIT,
    EVENT_TYPE_WINDOW_RESIZED,
    EVENT_TYPE_WINDOW_SCALE_CHANGED,
    EVENT_TYPE_KEY_DOWN,
    EVENT_TYPE_KEY_UP,
    EVENT_TYPE_MOUSE_MOVED,
    EVENT_TYPE_MOUSE_BUTTON_DOWN,
    EVENT_TYPE_MOUSE_BUTTON_UP
} EventType;

typedef struct WindowResizedEvent {
    i32 width;
    i32 height;
} WindowResizedEvent;

typedef struct WindowScaleChanged {
    f32 scale_x;
    f32 scale_y;
} WindowScaleChanged;

typedef struct KeyDownEvent {
    i32 key;
} KeyDownEvent;

typedef struct KeyUpEvent {
    i32 key;
} KeyUpEvent;

typedef struct MouseMovedEvent {
    f32 x;
    f32 y;
} MouseMovedEvent;

typedef struct MouseButtonDownEvent {
    i32 button;
} MouseButtonDownEvent;

typedef struct MouseButtonUpEvent {
    i32 button;
} MouseButtonUpEvent;

#endif
