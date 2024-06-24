#ifdef PEAR_PLATFORM_OPENGL

#include <graphics/window.h>
#include <graphics/platform/opengl/window.h>
#include <event/event_dispatcher.h>
#include <core/log.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>

typedef struct Window {
    GLFWwindow* window;
    bool should_close;
} Window;

static u32 window_width = 1280;
static u32 window_height = 720;
static const char* window_title = "New window";

void window_close_callback(GLFWwindow* window) {
    event_send(EVENT_TYPE_QUIT, NULL);
}

void window_resize_callback(GLFWwindow* window, i32 width, i32 height) {
    WindowResizedEvent event = {
        .width = width,
        .height = height
    };

    event_send(EVENT_TYPE_WINDOW_RESIZED, &event);
}

void window_key_callback(GLFWwindow* window, i32 key, i32 scancode, i32 action, i32 mods) {
    if (action == GLFW_PRESS) {
        KeyDownEvent event = {
            .key = key
        };
        event_send(EVENT_TYPE_KEY_DOWN, &event);
    }
    else if (action == GLFW_RELEASE) {
        KeyUpEvent event = {
            .key = key
        };
        event_send(EVENT_TYPE_KEY_UP, &event);
    }
}

void window_mouse_movement_callback(GLFWwindow* window, f64 x, f64 y) {
    MouseMovedEvent event = {
        .x = x,
        .y = y
    };

    event_send(EVENT_TYPE_MOUSE_MOVED, &event);
}

void window_mouse_button_callback(GLFWwindow* window, i32 button, i32 action, i32 mods) {
    if (action == GLFW_PRESS) {
        MouseButtonDownEvent event = {
            .button = button
        };
        event_send(EVENT_TYPE_MOUSE_BUTTON_DOWN, &event);
    }
    else if (action == GLFW_RELEASE) {
        MouseButtonUpEvent event = {
            .button = button
        };
        event_send(EVENT_TYPE_MOUSE_BUTTON_UP, &event);
    }
}

void window_scale_callback(GLFWwindow* window, f32 scale_x, f32 scale_y) {
    WindowScaleChanged event = {
        .scale_x = scale_x,
        .scale_y = scale_y
    };

    event_send(EVENT_TYPE_WINDOW_SCALE_CHANGED, &event);
}

void window_init_opengl_debug_output() {
#ifdef PEAR_DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif
}

void window_set_width(u32 width) {
    window_width = width;
}

void window_set_height(u32 height) {
    window_height = height;
}

void window_set_title(const char* title) {
    window_title = title;
}

Window* window_new() {
    if (glfwInit() != GLFW_TRUE)
        PEAR_ERROR("failed to initialize glfw!");
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window_init_opengl_debug_output();

    Window* window = (Window*)malloc(sizeof(Window));

    window->window = glfwCreateWindow(window_width, window_height, window_title, NULL, NULL);
    window->should_close = false;

    if (window->window == NULL)
        PEAR_ERROR("failed to create window!");
    glfwMakeContextCurrent(window->window);

    glfwSetWindowCloseCallback(window->window, window_close_callback);
    glfwSetWindowSizeCallback(window->window, window_resize_callback);
    glfwSetKeyCallback(window->window, window_key_callback);
    glfwSetCursorPosCallback(window->window, window_mouse_movement_callback);
    glfwSetMouseButtonCallback(window->window, window_mouse_button_callback);
    glfwSetWindowContentScaleCallback(window->window, window_scale_callback);

    return window;
}

void window_delete(Window* window) {
    glfwDestroyWindow(window->window);

    free(window);

    glfwTerminate();
}

void window_update(Window* window) {
    glfwSwapBuffers(window->window);
    glfwPollEvents();
}

bool window_should_close(Window* window) {
    return window->should_close;
}

i32 window_get_width(Window* window) {
    i32 width;
    glfwGetWindowSize(window->window, &width, NULL);
    return width;
}

i32 window_get_height(Window* window) {
    i32 height;
    glfwGetWindowSize(window->window, NULL, &height);
    return height;
}

f32 window_get_scale_x(Window* window) {
    f32 scale;
    glfwGetWindowContentScale(window->window, &scale, NULL);
    return scale;
}

f32 window_get_scale_y(Window* window) {
    f32 scale;
    glfwGetWindowContentScale(window->window, NULL, &scale);
    return scale;
}

#endif
