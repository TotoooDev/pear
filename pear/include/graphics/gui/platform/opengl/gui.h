#ifdef PEAR_PLATFORM_OPENGL

#ifndef PEAR_GRAPHICS_GUI_OPENGL_H_
#define PEAR_GRAPHICS_GUI_OPENGL_H_

struct nk_context* gui_platform_init();
void gui_platform_free();
void gui_platform_clear();
void gui_platform_render();

#endif

#endif
