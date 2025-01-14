#include <loaders/model.h>
#include <loaders/image.h>
#include <graphics/mesh_info.h>
#include <graphics/mesh.h>
#include <graphics/texture.h>
#include <util/array.h>
#include <core/log.h>
#include <core/alloc.h>
#include <stdio.h>

#define CGLTF_IMPLEMENTATION
#include <loaders/vendor/cgltf.h>

material_t* loader_load_material(cgltf_material* gltf_material, char* directory) {
    material_t* material = PEAR_MALLOC(sizeof(material_t));

    if (strlen(gltf_material->name) > 255) {
        strcpy(material->name, "original name too long!");
    }
    else {
        strcpy(material->name, gltf_material->name);
    }

    material->color[0] = gltf_material->pbr_metallic_roughness.base_color_factor[0];
    material->color[1] = gltf_material->pbr_metallic_roughness.base_color_factor[1];
    material->color[2] = gltf_material->pbr_metallic_roughness.base_color_factor[2];

    material->shininess = gltf_material->pbr_metallic_roughness.roughness_factor;

    if (gltf_material->pbr_metallic_roughness.base_color_texture.texture != NULL) {
        cgltf_texture* gltf_texture = gltf_material->pbr_metallic_roughness.base_color_texture.texture;
        image_t* image;
        if (gltf_texture->image->buffer_view != NULL) {
            u8* image_data = cgltf_buffer_view_data(gltf_texture->image->buffer_view);
            image = loader_load_image_data(image_data, gltf_texture->image->buffer_view->size);
        }
        else {
            char texture_path[1024];
            sprintf(texture_path, "%s/%s", directory, gltf_texture->image->uri);
            image = loader_load_image(texture_path);
        }

        texture_t* texture = texture_new_from_image(image, TEXTURE_WRAPPING_NONE, TEXTURE_FILTERING_NEAREST);
        image_delete(image);
        material->diffuse = texture;
        material->use_color = false;
    }
    else {
        material->diffuse = NULL;
        material->use_color = true;
    }
    material->specular = NULL;
    material->normal = NULL;

    return material;
}

void loader_load_indices(mesh_info_t* mesh_info, cgltf_primitive primitive) {
    u32 num_indices = cgltf_accessor_unpack_indices(primitive.indices, NULL, 0, 0);
    u32* indices = (u32*)PEAR_MALLOC(sizeof(u32) * num_indices);
    u32 ret = cgltf_accessor_unpack_indices(primitive.indices, indices, sizeof(u32), num_indices);
    if (ret == 0) {
        PEAR_WARN("failed to unpack indices!");
        return;
    }

    meshinfo_add_indices(mesh_info, indices, num_indices);
    PEAR_FREE(indices);
}

void loader_load_positions(mesh_info_t* mesh_info, cgltf_attribute attribute, mat4 matrix) {
    u32 num_data = cgltf_accessor_unpack_floats(attribute.data, NULL, 0);
    vec3* positions = PEAR_MALLOC(sizeof(f32) * num_data);
    u32 ret = cgltf_accessor_unpack_floats(attribute.data, (f32*)positions, num_data);
    if (ret == 0) {
        PEAR_WARN("failed to unpack attribute %s!", attribute.name);
        return;
    }

    for (u32 i = 0; i < num_data / 3; i++) {
        glm_mat4_mulv3(matrix, positions[i], 1.0f, positions[i]);
    }

    meshinfo_add_position(mesh_info, positions, num_data / 3);
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
    PEAR_FREE(texture_coords);
}

void loader_load_attributes(mesh_info_t* mesh_info, cgltf_primitive primitive, mat4 matrix) {
    for (u32 i = 0; i < primitive.attributes_count; i++) {
        u32 num_data = cgltf_accessor_unpack_floats(primitive.attributes[i].data, NULL, 0);
        u32 num_components = cgltf_num_components(primitive.attributes[i].type);

        switch (primitive.attributes[i].type) {
        case cgltf_attribute_type_position:
            loader_load_positions(mesh_info, primitive.attributes[i], matrix);
            break;
        case cgltf_attribute_type_normal:
            loader_load_normals(mesh_info, primitive.attributes[i]);
            break;
        case cgltf_attribute_type_texcoord:
            loader_load_texture_coords(mesh_info, primitive.attributes[i]);
            break;
        default:
            PEAR_WARN("unsupported attribute type %d!", primitive.attributes[i].type);
            break;
        }
    }
}

mesh_t* loader_handle_mesh(cgltf_mesh* mesh, array_t* materials, array_t* loaded_materials, mat4 matrix, char* directory) {
    for (u32 j = 0; j < mesh->primitives_count; j++) {
        if (mesh->primitives[j].type != cgltf_primitive_type_triangles) {
            PEAR_ERROR("pear does not support other primitives than triangles!");
            return NULL;
        }

        mesh_info_t* mesh_info = meshinfo_new();

        loader_load_indices(mesh_info, mesh->primitives[j]);
        loader_load_attributes(mesh_info, mesh->primitives[j], matrix);

        u32 index;
        cgltf_material* gltf_material = mesh->primitives[j].material;
        bool skip = false;
        for (u32 k = 0; k < array_get_length(loaded_materials); k++) {
            if (array_get(loaded_materials, k) == gltf_material) {
                index = k;
                skip = true;
                break;
            }
        }

        if (!skip) {
            index = array_get_length(materials);
            material_t* material = loader_load_material(gltf_material, directory);
            array_add(materials, material);
            array_add(loaded_materials, gltf_material);
        }

        mesh_t* mesh = mesh_new(mesh_info, index);
        meshinfo_delete(mesh_info);
        return mesh;
    }
}

void loader_handle_node(cgltf_node* node, array_t* meshes, array_t* materials, array_t* loaded_materials, char* directory) {
    mat4 matrix;
    cgltf_float gltf_matrix[16];
    cgltf_node_transform_world(node, gltf_matrix);
    glm_mat4_make(gltf_matrix, matrix);

    if (node->mesh != NULL) {
        mesh_t* mesh = loader_handle_mesh(node->mesh, materials, loaded_materials, matrix, directory);
        array_add(meshes, mesh);
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


    char directory[1024];
    char* slash = strrchr(filename, '/');
    if (slash == NULL) {
        directory[0] = '\0';
    }
    else {
        u32 directory_index = strrchr(filename, '/') - filename;
        strcpy(directory, filename);
        directory[directory_index] = '\0';
    }

    array_t* meshes = array_new(10);
    array_t* materials = array_new(10);
    array_t* loaded_materials = array_new(5);

    for (u32 i = 0; i < data->scenes_count; i++) {
        cgltf_scene scene = data->scenes[i];
        for (u32 j = 0; j < data->nodes_count; j++) {
            loader_handle_node(&data->nodes[j], meshes, materials, loaded_materials, directory);
        }
    }

    model_t* model = model_new((mesh_t**)array_get_data(meshes), (material_t**)array_get_data(materials), array_get_length(meshes), array_get_length(materials));

    cgltf_free(data);

    return model;
}
