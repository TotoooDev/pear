#include <loaders/model.h>
#include <util/filesystem.h>
#include <core/log.h>
#include <core/alloc.h>
#include <stdio.h>

#define CGLTF_IMPLEMENTATION
#include <loaders/vendor/cgltf.h>

model_t* loader_load_gltf(const char* filename) {
    cgltf_options options = { 0 };
    cgltf_data* data = NULL;

    cgltf_result result = cgltf_parse_file(&options, filename, &data);
    if (result != cgltf_result_success) {
        PEAR_INFO("failed to load gltf %s!", filename);
        return NULL;
    }

    result = cgltf_load_buffers(&options, data, filename);
    if (result != cgltf_result_success) {
        PEAR_INFO("failed to load gltf buffers for %s!", filename);
        return NULL;
    }

    PEAR_INFO("%d meshes:", data->meshes_count);
    for (u32 i = 0; i < data->meshes_count; i++) {
        PEAR_INFO("  %s", data->meshes[i].name);
        PEAR_INFO("  primitives:");
        for (u32 j = 0; j < data->meshes[i].primitives_count; j++) {
            PEAR_INFO("    type: %d", data->meshes[i].primitives[j].type);
            if (data->meshes[i].primitives[j].type != cgltf_primitive_type_triangles) {
                PEAR_WARN("pear does not support other primitives than triangles!");
                return NULL;
            } 

            u32 num_indices = cgltf_accessor_unpack_indices(data->meshes[i].primitives[j].indices, NULL, 0, 0);
            PEAR_INFO("    %d indices:", num_indices);

            u32* indices = (u32*)PEAR_MALLOC(sizeof(u32) * num_indices);
            u32 ret = cgltf_accessor_unpack_indices(data->meshes[i].primitives[j].indices, indices, sizeof(u32), num_indices);
            if (ret == 0) {
                PEAR_WARN("failed to unpack indices!");
                return NULL;
            }
            // for (u32 k = 0; k < num_indices; k++) {
            //     PEAR_INFO("      %d", indices[k]);
            // }
            PEAR_FREE(indices);

            PEAR_INFO("    %d attributes", data->meshes[i].primitives[j].attributes_count);
            for (u32 k = 0; k < data->meshes[i].primitives[j].attributes_count; k++) {
                u32 num_data = cgltf_accessor_unpack_floats(data->meshes[i].primitives[j].attributes[k].data, NULL, 0);
                u32 num_components = cgltf_num_components(data->meshes[i].primitives[j].attributes[k].type);
                PEAR_INFO("    %s", data->meshes[i].primitives[j].attributes[k].name);
                PEAR_INFO("      type: %d", data->meshes[i].primitives[j].attributes[k].type);
                PEAR_INFO("      index: %d", data->meshes[i].primitives[j].attributes[k].index);
                PEAR_INFO("      num components: %d", num_components);
                PEAR_INFO("      num data: %d", num_data);

                f32* stuff = PEAR_MALLOC(sizeof(f32) * num_data);
                u32 ret = cgltf_accessor_unpack_floats(data->meshes[i].primitives[j].attributes[k].data, stuff, num_data);
                if (ret == 0) {
                    PEAR_WARN("failed to unpack attribute %s!", data->meshes[i].primitives[j].attributes[k].name);
                    return NULL;
                }
                // for (u32 l = 0; l < num_data; l += 3) {
                //     PEAR_INFO("        %f %f %f", stuff[l], stuff[l + 1], stuff[l + 2]);
                // }
                PEAR_FREE(stuff);
            }
        }
    }

    PEAR_INFO("%d materials:", data->materials_count);
    for (u32 i = 0; i < data->materials_count; i++) {
        PEAR_INFO("  %s", data->materials[i].name);
        PEAR_INFO("    has specular: %d", data->materials[i].has_specular);
        PEAR_INFO("    has pbr metallic roughness: %d", data->materials[i].has_pbr_metallic_roughness);
        PEAR_INFO("    roughness: %f", data->materials[i].pbr_metallic_roughness.roughness_factor);
        PEAR_INFO("    base color factor: %f %f %f %f", data->materials[i].pbr_metallic_roughness.base_color_factor[0], data->materials[i].pbr_metallic_roughness.base_color_factor[1], data->materials[i].pbr_metallic_roughness.base_color_factor[2], data->materials[i].pbr_metallic_roughness.base_color_factor[3]);
        // PEAR_INFO("    color texture: %p", data->materials[i].pbr_metallic_roughness.base_color_texture.texture->image->buffer_view->buffer->data);
    }

    cgltf_free(data);
}
