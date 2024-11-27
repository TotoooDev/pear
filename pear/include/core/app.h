#ifndef PEAR_APP_H_
#define PEAR_APP_H_

#include <scene/scene.h>

void app_init();
void app_stop();
void app_run();

scene_t* app_get_scene();
void app_set_scene(scene_t* scene);

#endif
