#ifndef PEAR_EVENT_EVENTS
#define PEAR_EVENT_EVENTS

#include <core/types.h>

typedef enum EventType {
    EVENT_TYPE_QUIT,
    EVENT_TYPE_WINDOW_RESIZED
} EventType;

typedef struct WindowResizedEvent {
    i32 width;
    i32 height;
} WindowResizedEvent;

#endif
