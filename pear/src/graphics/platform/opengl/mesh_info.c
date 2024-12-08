#ifdef PEAR_PLATFORM_OPENGL

#include <graphics/platform/opengl/mesh_info.h>

void meshinfo_add_attribute_vec3(mesh_info_t* mesh_info, f32* vertices, vec3* attributes, u32 num_attributes, u32* index) {
    for (u32 i = 0; i < num_attributes; i++) {
        vertices[*index + i * 3] = attributes[i][0];
        vertices[*index + i * 3 + 1] = attributes[i][1];
        vertices[*index + i * 3 + 2] = attributes[i][2];
    }
    *index += num_attributes * 3;
}

void meshinfo_add_attribute_vec2(mesh_info_t* mesh_info, f32* vertices, vec2* attributes, u32 num_attributes, u32* index) {
    for (u32 i = 0; i < num_attributes; i++) {
        vertices[*index + i * 2] = attributes[i][0];
        vertices[*index + i * 2 + 1] = attributes[i][1];
    }
    *index += num_attributes * 2;
}

void meshinfo_get_vertices(mesh_info_t* mesh_info, f32* vertices) {
    u32 index = 0;

    meshinfo_add_attribute_vec3(mesh_info, vertices, meshinfo_get_positions(mesh_info), meshinfo_get_num_positions(mesh_info), &index);
    meshinfo_add_attribute_vec2(mesh_info, vertices, meshinfo_get_texture_coords(mesh_info), meshinfo_get_num_texture_coords(mesh_info), &index);
    meshinfo_add_attribute_vec3(mesh_info, vertices, meshinfo_get_normals(mesh_info), meshinfo_get_num_normals(mesh_info), &index);
}

u32 meshinfo_get_vertices_size(mesh_info_t* mesh_info) {
    return meshinfo_get_num_positions(mesh_info) * sizeof(vec3) +
        meshinfo_get_num_texture_coords(mesh_info) * sizeof(vec2) +
        meshinfo_get_num_normals(mesh_info) * sizeof(vec3);
}

#endif
