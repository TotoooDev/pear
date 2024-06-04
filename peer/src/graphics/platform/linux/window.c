#ifdef PEER_PLATFORM_LINUX

#include <graphics/window.h>
#include <core/log.h>
#include <graphics/platform/linux/window.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>

typedef struct Window {
    GLFWwindow* window;
    bool should_close;
} Window;

static u32 window_num_windows = 0;
static u32 window_next_width = 1280;
static u32 window_next_height = 720;
static const char* window_next_title = "New window";

void window_set_next_width(u32 width) {
    window_next_width = width;
}

void window_set_next_height(u32 height) {
    window_next_height = height;
}

void window_set_next_title(const char* title) {
    window_next_title = title;
}

Window* window_new() {
    if (window_num_windows == 0) {
        if (glfwInit() != GLFW_TRUE)
            PEER_ERROR("failed to initialize glfw!");
    }

    Window* window = (Window*)malloc(sizeof(Window));

    window->window = glfwCreateWindow(window_next_width, window_next_height, window_next_title, NULL, NULL);
    window->should_close = false;

    if (window->window == NULL)
        PEER_ERROR("failed to create window!");

    window_num_windows++;
    return window;
}

void window_delete(Window* window) {
    glfwDestroyWindow(window->window);

    free(window);

    window_num_windows--;
    if (window_num_windows == 0)
        glfwTerminate();
}

bool window_should_close(Window* window) {
    return window->should_close;
}

#endif
