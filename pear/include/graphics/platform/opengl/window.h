#ifndef PEAR_OPENGL_WINDOW_H
#define PEAR_OPENGL_WINDOW_H

#include <core/types.h>
#include <GLFW/glfw3.h>

void window_set_width(u32 width);
void window_set_height(u32 height);
void window_set_title(const char* title);
GLFWwindow* window_get_glfw(Window* window);

#endif
