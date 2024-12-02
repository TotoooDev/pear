#ifdef PEAR_PLATFORM_OPENGL

#include <graphics/renderer.h>
#include <graphics/mesh.h>
#include <graphics/mesh_info.h>
#include <graphics/texture.h>
#include <graphics/platform/opengl/shader.h>
#include <graphics/platform/opengl/mesh.h>
#include <graphics/platform/opengl/mesh_info.h>
#include <graphics/platform/opengl/texture.h>
#include <core/app.h>
#include <event/event_dispatcher.h>
#include <util/filesystem.h>
#include <core/log.h>
#include <GL/glew.h>
#include <core/alloc.h>

typedef struct renderer_t {
    f32 fov;
    f32 aspect_ratio;
    f32 near;
    f32 far;

    f32 viewport_width;
    f32 viewport_height;
    f32 viewport_scale_x;
    f32 viewport_scale_y;
    f32 viewport_width_scaled;
    f32 viewport_height_scaled;

    mat4 projection_matrix;
    mat4 view_matrix;

    shader_t* shader;
    mesh_t* mesh;
    texture_t* texture;
    
    camera_t camera;
} renderer_t;

void renderer_calculate_projection(renderer_t* renderer) {
    glm_perspective(renderer->fov, renderer->aspect_ratio, renderer->near, renderer->far, renderer->projection_matrix);
}

void renderer_set_viewport(renderer_t* renderer) {
    glViewport(0, 0, renderer->viewport_width_scaled, renderer->viewport_height_scaled);
}

void renderer_on_event(event_type_t type, void* e, void* user_data) {
    renderer_t* renderer = (renderer_t*)user_data;

    if (type == EVENT_TYPE_WINDOW_RESIZED) {
        window_resized_event_t* event = (window_resized_event_t*)e;
        renderer->viewport_width = event->width;
        renderer->viewport_height = event->height;
        renderer->viewport_width_scaled = renderer->viewport_width * renderer->viewport_scale_x;
        renderer->viewport_height_scaled = renderer->viewport_height * renderer->viewport_scale_y;
        renderer->aspect_ratio = renderer->viewport_width / renderer->viewport_height;
        
        renderer_calculate_projection(renderer);
        renderer_set_viewport(renderer);
    }

    if (type == EVENT_TYPE_WINDOW_SCALE_CHANGED) {
        window_scale_changed_event_t* event = (window_scale_changed_event_t*)e;
        renderer->viewport_scale_x = event->scale_x;
        renderer->viewport_scale_y = event->scale_y;
        renderer->viewport_width_scaled = renderer->viewport_width * event->scale_x;
        renderer->viewport_height_scaled = renderer->viewport_height * event->scale_y;

        renderer_set_viewport(renderer);
    }
}

renderer_t* renderer_new() {
    GLenum res = glewInit();
    if (res != GLEW_OK) {
        PEAR_ERROR("failed to initialize glew! %s", glewGetErrorString(res));
        return NULL;
    }

    renderer_t* renderer = (renderer_t*)PEAR_MALLOC(sizeof(renderer_t));

    renderer->aspect_ratio = window_get_width(app_get_window()) / (f32)window_get_height(app_get_window());
    renderer->fov = glm_rad(45.0f);
    renderer->near = 0.01f;
    renderer->far = 100.0f;
    renderer->viewport_width = window_get_width(app_get_window());
    renderer->viewport_height = window_get_height(app_get_window());
    renderer->viewport_scale_x = window_get_scale_x(app_get_window());
    renderer->viewport_scale_y = window_get_scale_y(app_get_window());
    renderer->viewport_width_scaled = renderer->viewport_width * window_get_scale_x(app_get_window());
    renderer->viewport_height_scaled = renderer->viewport_height * window_get_scale_y(app_get_window());
    renderer->shader = shader_new(fileystem_read_file("shaders/shader.vert"), fileystem_read_file("shaders/shader.frag"));
    renderer->camera = camera_new_at_origin();
    renderer->camera.pos[2] = 2.0f;

    renderer_set_viewport(renderer);
    renderer_calculate_projection(renderer);
    camera_get_view_matrix(renderer->camera, renderer->view_matrix);

    vec3 positions[] = {
        {0.5f,  0.5f, 0.0f},
        {0.5f, -0.5f, 0.0f},
        {-0.5f, -0.5f, 0.0f},
        {-0.5f,  0.5f, 0.0f}
    };
    vec3 colors[] = {
        {1.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 1.0f},
        {1.0f, 0.0f, 1.0f}
    };
    vec2 texture_coords[] = {
        {1.0f, 1.0f},
        {1.0f, 0.0f},
        {0.0f, 0.0f},
        {0.0f, 1.0f}
    };
    u32 indices[] = {
        0, 1, 3,
        1, 2, 3
    };
    mesh_info_t* mesh_info = meshinfo_new();
    meshinfo_add_indices(mesh_info, indices, 6);
    meshinfo_add_position(mesh_info, positions, 4);
    meshinfo_add_color(mesh_info, colors, 4);
    meshinfo_add_texture_coords(mesh_info, texture_coords, 4);

    renderer->mesh = mesh_new(mesh_info);

    meshinfo_delete(mesh_info);
    
    bool success;
    pear_image_t pear_image = pear_image_load("wall.image", &success);
    image_t* image = image_new_from_pear_image(pear_image);
    renderer->texture = texture_new_from_image(image, TEXTURE_WRAPPING_NONE, TEXTURE_FILTERING_NEAREST);
    image_delete(image);

    event_subscribe(renderer_on_event, renderer);

    return renderer;
}

void renderer_delete(renderer_t* renderer) {
    PEAR_FREE(renderer);
}

void renderer_clear(renderer_t* renderer, f32 r, f32 g, f32 b) {
    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void renderer_draw_scene(renderer_t* renderer, scene_t* scene) {
    mat4 model_matrix;
    glm_mat4_identity(model_matrix);

    shader_use(renderer->shader);
    shader_set_u32(renderer->shader, 0, "u_platform");
    shader_set_mat4(renderer->shader, renderer->projection_matrix, "u_projection");
    shader_set_mat4(renderer->shader, renderer->view_matrix, "u_view");
    shader_set_mat4(renderer->shader, model_matrix, "u_model");
    shader_set_i32(renderer->shader, 0, "u_texture");

    texture_use(renderer->texture, 0);
    mesh_use(renderer->mesh);
    glDrawElements(GL_TRIANGLES, mesh_get_num_indices(renderer->mesh), GL_UNSIGNED_INT, 0);
}

void renderer_set_camera(renderer_t* renderer, camera_t camera) {
    renderer->camera = camera;
    camera_get_view_matrix(renderer->camera, renderer->view_matrix);
}

camera_t renderer_get_camera(renderer_t* renderer) {
    return renderer->camera;
}

#endif
