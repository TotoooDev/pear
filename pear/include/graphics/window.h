#ifndef PEAR_GRAPHICS_WINDOW_H_
#define PEAR_GRAPHICS_WINDOW_H_

#include <core/types.h>

typedef struct window_t window_t;

window_t* window_new(const char* title, u32 width, u32 height);
void window_delete(window_t* window);

void window_update(window_t* window);

u32 window_get_width(window_t* window);
u32 window_get_height(window_t* window);
f32 window_get_scale_x(window_t* window);
f32 window_get_scale_y(window_t* window);

#endif
