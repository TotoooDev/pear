#include <graphics/mesh_info.h>
#include <core/alloc.h>
#include <string.h>

typedef struct mesh_info_t {
    u32* indices;
    vec3* positions;
    vec2* texture_coords;
    vec3* normals;

    u32 num_indices;
    u32 num_positions;
    u32 num_texture_coords;
    u32 num_normals;
} mesh_info_t;

mesh_info_t* meshinfo_new() {
    mesh_info_t* mesh_info = (mesh_info_t*)PEAR_MALLOC(sizeof(mesh_info_t));

    mesh_info->indices = NULL;
    mesh_info->positions = NULL;
    mesh_info->texture_coords = NULL;
    mesh_info->normals = NULL;

    mesh_info->num_indices = 0;
    mesh_info->num_positions = 0;
    mesh_info->num_texture_coords = 0;
    mesh_info->num_normals = 0;

    return mesh_info;
}

void meshinfo_delete(mesh_info_t* mesh_info) {
    PEAR_FREE(mesh_info->indices);
    PEAR_FREE(mesh_info->positions);
    PEAR_FREE(mesh_info->texture_coords);
    PEAR_FREE(mesh_info->normals);

    PEAR_FREE(mesh_info);
}

void meshinfo_add_indices(mesh_info_t* mesh_info, u32* indices, u32 num_indices) {
    u32 indices_size = sizeof(u32) * num_indices;
    mesh_info->indices = (u32*)PEAR_MALLOC(indices_size);
    memcpy(mesh_info->indices, indices, indices_size);
    mesh_info->num_indices = num_indices;
}

void meshinfo_add_position(mesh_info_t* mesh_info, vec3* positions, u32 num_positions) {
    u32 positions_size = sizeof(vec3) * num_positions;
    mesh_info->positions = (vec3*)PEAR_MALLOC(positions_size);
    memcpy(mesh_info->positions, positions, positions_size);
    mesh_info->num_positions = num_positions;
}

void meshinfo_add_texture_coords(mesh_info_t* mesh_info, vec2* texture_coords, u32 num_texture_coords) {
    u32 texture_coords_size = sizeof(vec2) * num_texture_coords;
    mesh_info->texture_coords = (vec2*)PEAR_MALLOC(texture_coords_size);
    memcpy(mesh_info->texture_coords, texture_coords, texture_coords_size);
    mesh_info->num_texture_coords = num_texture_coords;
}

void meshinfo_add_normal(mesh_info_t* mesh_info, vec3* normals, u32 num_normals) {
    u32 normals_size = sizeof(vec3) * num_normals;
    mesh_info->normals = (vec3*)PEAR_MALLOC(normals_size);
    memcpy(mesh_info->normals, normals, normals_size);
    mesh_info->num_normals = num_normals;
}

u32* meshinfo_get_indices(mesh_info_t* mesh_info) {
    return mesh_info->indices;
}

vec3* meshinfo_get_positions(mesh_info_t* mesh_info) {
    return mesh_info->positions;
}

vec2* meshinfo_get_texture_coords(mesh_info_t* mesh_info) {
    return mesh_info->texture_coords;
}

vec3* meshinfo_get_normals(mesh_info_t* mesh_info) {
    return mesh_info->normals;
}

u32 meshinfo_get_num_indices(mesh_info_t* mesh_info) {
    return mesh_info->num_indices;
}

u32 meshinfo_get_num_positions(mesh_info_t* mesh_info) {
    return mesh_info->num_positions;
}

u32 meshinfo_get_num_texture_coords(mesh_info_t* mesh_info) {
    return mesh_info->num_texture_coords;
}

u32 meshinfo_get_num_normals(mesh_info_t* mesh_info) {
    return mesh_info->num_normals;
}

bool meshinfo_has_positions(mesh_info_t* mesh_info) {
    return mesh_info->num_positions != 0 && mesh_info->positions != NULL;
}

bool meshinfo_has_texture_coords(mesh_info_t* mesh_info) {
    return mesh_info->num_texture_coords != 0 && mesh_info->texture_coords != NULL;
}

bool meshinfo_has_normals(mesh_info_t* mesh_info) {
    return mesh_info->num_normals != 0 && mesh_info->normals != NULL;
}
