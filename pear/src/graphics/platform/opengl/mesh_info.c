#ifdef PEAR_PLATFORM_OPENGL

#include <graphics/mesh_info.h>
#include <core/log.h>
#include <stdlib.h>

#define MESH_INFO_MAX_ATTRIBUTES 256

typedef struct MeshInfo {
    MeshAttribute attributes[MESH_INFO_MAX_ATTRIBUTES];
    u32 num_attributes;
    u32 stride;
} MeshInfo;

MeshInfo* meshinfo_new() {
    MeshInfo* mesh_info = (MeshInfo*)malloc(sizeof(MeshInfo));

    mesh_info->num_attributes = 0;
    mesh_info->stride = 0;

    return mesh_info;
}

void meshinfo_delete(MeshInfo* mesh_info) {
    free(mesh_info);
}

void meshinfo_add_attribute(MeshInfo* mesh_info, MeshDataType type, bool is_normalized) {
    if (mesh_info->num_attributes >= MESH_INFO_MAX_ATTRIBUTES) {
        PEAR_ERROR("failed to add mesh attribute! no space left.");
        return;
    }
    
    mesh_info->attributes[mesh_info->num_attributes].type = type;
    mesh_info->attributes[mesh_info->num_attributes].offset = mesh_info->stride;
    mesh_info->attributes[mesh_info->num_attributes].is_normalized = is_normalized;
    
    switch (type) {
    case MESH_DATA_TYPE_INT:
        mesh_info->stride += sizeof(int);
        break;

    case MESH_DATA_TYPE_UINT:
        mesh_info->stride += sizeof(unsigned int);
        break;

    case MESH_DATA_TYPE_FLOAT:
        mesh_info->stride += sizeof(float);
        break;

    case MESH_DATA_TYPE_FLOAT2:
        mesh_info->stride += sizeof(float) * 2;
        break;

    case MESH_DATA_TYPE_FLOAT3:
        mesh_info->stride += sizeof(float) * 3;
        break;

    case MESH_DATA_TYPE_FLOAT4:
        mesh_info->stride += sizeof(float) * 4;
        break;
    }

    mesh_info->num_attributes++;
}

MeshAttribute* meshinfo_get_attributes(MeshInfo* mesh_info) {
    return mesh_info->attributes;
}

u32 meshinfo_get_num_attributes(MeshInfo* mesh_info) {
    return mesh_info->num_attributes;
}

u32 meshinfo_get_stride(MeshInfo* mesh_info) {
    return mesh_info->stride;
}

#endif
