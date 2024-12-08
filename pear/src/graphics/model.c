#include <graphics/model.h>
#include <core/log.h>
#include <core/alloc.h>
#include <pear_image.h>
#include <string.h>

typedef struct model_t {
    mesh_t** meshes;
    material_t* materials;

    u32 num_meshes;
    u32 num_materials;
} model_t;

model_t* model_new(mesh_t** meshes, material_t* materials, u32 num_meshes, u32 num_materials) {
    model_t* model = (model_t*)PEAR_MALLOC(sizeof(model_t));

    model->meshes = meshes;
    model->materials = materials;
    model->num_meshes = num_meshes;
    model->num_materials = num_materials;

    return model;
}

model_t* model_new_from_pear_model(pear_model_t model) {
    mesh_t** meshes = (mesh_t**)PEAR_MALLOC(sizeof(mesh_t*) * model.num_meshes);
    for (u32 i = 0; i < model.num_meshes; i++) {
        pear_mesh_t mesh = model.meshes[i];

        vec3* positions = (vec3*)PEAR_MALLOC(sizeof(vec3) * mesh.num_vertices);
        vec2* texture_coords = (vec2*)PEAR_MALLOC(sizeof(vec2) * mesh.num_vertices);
        vec3* normals = (vec3*)PEAR_MALLOC(sizeof(vec3) * mesh.num_vertices);
        u32* indices = (u32*)PEAR_MALLOC(sizeof(u32) * mesh.num_indices);
        
        for (u32 j = 0; j < mesh.num_vertices; j++) {
            pear_vertex_t vertex = mesh.vertices[j];

            positions[j][0] = vertex.pos[0];
            positions[j][1] = vertex.pos[1];
            positions[j][2] = vertex.pos[2];

            texture_coords[j][0] = vertex.texture_coords[0];
            texture_coords[j][1] = vertex.texture_coords[1];

            normals[j][0] = vertex.normal[0];
            normals[j][1] = vertex.normal[1];
            normals[j][2] = vertex.normal[2];
        }

        memcpy(indices, mesh.indices, sizeof(u32) * mesh.num_indices);

        mesh_info_t* mesh_info = meshinfo_new();
        meshinfo_add_position(mesh_info, positions, mesh.num_vertices);
        meshinfo_add_texture_coords(mesh_info, texture_coords, mesh.num_vertices);
        meshinfo_add_normal(mesh_info, normals, mesh.num_vertices);
        meshinfo_add_indices(mesh_info, indices, mesh.num_indices);
        
        mesh_t* renderer_mesh = mesh_new(mesh_info, mesh.material_index);
        meshes[i] = renderer_mesh;
        
        meshinfo_delete(mesh_info);

        PEAR_FREE(positions);
        PEAR_FREE(texture_coords);
        PEAR_FREE(normals);
        PEAR_FREE(indices);
    }

    material_t* materials = (material_t*)PEAR_MALLOC(sizeof(material_t) * model.num_materials);
    for (u32 i = 0; i < model.num_materials; i++) {
        pear_material_t material = model.materials[i];
        material_t renderer_material;

        bool success;
        if (material.diffuse_path[0] != '\0') {
            image_t* diffuse_image = image_new_from_pear_image(pear_image_load(material.diffuse_path, &success));
            if (!success) {
                PEAR_ERROR("failed to load texture %s!", material.diffuse_path);
            }

            renderer_material.diffuse = texture_new_from_image(diffuse_image, TEXTURE_WRAPPING_CLAMP, TEXTURE_FILTERING_NEAREST);
        }
        if (material.specular_path[0] != '\0') {
            image_t* specular_image = image_new_from_pear_image(pear_image_load(material.specular_path, &success));
            if (!success) {
                PEAR_ERROR("failed to load texture %s!", material.specular_path);
            }

            renderer_material.specular = texture_new_from_image(specular_image, TEXTURE_WRAPPING_CLAMP, TEXTURE_FILTERING_NEAREST);
        }
        if (material.normal_path[0] != '\0') {
            image_t* normal_image = image_new_from_pear_image(pear_image_load(material.normal_path, &success));
            if (!success) {
                PEAR_ERROR("failed to load texture %s!", material.normal_path);
            }

            renderer_material.normal = texture_new_from_image(normal_image, TEXTURE_WRAPPING_CLAMP, TEXTURE_FILTERING_NEAREST);
        }

        glm_vec3_copy(material.color, renderer_material.color);

        materials[i] = renderer_material;
    }

    return model_new(meshes, materials, model.num_meshes, model.num_materials);
}

void model_delete(model_t* model) {
    for (u32 i = 0; i < model->num_meshes; i++) {
        mesh_delete(model->meshes[i]);
    }

    PEAR_FREE(model->meshes);
    PEAR_FREE(model->materials);
    PEAR_FREE(model);
}

u32 model_get_num_meshes(model_t* model) {
    return model->num_meshes;
}

u32 model_get_num_materials(model_t* model) {
    return model->num_materials;
}

mesh_t** model_get_meshes(model_t* model) {
    return model->meshes;
}

material_t* model_get_materials(model_t* model) {
    return model->materials;
}
