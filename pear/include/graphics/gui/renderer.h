#ifndef PEAR_GUI_RENDERER_H_
#define PEAR_GUI_RENDERER_H_

#include <graphics/gui/gui.h>
#include <graphics/texture.h>

void gui_renderer_set_shadow_map(texture_t* shadow_map);
void gui_renderer(struct nk_context* nk_context, void* user_data);

#endif
