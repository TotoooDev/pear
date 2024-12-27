#ifdef PEAR_DEBUG

#ifndef PEAR_GRAPHICS_EDITOR_H_
#define PEAR_GRAPHICS_EDITOR_H_

#include <core/types.h>

void editor_init();
void editor_free();

void editor_clear();
void editor_render();


void editor_enable_demo(bool enable);

#endif

#endif
