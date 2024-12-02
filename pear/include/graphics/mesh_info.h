#ifndef PEAR_GRAPHICS_MESH_INFO_H_
#define PEAR_GRAPHICS_MESH_INFO_H_

#include <core/types.h>

typedef struct mesh_info_t mesh_info_t;

mesh_info_t* meshinfo_new();
void meshinfo_delete(mesh_info_t* mesh_info);

void meshinfo_add_indices(mesh_info_t* mesh_info, u32* indices, u32 num_indices);
void meshinfo_add_position(mesh_info_t* mesh_info, vec3* positions, u32 num_positions);
void meshinfo_add_color(mesh_info_t* mesh_info, vec3* colors, u32 num_colors);
void meshinfo_add_texture_coords(mesh_info_t* mesh_info, vec2* texture_coords, u32 num_texture_coords);
void meshinfo_add_normal(mesh_info_t* mesh_info, vec3* normals, u32 num_normals);

u32* meshinfo_get_indices(mesh_info_t* mesh_info);
vec3* meshinfo_get_positions(mesh_info_t* mesh_info);
vec3* meshinfo_get_colors(mesh_info_t* mesh_info);
vec2* meshinfo_get_texture_coords(mesh_info_t* mesh_info);
vec3* meshinfo_get_normals(mesh_info_t* mesh_info);

u32 meshinfo_get_num_indices(mesh_info_t* mesh_info);
u32 meshinfo_get_num_positions(mesh_info_t* mesh_info);
u32 meshinfo_get_num_colors(mesh_info_t* mesh_info);
u32 meshinfo_get_num_texture_coords(mesh_info_t* mesh_info);
u32 meshinfo_get_num_normals(mesh_info_t* mesh_info);

bool meshinfo_has_positions(mesh_info_t* mesh_info);
bool meshinfo_has_colors(mesh_info_t* mesh_info);
bool meshinfo_has_texture_coords(mesh_info_t* mesh_info);
bool meshinfo_has_normals(mesh_info_t* mesh_info);

#endif
