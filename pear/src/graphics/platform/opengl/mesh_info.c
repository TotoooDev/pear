#ifdef PEAR_PLATFORM_OPENGL

#include <graphics/mesh_info.h>
#include <core/log.h>
#include <stdlib.h>

#define MESH_INFO_MAX_ATTRIBUTES 256

typedef struct MeshInfo {
    MeshAttribute attributes[MESH_INFO_MAX_ATTRIBUTES];
    u32 num_attributes;
    u32 num_vertices_total;
} MeshInfo;

void meshinfo_add_attribute(MeshInfo* mesh_info, u32 num_components, bool is_normalized, f32* data, u32 num_data, bool was_malloc) {
    if (mesh_info->num_attributes >= MESH_INFO_MAX_ATTRIBUTES) {
        PEAR_ERROR("failed to add mesh attribute! no space left.");
        return;
    }
    
    mesh_info->attributes[mesh_info->num_attributes].num_components = num_components;
    mesh_info->attributes[mesh_info->num_attributes].is_normalized = is_normalized;
    mesh_info->attributes[mesh_info->num_attributes].data = data;
    mesh_info->attributes[mesh_info->num_attributes].num_data = num_data;
    mesh_info->attributes[mesh_info->num_attributes].was_malloc = was_malloc;
    
    mesh_info->num_vertices_total += num_data;
    mesh_info->num_attributes++;
}

MeshInfo* meshinfo_new() {
    MeshInfo* mesh_info = (MeshInfo*)malloc(sizeof(MeshInfo));

    mesh_info->num_attributes = 0;
    mesh_info->num_vertices_total = 0;

    return mesh_info;
}

void meshinfo_delete(MeshInfo* mesh_info) {
    for (u32 i = 0; i < mesh_info->num_attributes; i++) {
        if (mesh_info->attributes[i].was_malloc) {
            free(mesh_info->attributes[i].data);
        }
    }
    free(mesh_info);
}

void meshinfo_add_attribute_float(MeshInfo* mesh_info, bool is_normalized, f32* data, u32 num_data) {
    meshinfo_add_attribute(mesh_info, 1, is_normalized, data, num_data, false);
}

void meshinfo_add_attribute_vec2(MeshInfo* mesh_info, bool is_normalized, vec2* data, u32 num_data) {
    f32* float_data = (f32*)malloc(sizeof(f32) * 2 * num_data);
    u32 index = 0;
    for (u32 i = 0; i < num_data; i++) {
        float_data[index++] = data[i][0];
        float_data[index++] = data[i][1];
    }
    meshinfo_add_attribute(mesh_info, 2, is_normalized, float_data, num_data * 2, true);
}

void meshinfo_add_attribute_vec3(MeshInfo* mesh_info, bool is_normalized, vec3* data, u32 num_data) {
    f32* float_data = (f32*)malloc(sizeof(f32) * 3 * num_data);
    u32 index = 0;
    for (u32 i = 0; i < num_data; i++) {
        float_data[index++] = data[i][0];
        float_data[index++] = data[i][1];
        float_data[index++] = data[i][2];
    }
    meshinfo_add_attribute(mesh_info, 3, is_normalized, float_data, num_data * 3, true);
}

void meshinfo_add_attribute_vec4(MeshInfo* mesh_info, bool is_normalized, vec4* data, u32 num_data) {
    f32* float_data = (f32*)malloc(sizeof(f32) * 4 * num_data);
    u32 index = 0;
    for (u32 i = 0; i < num_data; i++) {
        float_data[index++] = data[i][0];
        float_data[index++] = data[i][1];
        float_data[index++] = data[i][2];
        float_data[index++] = data[i][3];
    }
    meshinfo_add_attribute(mesh_info, 4, is_normalized, float_data, num_data * 4, true);
}

MeshAttribute* meshinfo_get_attributes(MeshInfo* mesh_info) {
    return mesh_info->attributes;
}

u32 meshinfo_get_num_attributes(MeshInfo* mesh_info) {
    return mesh_info->num_attributes;
}

void meshinfo_get_vertices(MeshInfo* mesh_info, f32* dest) {
    u32 index = 0;
    for (u32 i = 0; i < mesh_info->num_attributes; i++) {
        for (u32 j = 0; j < mesh_info->attributes[i].num_data; j++) {
            dest[index] = mesh_info->attributes[i].data[j];
            index++;
        }
    }
}

u32 meshinfo_get_num_vertices(MeshInfo* mesh_info) {
    return mesh_info->num_vertices_total;
}

#endif
