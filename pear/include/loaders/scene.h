#ifndef PEAR_LOADERS_SCENE_H_
#define PEAR_LOADERS_SCENE_H_

#include <scene/scene.h>

void loader_populate_scene(scene_t* scene, const char* filename);
scene_t* loader_load_scene(const char* filename);
void loader_write_scene(scene_t* scene, const char* filename, array_t* excluded_entities);

#endif
