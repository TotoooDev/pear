#ifndef PEAR_WINDOW_H
#define PEAR_WINDOW_H

#include <core/types.h>

typedef struct Window Window;

Window* window_new();
void window_delete(Window* window);

void window_update(Window* window);
bool window_should_close(Window* window);

#endif
