#ifdef PEAR_PLATFORM_OPENGL

#include <graphics/mesh.h>
#include <graphics/platform/opengl/mesh.h> 
#include <core/log.h>
#include <GL/glew.h>
#include <stdlib.h>

typedef struct Mesh {
    u32 vao;
    u32 vbo;
    u32 ebo;

    u32 num_vertices;
    u32 num_indices;
} Mesh;

Mesh* mesh_new(f32* vertices, u32* indices, u32 num_vertices, u32 num_indices) {
    Mesh* mesh = (Mesh*)malloc(sizeof(Mesh));

    mesh->num_vertices = num_vertices;
    mesh->num_indices = num_indices;

    glGenVertexArrays(1, &(mesh->vao));
    glGenBuffers(1, &(mesh->vbo));
    glGenBuffers(1, &(mesh->ebo));

    glBindVertexArray(mesh->vao);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
    glBufferData(GL_ARRAY_BUFFER, num_vertices, vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_indices, indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, false, 6 * sizeof(f32), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, false, 6 * sizeof(f32), (void*)(3 * sizeof(f32)));
    glEnableVertexAttribArray(1);

    return mesh;
}

void mesh_delete(Mesh* mesh) {
    glDeleteVertexArrays(1, &(mesh->vao));
    glDeleteBuffers(1, &(mesh->vbo));
    glDeleteBuffers(1, &(mesh->ebo));

    free(mesh);
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
}

#endif
