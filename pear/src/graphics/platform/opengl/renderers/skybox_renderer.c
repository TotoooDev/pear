#ifdef PEAR_PLATFORM_OPENGL

#include <graphics/platform/opengl/renderers/skybox_renderer.h>
#include <graphics/platform/opengl/renderer.h>
#include <graphics/platform/opengl/shader.h>
#include <graphics/platform/opengl/mesh.h>
#include <graphics/platform/opengl/cubemap.h>
#include <scene/components/skybox.h>
#include <util/array.h>
#include <core/alloc.h>
#include <GL/glew.h>

typedef struct skybox_renderer_t {
    shader_t* shader;
    mesh_t* cubemap_mesh;
    array_t* cubemaps;
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

void skyboxrenderer_system(scene_t* scene, entity_t* entity, f32 timestep, void* user_pointer) {
    if (!scene_has_component(scene, entity, "skybox")) {
        return;
    }

    renderer_interface_t* interface = (renderer_interface_t*)user_pointer;
    skybox_renderer_t* renderer = (skybox_renderer_t*)interface->renderer;
    skybox_component_t* skybox = scene_get_component(scene, entity, "skybox");
    if (!skybox->draw || skybox->cubemap == NULL) {
        return;
    }

    array_add(renderer->cubemaps, skybox->cubemap);
}

void skyboxrenderer_draw(renderer_interface_t* interface, renderer_t* renderer) {
    glDepthFunc(GL_LEQUAL);

    skybox_renderer_t* skybox_renderer = (skybox_renderer_t*)interface->renderer;
    for (u32 i = 0; i < array_get_length(skybox_renderer->cubemaps); i++) {
        cubemap_t* cubemap = array_get(skybox_renderer->cubemaps, i);

        shader_use(skybox_renderer->shader);
        shader_set_i32(skybox_renderer->shader, 0, "u_cubemap");

        mesh_use(skybox_renderer->cubemap_mesh);
        cubemap_use(cubemap, 0);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    glDepthFunc(GL_LESS);
}

void skyboxrenderer_clear(renderer_interface_t* interface, f32 r, f32 g, f32 b) {
    skybox_renderer_t* renderer = (skybox_renderer_t*)interface->renderer;
    array_clear(renderer->cubemaps);
}

void skyboxrenderer_delete(renderer_interface_t* interface) {
    skybox_renderer_t* skybox_renderer = (skybox_renderer_t*)interface->renderer;
    array_delete(skybox_renderer->cubemaps);
    mesh_delete(skybox_renderer->cubemap_mesh);
    shader_delete(skybox_renderer->shader);
    PEAR_FREE(skybox_renderer);
    PEAR_FREE(interface);
}

renderer_interface_t* skyboxrenderer_new(renderer_t* renderer) {
    skybox_renderer_t* skybox_renderer = (skybox_renderer_t*)PEAR_MALLOC(sizeof(skybox_renderer_t));
    skybox_renderer->shader = shader_new_from_file("shaders/skybox.vert", "shaders/skybox.frag");
    skybox_renderer->cubemaps = array_new(2);
    skyboxrenderer_init_mesh(skybox_renderer);

    ubo_t* matrices_ubo = renderer_get_matrices_ubo(renderer);
    shader_use(skybox_renderer->shader);
    shader_set_ubo(skybox_renderer->shader, matrices_ubo, "ubo_matrices");

    renderer_interface_t* interface = (renderer_interface_t*)PEAR_MALLOC(sizeof(renderer_interface_t));
    interface->system = skyboxrenderer_system;
    interface->draw_function = skyboxrenderer_draw;
    interface->clear_function = skyboxrenderer_clear;
    interface->delete_function = skyboxrenderer_delete;
    interface->renderer = skybox_renderer;

    return interface;
}

#endif
