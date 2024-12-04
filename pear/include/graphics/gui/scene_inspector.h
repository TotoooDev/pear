#ifndef PEAR_GUI_SCENE_INSPECTOR_H_
#define PEAR_GUI_SCENE_INSPECTOR_H_

#include <graphics/gui/gui.h>
#include <scene/scene.h>

void gui_scene_inspector_set_scene(scene_t* scene);
void gui_scene_inspector(struct nk_context* nk_context, void* user_data);

#endif
