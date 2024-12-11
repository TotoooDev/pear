#ifdef PEAR_PLATFORM_OPENGL

#include <graphics/window.h>
#include <graphics/platform/opengl/window.h>
#include <event/event_dispatcher.h>
#include <core/log.h>
#include <GLFW/glfw3.h>
#include <core/alloc.h>

typedef struct window_t {
    GLFWwindow* window;
} window_t;

static bool window_first_mouse = true;
static f32 window_last_mouse_x = 0.0f;
static f32 window_last_mouse_y = 0.0f;

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

    if (window_first_mouse) {
        event.rel_x = 0.0f;
        event.rel_y = 0.0f;
        window_first_mouse = false;
    }
    else {
        event.rel_x = event.x - window_last_mouse_x;
        event.rel_y = event.y - window_last_mouse_y;
    }
    window_last_mouse_x = event.x;
    window_last_mouse_y = event.y;

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

    #ifdef PEAR_DEBUG
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    #endif

    window_t* window = (window_t*)PEAR_MALLOC(sizeof(window_t));

    window->window = glfwCreateWindow(width, height, title, NULL, NULL);

    if (window->window == NULL) {
        const char* error;
        u32 error_code = glfwGetError(&error);
        PEAR_ERROR("failed to create window! %d: %s", error);
        return NULL;
    }
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
    PEAR_FREE(window);
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

GLFWwindow* window_get_glfw(window_t* window) {
    return window->window;
}

#endif
