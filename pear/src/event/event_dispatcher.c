#include <event/event_dispatcher.h>
#include <core/types.h>
#include <core/log.h>
#include <core/alloc.h>

#define EVENT_NUM_MAX_FUNCTIONS 1024

static event_function_t event_functions[EVENT_NUM_MAX_FUNCTIONS];
static void* event_user_datas[EVENT_NUM_MAX_FUNCTIONS];
static u32 event_num_functions = 0;

void event_subscribe(event_function_t function, void* user_data) {
    if (event_num_functions >= EVENT_NUM_MAX_FUNCTIONS) {
        PEAR_ERROR("unable to subscribe to event! the event arrays are full (%d functions max).", EVENT_NUM_MAX_FUNCTIONS);
        return;
    }

    if (function == NULL)
        PEAR_WARN("subscribing with a null function pointer!");

    event_functions[event_num_functions] = function;
    event_user_datas[event_num_functions] = user_data;

    event_num_functions++;
}

void event_send(event_type_t type, void* event) {
    for (u32 i = 0; i < event_num_functions; i++) {
        event_function_t function = event_functions[i];
        void* user_data = event_user_datas[i];
        function(type, event, user_data);
    }
}
