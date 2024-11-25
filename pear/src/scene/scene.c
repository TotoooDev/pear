#include <scene/scene.h>
#include <core/types.h>
#include <stdlib.h>

typedef struct scene_t {
    u32 foo;
} scene_t;

scene_t* scene_new() {
    scene_t* scene = (scene_t*)malloc(sizeof(scene_t));
    return scene;
}

void scene_delete(scene_t* scene) {
    free(scene);
}
