#ifndef PEAR_NK_H
#define PEAR_NK_H

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_KEYSTATE_BASED_INPUT
#include <graphics/vendor/nuklear.h>

typedef void(*GUIFunction)(struct nk_context* nk_context, void* user_data);

static const nk_flags gui_default_window_flags = NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE | NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE;

void gui_init();
void gui_free();

void gui_add(GUIFunction function, void* user_data);
void gui_clear();
void gui_render();

#endif
