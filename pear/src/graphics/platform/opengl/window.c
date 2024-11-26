#ifndef PEAR_OPENGL

#include <graphics/window.h>
#include <core/log.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>

typedef struct window_t {
    GLFWwindow* window;
} window_t;

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
