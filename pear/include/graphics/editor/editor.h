#ifdef PEAR_ENABLE_EDITOR

#ifndef PEAR_GRAPHICS_EDITOR_H_
#define PEAR_GRAPHICS_EDITOR_H_

#include <core/types.h>

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui.h>
#include <cimconfig.h>
#include <cimguizmo.h>

typedef void(*editor_function_t)(void*);

void editor_init();
void editor_free();

void editor_clear();
void editor_render();

void editor_add_function(editor_function_t function, void* user_data);

#endif

#endif
