#ifndef PEAR_SCENE_COMPONENTS_DRAWABLE_H_
#define PEAR_SCENE_COMPONENTS_DRAWABLE_H_

#include <graphics/mesh.h>
#include <graphics/texture.h>

typedef struct drawable_component_t {
    mesh_t* mesh;
    texture_t* texture;
} drawable_component_t;

#endif
