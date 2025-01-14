#include <event/event_dispatcher.h>
#include <util/array.h>
#include <core/types.h>
#include <core/log.h>
#include <core/alloc.h>

static array_t* event_functions;
static array_t* event_user_datas;

void event_init() {
    event_functions = array_new(10);
    event_user_datas = array_new(10);
}

void event_free() {
    array_delete(event_functions);
    array_delete(event_user_datas);
}

void event_subscribe(event_function_t function, void* user_data) {
    if (function == NULL) {
        PEAR_WARN("subscribing with a null function pointer!");
    }

    array_add(event_functions, function);
    array_add(event_user_datas, user_data);
}

void event_unsubscribe(event_function_t function) {
    u32 index = array_get_index(event_functions, function);
    array_remove_index(event_functions, index);
    array_remove_index(event_user_datas, index);
}

void event_send(event_type_t type, void* event) {
    for (u32 i = 0; i < array_get_length(event_functions); i++) {
        event_function_t function = array_get(event_functions, i);
        void* user_data = array_get(event_user_datas, i);
        function(type, event, user_data);
    }
}
