#include <graphics/model.h>
#include <graphics/material.h>
// #include <fs/loaders/image_loader.h>
#include <stdlib.h>
#include <core/log.h>

typedef struct Model {
    Mesh** meshes;
    u32 num_meshes;
} Model;

Model* model_new(Mesh** meshes, u32 num_meshes) {
    Model* model = (Model*)malloc(sizeof(Model));
    
    model->meshes = meshes;
    model->num_meshes = num_meshes;
    
    return model;
}

Model* model_new_from_pear3d(Pear3D pear3d) {
    Mesh** meshes = (Mesh**)malloc(sizeof(Mesh*) * pear3d.num_meshes);

    // process the materials first
    Material default_material = material_get_default();
    bool use_default_material = false;
    if (pear3d.num_materials == 0) {
        use_default_material = true;
    }

    u32* material_indices = (u32*)malloc(sizeof(u32) * pear3d.num_materials);

    for (u32 i = 0; i < pear3d.num_materials; i++) {
        bool success;
        Pear3D_Image pear_image = pear3d_load_image(pear3d.materials[i].texture_diffuse, &success);
        if (!success) {
            PEAR_ERROR("failed to load pear3d image %s!", pear3d.materials[i].texture_diffuse);
            pear3d_delete_image(pear_image);
            material_indices[i] = material_add(material_get_default());
            continue;
        }

        Image* texture = image_new_from_pear3d(pear_image);
        
        Material material = {
            .texture_albedo = texture_new_from_image(texture, TEXTURE_WRAPPING_REPEAT, TEXTURE_FILTERING_NEAREST)
        };
        material_indices[i] = material_add(material);

        image_delete(texture);
        pear3d_delete_image(pear_image);
    }

    // then the meshes
    for (u32 i = 0; i < pear3d.num_meshes; i++) {
        MeshInfo* mesh_info = meshinfo_new();
        
        vec3* positions = (vec3*)malloc(sizeof(vec3) * pear3d.meshes[i].num_vertices);
        vec2* texture_coords = (vec2*)malloc(sizeof(vec2) * pear3d.meshes[i].num_vertices);
        vec3* normals = (vec3*)malloc(sizeof(vec3) * pear3d.meshes[i].num_vertices);

        for (u32 j = 0; j < pear3d.meshes[i].num_vertices; j++) {
            glm_vec3_copy(pear3d.meshes[i].vertices[j].position, positions[j]);
            glm_vec2_copy(pear3d.meshes[i].vertices[j].texture_coords, texture_coords[j]);
            glm_vec3_copy(pear3d.meshes[i].vertices[j].normal, normals[j]);
        }

        meshinfo_add_attribute_vec3(mesh_info, false, positions, pear3d.meshes[i].num_vertices);
        meshinfo_add_attribute_vec2(mesh_info, false, texture_coords, pear3d.meshes[i].num_vertices);
        meshinfo_add_attribute_vec3(mesh_info, false, normals, pear3d.meshes[i].num_vertices);

        meshes[i] = mesh_new(mesh_info, material_indices[pear3d.meshes[i].material_index], pear3d.meshes[i].indices, pear3d.meshes[i].num_indices);

        meshinfo_delete(mesh_info);
        free(positions);
        free(texture_coords);
        free(normals);
    }

    free(material_indices);

    return model_new(meshes, pear3d.num_meshes);
}

void model_delete(Model* model) {
    for (u32 i = 0; i < model->num_meshes; i++) {
        mesh_delete(model->meshes[i]);
    }

    free(model->meshes);
    free(model);
}

Mesh** model_get_meshes(Model* model) {
    return model->meshes;
}

u32 model_get_num_meshes(Model* model) {
    return model->num_meshes;
}
