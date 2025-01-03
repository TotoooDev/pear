#ifdef PEAR_ENABLE_EDITOR

#ifndef PEAR_GRAPHICS_EDITOR_RENDERER_INSPECTOR_H_
#define PEAR_GRAPHICS_EDITOR_RENDERER_INSPECTOR_H_

#include <graphics/texture.h>
#include <core/types.h>

void editor_set_shadow_map(texture_t* shadow_map);
void editor_renderer_inspector(bool* show);

#endif

#endif
