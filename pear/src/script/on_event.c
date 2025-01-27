#include <script/on_event.h>
#include <script/script.h>
#include <event/keyboard.h>
#include <event/mouse.h>

void script_on_key_down(script_t* script, lua_State* state, void* e) {
    key_down_event_t* event = (key_down_event_t*)e;

    script_begin_table(script, "key");
        script_set_string(script, event_get_key_string(event->key), event_get_key_string(event->key));
        script_begin_table(script, "state");
            script_set_bool(script, true, event_get_key_string(event->key));
        script_end_table(script);
    script_end_table(script);

    lua_getglobal(state, "on_key_press");
    if (lua_isfunction(state, -1)) {
        lua_pushstring(state, event_get_key_string(event->key));
        lua_pcall(state, 1, 0, 0);
    }
    else {
        lua_pop(state, -1);
    }
}

void script_on_key_up(script_t* script, lua_State* state, void* e) {
    key_up_event_t* event = (key_up_event_t*)e;

    script_begin_table(script, "key");
        script_begin_table(script, "state");
            script_set_bool(script, false, event_get_key_string(event->key));
        script_end_table(script);
    script_end_table(script);

    lua_getglobal(state, "on_key_release");
    if (lua_isfunction(state, -1)) {
        lua_pushstring(state, event_get_key_string(event->key));
        lua_pcall(state, 1, 0, 0);
    }
    else {
        lua_pop(state, -1);
    }
}

void script_on_mouse_moved(script_t* script, lua_State* state, void* e) {
    mouse_moved_event_t* event = (mouse_moved_event_t*)e;

    vec3 relative = { event->rel_x, event->rel_y, 0.0f };
    vec3 pos = { event->x, event->y, 0.0f };

    script_begin_table(script, "mouse");
        script_set_vec3(script, pos, "pos");
        script_set_vec3(script, relative, "relative");
    script_end_table(script);

    lua_getglobal(state, "on_mouse_movement");
    if (lua_isfunction(state, -1)) {
        lua_pcall(state, 0, 0, 0);
    }
    else {
        lua_pop(state, -1);
    }
}

void script_on_button_down(script_t* script, lua_State* state, void* e) {
    button_down_event_t* event = (button_down_event_t*)e;

    script_begin_table(script, "mouse");
        script_begin_table(script, "button");
            if (event->button == PEAR_MOUSE_BUTTON_LEFT) {
                script_set_bool(script, true, "left");
            }
            if (event->button == PEAR_MOUSE_BUTTON_RIGHT) {
                script_set_bool(script, true, "right");
            }
            if (event->button == PEAR_MOUSE_BUTTON_MIDDLE) {
                script_set_bool(script, true, "middle");
            }
        script_end_table(script);
    script_end_table(script);

    lua_getglobal(state, "on_button_down");
    if (lua_isfunction(state, -1)) {
        lua_pcall(state, 0, 0, 0);
    }
    else {
        lua_pop(state, -1);
    }
}

void script_on_button_up(script_t* script, lua_State* state, void* e) {
    button_up_event_t* event = (button_up_event_t*)e;

    script_begin_table(script, "mouse");
        script_begin_table(script, "button");
            if (event->button == PEAR_MOUSE_BUTTON_LEFT) {
                script_set_bool(script, false, "left");
            }
            if (event->button == PEAR_MOUSE_BUTTON_RIGHT) {
                script_set_bool(script, false, "right");
            }
            if (event->button == PEAR_MOUSE_BUTTON_MIDDLE) {
                script_set_bool(script, false, "middle");
            }
        script_end_table(script);
    script_end_table(script);

    lua_getglobal(state, "on_button_up");
    if (lua_isfunction(state, -1)) {
        lua_pcall(state, 0, 0, 0);
    }
    else {
        lua_pop(state, -1);
    }
}

void script_on_scroll(script_t* script, lua_State* state, void* e) {
    mouse_scrolled_event_t* event = (mouse_scrolled_event_t*)e;

    lua_getglobal(state, "on_scroll");
    if (lua_isfunction(state, -1)) {
        lua_pushnumber(state, event->x);
        lua_pushnumber(state, event->y);
        lua_pcall(state, 2, 0, 0);
    }
    else {
        lua_pop(state, -1);
    }
}

void script_on_event(event_type_t type, void* e, void* user_data) {
    script_t* script = (script_t*)user_data;
    lua_State* state = script_get_state(script);

    if (type == EVENT_TYPE_KEY_DOWN) {
        script_on_key_down(script, state, e);
    }

    if (type == EVENT_TYPE_KEY_UP) {
        script_on_key_up(script, state, e);
    }

    if (type == EVENT_TYPE_MOUSE_MOVED) {
        script_on_mouse_moved(script, state, e);
    }

    if (type == EVENT_TYPE_BUTTON_DOWN) {
        script_on_button_down(script, state, e);
    }

    if (type == EVENT_TYPE_BUTTON_UP) {
        script_on_button_up(script, state, e);
    }

    if (type == EVENT_TYPE_MOUSE_SCROLL) {
        script_on_scroll(script, state, e);
    }
}
