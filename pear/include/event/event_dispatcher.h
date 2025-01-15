#ifndef PEAR_EVENT_DISPATCHER_H_
#define PEAR_EVENT_DISPATCHER_H_

#include <event/event.h>

typedef void(*event_function_t)(event_type_t, void*, void*);

void event_init();
void event_free();
void event_subscribe(event_function_t function, void* user_data);
void event_unsubscribe(event_function_t function, void* user_data);
void event_send(event_type_t type, void* event);

#endif
