#ifdef PEAR_PLATFORM_OPENGL

#ifndef PEAR_GRAPHICS_OPENGL_WINDOW_H_
#define PEAR_GRAPHICS_OPENGL_WINDOW_H_

#include <graphics/window.h>
#include <GLFW/glfw3.h>

GLFWwindow* window_get_glfw(window_t* window);
void window_lock_mouse(window_t* window, bool lock);

#endif

#endif
