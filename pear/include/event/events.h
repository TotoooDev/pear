#ifndef PEAR_EVENT_EVENTS
#define PEAR_EVENT_EVENTS

#include <core/types.h>

typedef enum EventType {
    EVENT_TYPE_QUIT,
    EVENT_TYPE_WINDOW_RESIZED,
    EVENT_TYPE_KEY_DOWN,
    EVENT_TYPE_KEY_UP
} EventType;

typedef struct WindowResizedEvent {
    i32 width;
    i32 height;
} WindowResizedEvent;

typedef struct KeyDownEvent {
    i32 key;
} KeyDownEvent;

typedef struct KeyUpEvent {
    i32 key;
} KeyUpEvent;

#endif
