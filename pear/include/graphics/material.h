#ifndef PEAR_GRAPHICS_MATERIAL_H_
#define PEAR_GRAPHICS_MATERIAL_H_

#include <graphics/texture.h>

typedef struct material_t {
    texture_t* diffuse;
    texture_t* specular;
    texture_t* normal;

    vec3 color;
} material_t;

#endif
