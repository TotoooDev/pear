#include <loaders/model.h>
#include <graphics/mesh_info.h>
#include <graphics/mesh.h>
#include <graphics/texture.h>
#include <core/log.h>
#include <core/alloc.h>
#include <stdio.h>

#define CGLTF_IMPLEMENTATION
#include <loaders/vendor/cgltf.h>

void loader_load_indices(mesh_info_t* mesh_info, cgltf_primitive primitive) {
    u32 num_indices = cgltf_accessor_unpack_indices(primitive.indices, NULL, 0, 0);
    u32* indices = (u32*)PEAR_MALLOC(sizeof(u32) * num_indices);
    u32 ret = cgltf_accessor_unpack_indices(primitive.indices, indices, sizeof(u32), num_indices);
    if (ret == 0) {
        PEAR_WARN("failed to unpack indices!");
        return;
    }

    meshinfo_add_indices(mesh_info, indices, num_indices);

    // PEAR_INFO("    %d indices:", num_indices);
    // for (u32 k = 0; k < num_indices; k++) {
    //     PEAR_INFO("      %d", indices[k]);
    // }

    PEAR_FREE(indices);
}

void loader_load_positions(mesh_info_t* mesh_info, cgltf_attribute attribute) {
    u32 num_data = cgltf_accessor_unpack_floats(attribute.data, NULL, 0);
    vec3* positions = PEAR_MALLOC(sizeof(f32) * num_data);
    u32 ret = cgltf_accessor_unpack_floats(attribute.data, (f32*)positions, num_data);
    if (ret == 0) {
        PEAR_WARN("failed to unpack attribute %s!", attribute.name);
        return;
    }

    meshinfo_add_position(mesh_info, positions, num_data / 3);

    // PEAR_INFO("    %s", attribute.name);
    // PEAR_INFO("      type: %d", attribute.type);
    // PEAR_INFO("      index: %d", attribute.index);
    // PEAR_INFO("      num data: %d", num_data);
    // for (u32 l = 0; l < num_data; l += 3) {
    //     PEAR_INFO("        %f %f %f", positions[l], positions[l + 1], positions[l + 2]);
    // }

    PEAR_FREE(positions);
}

void loader_load_normals(mesh_info_t* mesh_info, cgltf_attribute attribute) {
    u32 num_data = cgltf_accessor_unpack_floats(attribute.data, NULL, 0);
    vec3* normals = PEAR_MALLOC(sizeof(f32) * num_data);
    u32 ret = cgltf_accessor_unpack_floats(attribute.data, (f32*)normals, num_data);
    if (ret == 0) {
        PEAR_WARN("failed to unpack attribute %s!", attribute.name);
        return;
    }

    meshinfo_add_normal(mesh_info, normals, num_data / 3);

    // PEAR_INFO("    %s", attribute.name);
    // PEAR_INFO("      type: %d", attribute.type);
    // PEAR_INFO("      index: %d", attribute.index);
    // PEAR_INFO("      num data: %d", num_data);
    // for (u32 l = 0; l < num_data; l += 3) {
    //     PEAR_INFO("        %f %f %f", normals[l], normals[l + 1], normals[l + 2]);
    // }

    PEAR_FREE(normals);
}

void loader_load_texture_coords(mesh_info_t* mesh_info, cgltf_attribute attribute) {
    u32 num_data = cgltf_accessor_unpack_floats(attribute.data, NULL, 0);
    vec2* texture_coords = PEAR_MALLOC(sizeof(f32) * num_data);
    u32 ret = cgltf_accessor_unpack_floats(attribute.data, (f32*)texture_coords, num_data);
    if (ret == 0) {
        PEAR_WARN("failed to unpack attribute %s!", attribute.name);
        return;
    }

    meshinfo_add_texture_coords(mesh_info, texture_coords, num_data / 2);

    // PEAR_INFO("    %s", attribute.name);
    // PEAR_INFO("      type: %d", attribute.type);
    // PEAR_INFO("      index: %d", attribute.index);
    // PEAR_INFO("      num data: %d", num_data);
    // for (u32 l = 0; l < num_data; l += 2) {
    //     PEAR_INFO("        %f %f", texture_coords[l], texture_coords[l + 1]);
    // }

    PEAR_FREE(texture_coords);
}

void loader_load_attributes(mesh_info_t* mesh_info, cgltf_primitive primitive) {
    // PEAR_INFO("    %d attributes", primitive.attributes_count);
    for (u32 i = 0; i < primitive.attributes_count; i++) {
        u32 num_data = cgltf_accessor_unpack_floats(primitive.attributes[i].data, NULL, 0);
        u32 num_components = cgltf_num_components(primitive.attributes[i].type);

        switch (primitive.attributes[i].type) {
        case cgltf_attribute_type_position:
            loader_load_positions(mesh_info, primitive.attributes[i]);
            break;
        case cgltf_attribute_type_normal:
            loader_load_normals(mesh_info, primitive.attributes[i]);
            break;
        case cgltf_attribute_type_texcoord:
            loader_load_texture_coords(mesh_info, primitive.attributes[i]);
            break;
        default:
            PEAR_WARN("unsupported attribute type %d!", primitive.attributes[i].type);
        }
    }
}

model_t* loader_load_gltf(const char* filename) {
    cgltf_options options = { 0 };
    cgltf_data* data = NULL;

    cgltf_result result = cgltf_parse_file(&options, filename, &data);
    if (result != cgltf_result_success) {
        PEAR_ERROR("failed to load gltf %s!", filename);
        return NULL;
    }

    result = cgltf_load_buffers(&options, data, filename);
    if (result != cgltf_result_success) {
        PEAR_ERROR("failed to load gltf buffers for %s!", filename);
        return NULL;
    }

    // materials are duplicated
    material_t* materials = (material_t*)PEAR_MALLOC(sizeof(material_t) * data->meshes_count);
    mesh_t** meshes = (mesh_t**)PEAR_MALLOC(sizeof(mesh_t*) * data->meshes_count);

    // PEAR_INFO("%d meshes:", data->meshes_count);
    for (u32 i = 0; i < data->meshes_count; i++) {
        // PEAR_INFO("  %s", data->meshes[i].name);
        // PEAR_INFO("  primitives:");
        for (u32 j = 0; j < data->meshes[i].primitives_count; j++) {
            // PEAR_INFO("    type: %d", data->meshes[i].primitives[j].type);
            if (data->meshes[i].primitives[j].type != cgltf_primitive_type_triangles) {
                PEAR_ERROR("pear does not support other primitives than triangles!");
                return NULL;
            }

            mesh_info_t* mesh_info = meshinfo_new();

            loader_load_indices(mesh_info, data->meshes[i].primitives[j]);
            loader_load_attributes(mesh_info, data->meshes[i].primitives[j]);

            cgltf_material* gltf_material = data->meshes[i].primitives[j].material;

            materials[i].color[0] = gltf_material->pbr_metallic_roughness.base_color_factor[0];
            materials[i].color[1] = gltf_material->pbr_metallic_roughness.base_color_factor[1];
            materials[i].color[2] = gltf_material->pbr_metallic_roughness.base_color_factor[2];

            materials[i].shininess = gltf_material->pbr_metallic_roughness.roughness_factor;

            if (gltf_material->pbr_metallic_roughness.base_color_texture.texture != NULL) {
                // load diffuse texture
                // cgltf_buffer* buffer = gltf_material->pbr_metallic_roughness.base_color_texture.texture->image->buffer_view->buffer;
                u8* image_data = cgltf_buffer_view_data(gltf_material->pbr_metallic_roughness.base_color_texture.texture->image->buffer_view);
                // void* image_data = PEAR_MALLOC(buffer->size);
                // memcpy(image_data, buffer->data, buffer->size);

                image_t* image = image_new(4, image_data, gltf_material->pbr_metallic_roughness.base_color_texture.texture->image->buffer_view->size);
                texture_t* texture = texture_new_from_image(image, TEXTURE_WRAPPING_NONE, TEXTURE_FILTERING_NEAREST);
                // image_delete(image);
                materials[i].diffuse = texture;
            }
            else {
                materials[i].diffuse = NULL;
            }
            materials[i].specular = NULL;
            materials[i].normal = NULL;

            mesh_t* mesh = mesh_new(mesh_info, i);
            meshes[i] = mesh;

            meshinfo_delete(mesh_info);
        }
    }

    model_t* model = model_new(meshes, materials, data->meshes_count, data->meshes_count);

    cgltf_free(data);

    return model;
}
