#ifdef PEAR_PLATFORM_OPENGL

#ifndef PEAR_GRAPHICS_OPENGL_MESH_INFO_H_
#define PEAR_GRAPHICS_OPENGL_MESH_INFO_H_

#include <graphics/mesh_info.h>

void meshinfo_get_vertices(mesh_info_t* mesh_info, f32* vertices);
u32 meshinfo_get_vertices_size(mesh_info_t* mesh_info);

#endif

#endif
