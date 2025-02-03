#ifdef PEAR_PLATFORM_OPENGL

#include <graphics/mesh.h>
#include <graphics/platform/opengl/mesh.h> 
#include <graphics/platform/opengl/mesh_info.h>
#include <core/log.h>
#include <glad/glad.h>
#include <core/alloc.h>

typedef struct mesh_t {
    u32 vao;
    u32 vbo;
    u32 ebo;

    u32 num_indices;
    u32 num_vertices;

    u32 material_index;
} mesh_t;

void mesh_enable_attribute(u32 index, u32 num_components, u32 offset, bool activate) {
    if (activate) {
        glVertexAttribPointer(index, num_components, GL_FLOAT, GL_FALSE, 0, (void*)offset);
        glEnableVertexAttribArray(index);
    }
}

mesh_t* mesh_new(mesh_info_t* mesh_info, u32 material_index) {
    mesh_t* mesh = (mesh_t*)PEAR_MALLOC(sizeof(mesh_t));

    mesh->material_index = material_index;
    mesh->num_indices = meshinfo_get_num_indices(mesh_info);
    mesh->num_vertices = meshinfo_get_num_positions(mesh_info);

    u32 vertices_size = meshinfo_get_vertices_size(mesh_info);
    f32* vertices = (f32*)PEAR_MALLOC(vertices_size);
    meshinfo_get_vertices(mesh_info, vertices);

    glGenVertexArrays(1, &mesh->vao);
    glGenBuffers(1, &mesh->vbo);
    glGenBuffers(1, &mesh->ebo);

    glBindVertexArray(mesh->vao);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshinfo_get_num_indices(mesh_info) * sizeof(u32), meshinfo_get_indices(mesh_info), GL_STATIC_DRAW);

    u32 offset = 0;

    mesh_enable_attribute(0, 3, offset, meshinfo_has_positions(mesh_info)); 
    offset += meshinfo_get_num_positions(mesh_info) * sizeof(vec3);

    mesh_enable_attribute(1, 2, offset, meshinfo_has_texture_coords(mesh_info)); 
    offset += meshinfo_get_num_positions(mesh_info) * sizeof(vec2);

    mesh_enable_attribute(2, 3, offset, meshinfo_has_normals(mesh_info)); 
    offset += meshinfo_get_num_positions(mesh_info) * sizeof(vec3);

    PEAR_FREE(vertices);

    return mesh;
}

void mesh_delete(mesh_t* mesh) {
    glDeleteVertexArrays(1, &(mesh->vao));
    glDeleteBuffers(1, &(mesh->vbo));
    glDeleteBuffers(1, &(mesh->ebo));

    PEAR_FREE(mesh);
}

u32 mesh_get_num_indices(mesh_t* mesh) {
    return mesh->num_indices;
}

void mesh_use(mesh_t* mesh) {
    glBindVertexArray(mesh->vao);
}

u32 mesh_get_material_index(mesh_t* mesh) {
    return mesh->material_index;
}

u32 mesh_get_num_vertices(mesh_t* mesh) {
    return mesh->num_vertices;
}

#endif
