#ifndef PEAR_MESH_INFO_H
#define PEAR_MESH_INFO_H

#include <core/types.h>

typedef struct MeshAttribute {
    u32 num_components;
    bool is_normalized;

    f32* data;
    u32 num_data;

    bool was_malloc;
} MeshAttribute;

typedef struct MeshInfo MeshInfo;

MeshInfo* meshinfo_new();
void meshinfo_delete(MeshInfo* mesh_info);

void meshinfo_add_attribute_float(MeshInfo* mesh_info, bool is_normalized, f32* data, u32 num_data);
void meshinfo_add_attribute_vec2(MeshInfo* mesh_info, bool is_normalized, vec2* data, u32 num_data);
void meshinfo_add_attribute_vec3(MeshInfo* mesh_info, bool is_normalized, vec3* data, u32 num_data);
void meshinfo_add_attribute_vec4(MeshInfo* mesh_info, bool is_normalized, vec4* data, u32 num_data);

MeshAttribute* meshinfo_get_attributes(MeshInfo* mesh_info);
u32 meshinfo_get_num_attributes(MeshInfo* mesh_info);

void meshinfo_get_vertices(MeshInfo* mesh_info, f32* dest);
u32 meshinfo_get_num_vertices(MeshInfo* mesh_info);

#endif
