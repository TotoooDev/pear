#ifdef PEAR_PLATFORM_OPENGL

#include <graphics/window.h>
#include <event/event_dispatcher.h>
#include <core/log.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>

typedef struct window_t {
    GLFWwindow* window;
} window_t;

void window_close_callback(GLFWwindow* window) {
    event_send(EVENT_TYPE_QUIT, NULL);
}

void window_resize_callback(GLFWwindow* window, i32 width, i32 height) {
    window_resized_event_t event = {
        .width = width,
        .height = height
    };

    event_send(EVENT_TYPE_WINDOW_RESIZED, &event);
}

void window_key_callback(GLFWwindow* window, i32 key, i32 scancode, i32 action, i32 mods) {
    if (action == GLFW_PRESS) {
        key_down_event_t event = {
            .key = key
        };
        event_send(EVENT_TYPE_KEY_DOWN, &event);
    }
    else if (action == GLFW_RELEASE) {
        key_up_event_t event = {
            .key = key
        };
        event_send(EVENT_TYPE_KEY_UP, &event);
    }
}

void window_mouse_movement_callback(GLFWwindow* window, f64 x, f64 y) {
    mouse_moved_event_t event = {
        .x = x,
        .y = y
    };

    event_send(EVENT_TYPE_MOUSE_MOVED, &event);
}

void window_mouse_button_callback(GLFWwindow* window, i32 button, i32 action, i32 mods) {
    if (action == GLFW_PRESS) {
        button_down_event_t event = {
            .button = button
        };
        event_send(EVENT_TYPE_BUTTON_DOWN, &event);
    }
    else if (action == GLFW_RELEASE) {
        button_up_event_t event = {
            .button = button
        };
        event_send(EVENT_TYPE_BUTTON_UP, &event);
    }
}

void window_scale_callback(GLFWwindow* window, f32 scale_x, f32 scale_y) {
    window_scale_changed_event_t event = {
        .scale_x = scale_x,
        .scale_y = scale_y
    };

    event_send(EVENT_TYPE_WINDOW_SCALE_CHANGED, &event);
}


window_t* window_new(const char* title, u32 width, u32 height) {
    if (glfwInit() != GLFW_TRUE) {
        PEAR_ERROR("failed to initialize glfw!");
        return NULL;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window_t* window = (window_t*)malloc(sizeof(window_t));

    window->window = glfwCreateWindow(width, height, title, NULL, NULL);

    if (window->window == NULL)
        PEAR_ERROR("failed to create window!");
    glfwMakeContextCurrent(window->window);

    glfwSetWindowCloseCallback(window->window, window_close_callback);
    glfwSetWindowSizeCallback(window->window, window_resize_callback);
    glfwSetWindowContentScaleCallback(window->window, window_scale_callback);
    glfwSetKeyCallback(window->window, window_key_callback);
    glfwSetMouseButtonCallback(window->window, window_mouse_button_callback);
    glfwSetCursorPosCallback(window->window, window_mouse_movement_callback);

    return window;
}

void window_delete(window_t* window) {
    glfwDestroyWindow(window->window);
    glfwTerminate();
    free(window);
}

void window_update(window_t* window) {
    glfwSwapBuffers(window->window);
    glfwPollEvents();
}

u32 window_get_width(window_t* window) {
    i32 width;
    glfwGetWindowSize(window->window, &width, NULL);
    return width;
}

u32 window_get_height(window_t* window) {
    i32 height;
    glfwGetWindowSize(window->window, NULL, &height);
    return height;
}

f32 window_get_scale_x(window_t* window) {
    f32 scale;
    glfwGetWindowContentScale(window->window, &scale, NULL);
    return scale;
}

f32 window_get_scale_y(window_t* window) {
    f32 scale;
    glfwGetWindowContentScale(window->window, NULL, &scale);
    return scale;
}

#endif
