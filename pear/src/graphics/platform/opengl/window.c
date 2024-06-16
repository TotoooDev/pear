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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    Window* window = (Window*)malloc(sizeof(Window));

    window->window = glfwCreateWindow(window_width, window_height, window_title, NULL, NULL);
    window->should_close = false;

    if (window->window == NULL)
        PEAR_ERROR("failed to create window!");
    glfwSetWindowCloseCallback(window->window, window_close_callback);
    glfwMakeContextCurrent(window->window);

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

#endif
