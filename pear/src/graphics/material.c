#include <graphics/material.h>
#include <core/log.h>

#define MATERIAL_NUM_MAX_MATERIALS 1024

static Material material_array[MATERIAL_NUM_MAX_MATERIALS];
static u32 material_num_materials;

static Material material_default = {
    .texture_albedo = NULL,
    .texture_specular = NULL,
    .texture_normal = NULL,

    .color_diffuse = { 1.0f, 0.0f, 1.0f, 1.0f },
    .color_specular = { 1.0f, 0.0f, 1.0f, 1.0f },

    .roughness = 128.0f
};

u32 material_add(Material material) {
    material_array[material_num_materials] = material;
    u32 old_index = material_num_materials;
    material_num_materials++;
    return old_index;
}

Material* material_get(u32 index) {
    if (index >= MATERIAL_NUM_MAX_MATERIALS) {
        PEAR_ERROR("trying to access material at invalid index %d! the array is of size %d.", index, MATERIAL_NUM_MAX_MATERIALS);
        return NULL;
    }

    if (index >= material_num_materials) {
        PEAR_ERROR("trying to access invalid material at index %d! there are only %d materials in the array.", index, material_num_materials);
        return NULL;
    }

    return &(material_array[index]);
}

Material material_get_default() {
    return material_default;
}

u32 material_get_num_materials() {
    return material_num_materials;
}
