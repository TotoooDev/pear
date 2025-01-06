#ifndef PEAR_APP_H_
#define PEAR_APP_H_

#include <graphics/window.h>
#include <graphics/renderer.h>
#include <scene/scene.h>

void app_init();
void app_stop();
void app_run();

scene_t* app_get_scene();
void app_set_scene(scene_t* scene);

window_t* app_get_window();
renderer_t* app_get_renderer();
f32 app_get_timestep();

u32 app_get_version_major();
u32 app_get_version_minor();
u32 app_get_version_patch();
const char* app_get_version_string();

#ifdef PEAR_ENABLE_EDITOR
void app_disable_editor();
#endif

#endif
