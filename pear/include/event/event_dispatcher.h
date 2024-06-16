#ifndef PEAR_EVENT_DISPATCHER_H
#define PEAR_EVENT_DISPATCHER_H

#include <event/events.h>

typedef void(*EventFunction)(EventType, void*, void*);

void event_subscribe(EventFunction function, void* user_data);
void event_send(EventType type, void* event);

#endif
