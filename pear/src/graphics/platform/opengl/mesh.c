#ifdef PEAR_PLATFORM_OPENGL

#include <graphics/mesh.h>
#include <graphics/platform/opengl/mesh.h> 
#include <graphics/platform/opengl/texture.h> 
#include <core/log.h>
#include <GL/glew.h>
#include <stdlib.h>

typedef struct Mesh {
    Material material;

    u32 vao;
    u32 vbo;
    u32 ebo;

    u32 num_vertices;
    u32 num_indices;
} Mesh;

Mesh* mesh_new(MeshInfo* mesh_info, Material material, u32* indices, u32 num_indices) {
    Mesh* mesh = (Mesh*)malloc(sizeof(Mesh));

    mesh->material = material;
    mesh->num_indices = num_indices;

    glGenVertexArrays(1, &(mesh->vao));
    glGenBuffers(1, &(mesh->vbo));
    glGenBuffers(1, &(mesh->ebo));

    glBindVertexArray(mesh->vao);

    f32* vertices = (f32*)malloc(sizeof(f32) * meshinfo_get_num_vertices(mesh_info));
    meshinfo_get_vertices(mesh_info, vertices);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
    glBufferData(GL_ARRAY_BUFFER, meshinfo_get_num_vertices(mesh_info) * sizeof(f32), vertices, GL_STATIC_DRAW);
    free(vertices);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_indices, indices, GL_STATIC_DRAW);

    u32 total_offset = 0;
    MeshAttribute* attributes = meshinfo_get_attributes(mesh_info);
    for (u32 i = 0; i < meshinfo_get_num_attributes(mesh_info); i++) {
        MeshAttribute attribute = attributes[i];

        glVertexAttribPointer(i, attribute.num_components, GL_FLOAT, attribute.is_normalized, 0, (void*)(sizeof(f32) * total_offset));
        glEnableVertexAttribArray(i);

        total_offset += attribute.num_data;
    }

    return mesh;
}

void mesh_delete(Mesh* mesh) {
    glDeleteVertexArrays(1, &(mesh->vao));
    glDeleteBuffers(1, &(mesh->vbo));
    glDeleteBuffers(1, &(mesh->ebo));

    free(mesh);
}

Material* mesh_get_material(Mesh* mesh) {
    return &(mesh->material);
}

u32 mesh_get_num_vertices(Mesh* mesh) {
    return mesh->num_vertices;
}

u32 mesh_get_num_indices(Mesh* mesh) {
    return mesh->num_indices;
}

void mesh_use(Mesh* mesh) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
    glBindVertexArray(mesh->vao);

    if (mesh->material.albedo != NULL) {
        glActiveTexture(GL_TEXTURE0);
        texture_use(mesh->material.albedo);
    }
}

#endif
