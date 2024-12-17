#ifndef PEAR_GRAPHICS_GUI_H_
#define PEAR_GRAPHICS_GUI_H_

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_KEYSTATE_BASED_INPUT
#include <graphics/vendor/nuklear.h>

#include <graphics/texture.h>

typedef void(*gui_function_t)(struct nk_context* nk_context, void* user_data);

static const nk_flags gui_default_window_flags = NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE | NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE;

void gui_init();
void gui_free();

void gui_add(gui_function_t function, void* user_data);
void gui_clear();
void gui_render();

struct nk_colorf gui_vec4_to_nk_color(vec4 color);
void gui_nk_color_to_vec4(struct nk_colorf nk_color, vec4 color);
struct nk_colorf gui_vec3_to_nk_color(vec3 color);
void gui_nk_color_to_vec3(struct nk_colorf nk_color, vec3 color);
struct nk_color gui_nk_colorf_to_nk_color(struct nk_colorf color);

struct nk_image gui_image_from_texture(texture_t* texture);

#endif
