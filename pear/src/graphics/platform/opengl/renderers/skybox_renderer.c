#ifdef PEAR_PLATFORM_OPENGL

#include <graphics/platform/opengl/renderers/skybox_renderer.h>
#include <graphics/platform/opengl/shader.h>
#include <graphics/platform/opengl/mesh.h>
#include <graphics/platform/opengl/cubemap.h>
#include <scene/components/skybox.h>
#include <core/alloc.h>
#include <GL/glew.h>

typedef struct skybox_renderer_t {
    ubo_t* ubo_matrices;
    shader_t* shader;
    mesh_t* cubemap_mesh;
} skybox_renderer_t;

void skyboxrenderer_init_mesh(skybox_renderer_t* renderer) {
    vec3 positions[] = {
        { -1.0f,  1.0f, -1.0f },
        { -1.0f, -1.0f, -1.0f },
        {  1.0f, -1.0f, -1.0f },
        {  1.0f, -1.0f, -1.0f },
        {  1.0f,  1.0f, -1.0f },
        { -1.0f,  1.0f, -1.0f },
        { -1.0f, -1.0f,  1.0f },
        { -1.0f, -1.0f, -1.0f },
        { -1.0f,  1.0f, -1.0f },
        { -1.0f,  1.0f, -1.0f },
        { -1.0f,  1.0f,  1.0f },
        { -1.0f, -1.0f,  1.0f },
        {  1.0f, -1.0f, -1.0f },
        {  1.0f, -1.0f,  1.0f },
        {  1.0f,  1.0f,  1.0f },
        {  1.0f,  1.0f,  1.0f },
        {  1.0f,  1.0f, -1.0f },
        {  1.0f, -1.0f, -1.0f },
        { -1.0f, -1.0f,  1.0f },
        { -1.0f,  1.0f,  1.0f },
        {  1.0f,  1.0f,  1.0f },
        {  1.0f,  1.0f,  1.0f },
        {  1.0f, -1.0f,  1.0f },
        { -1.0f, -1.0f,  1.0f },
        { -1.0f,  1.0f, -1.0f },
        {  1.0f,  1.0f, -1.0f },
        {  1.0f,  1.0f,  1.0f },
        {  1.0f,  1.0f,  1.0f },
        { -1.0f,  1.0f,  1.0f },
        { -1.0f,  1.0f, -1.0f },
        { -1.0f, -1.0f, -1.0f },
        { -1.0f, -1.0f,  1.0f },
        {  1.0f, -1.0f, -1.0f },
        {  1.0f, -1.0f, -1.0f },
        { -1.0f, -1.0f,  1.0f },
        {  1.0f, -1.0f,  1.0f }
    };

    mesh_info_t* mesh_info = meshinfo_new();
    meshinfo_add_position(mesh_info, positions, 36);

    renderer->cubemap_mesh = mesh_new(mesh_info, 0);

    meshinfo_delete(mesh_info);
}

skybox_renderer_t* skyboxrenderer_new(ubo_t* ubo_matrices) {
    skybox_renderer_t* renderer = (skybox_renderer_t*)PEAR_MALLOC(sizeof(skybox_renderer_t));
    
    renderer->ubo_matrices = ubo_matrices;
    renderer->shader = shader_new_from_file("shaders/skybox.vert", "shaders/skybox.frag");

    skyboxrenderer_init_mesh(renderer);
    
    return renderer;
}

void skyboxrenderer_delete(skybox_renderer_t* renderer) {
    mesh_delete(renderer->cubemap_mesh);
    shader_delete(renderer->shader);
    PEAR_FREE(renderer);
}

void skyboxrenderer_draw_scene(skybox_renderer_t* renderer, array_t* skyboxes) {
    for (u32 i = 0; i < array_get_length(skyboxes); i++) {
        skybox_component_t* skybox = array_get(skyboxes, i);

        if (!skybox->draw) {
            continue;
        }
        
        shader_use(renderer->shader);
        shader_set_ubo(renderer->shader, renderer->ubo_matrices, "ubo_matrices");
        shader_set_i32(renderer->shader, 0, "u_cubemap");

        mesh_use(renderer->cubemap_mesh);
        cubemap_use(skybox->cubemap, 0);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}


#endif
