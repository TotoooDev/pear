#include <graphics/model.h>
#include <core/log.h>
#include <core/alloc.h>
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

void model_delete(model_t* model) {
    for (u32 i = 0; i < model->num_meshes; i++) {
        mesh_delete(model->meshes[i]);
    }

    for (u32 i = 0; i < model->num_materials; i++) {
        if (model->materials[i].diffuse != NULL) {
            texture_delete(model->materials[i].diffuse);
        }
        if (model->materials[i].specular != NULL) {
            texture_delete(model->materials[i].specular);
        }
        if (model->materials[i].normal != NULL) {
            texture_delete(model->materials[i].normal);
        }
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
