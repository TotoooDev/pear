#ifndef PEAR_MESH_INFO_H
#define PEAR_MESH_INFO_H

#include <core/types.h>

typedef enum MeshDataType {
    MESH_DATA_TYPE_INT,
    MESH_DATA_TYPE_UINT,
    MESH_DATA_TYPE_FLOAT,
    MESH_DATA_TYPE_FLOAT2,
    MESH_DATA_TYPE_FLOAT3,
    MESH_DATA_TYPE_FLOAT4,
} MeshDataType;

typedef struct MeshAttribute {
    MeshDataType type;
    size_t offset;
    bool is_normalized;
} MeshAttribute;

typedef struct MeshInfo MeshInfo;

MeshInfo* meshinfo_new();
void meshinfo_delete(MeshInfo* mesh_info);

void meshinfo_add_attribute(MeshInfo* mesh_info, MeshDataType type, bool is_normalized);

MeshAttribute* meshinfo_get_attributes(MeshInfo* mesh_info);
u32 meshinfo_get_num_attributes(MeshInfo* mesh_info);
u32 meshinfo_get_stride(MeshInfo* mesh_info);

#endif
