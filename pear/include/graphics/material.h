#ifndef PEAR_MATERIAL_H
#define PEAR_MATERIAL_H

#include <graphics/texture.h>

typedef struct Material {
    Texture* texture_albedo;
    Texture* texture_specular;
    Texture* texture_normal;

    vec4 color_diffuse;
    vec4 color_specular;

    f32 roughness;

    bool use_color;
} Material;

u32 material_add(Material material);
Material* material_get(u32 index);
Material material_get_default();
u32 material_get_num_materials();

#endif
