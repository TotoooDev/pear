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

Mesh* mesh_new(MeshInfo* mesh_info, Material material, f32* vertices, u32* indices, u32 num_vertices, u32 num_indices) {
    Mesh* mesh = (Mesh*)malloc(sizeof(Mesh));

    mesh->material = material;
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

    MeshAttribute* attributes = meshinfo_get_attributes(mesh_info);
    for (u32 i = 0; i < meshinfo_get_num_attributes(mesh_info); i++) {
        MeshAttribute attribute = attributes[i];
        u32 gl_type;
        u32 num_components;

        switch (attribute.type) {
        case MESH_DATA_TYPE_INT:
            gl_type = GL_INT;
            num_components = 1;
            break;

        case MESH_DATA_TYPE_UINT:
            gl_type = GL_UNSIGNED_INT;
            num_components = 1;
            break;

        case MESH_DATA_TYPE_FLOAT:
            gl_type = GL_FLOAT;
            num_components = 1;
            break;

        case MESH_DATA_TYPE_FLOAT2:
            gl_type = GL_FLOAT;
            num_components = 2;
            break;

        case MESH_DATA_TYPE_FLOAT3:
            gl_type = GL_FLOAT;
            num_components = 3;
            break;

        case MESH_DATA_TYPE_FLOAT4:
            gl_type = GL_FLOAT;
            num_components = 4;
            break;
        }

        glVertexAttribPointer(i, num_components, gl_type, attribute.is_normalized, meshinfo_get_stride(mesh_info), (void*)attribute.offset);
        glEnableVertexAttribArray(i);
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
