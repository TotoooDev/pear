#ifndef PEAR_MATERIAL_H
#define PEAR_MATERIAL_H

#include <graphics/texture.h>

typedef struct Material {
    Texture* albedo;
    vec4 color;
} Material;

#endif
