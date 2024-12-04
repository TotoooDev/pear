#ifdef PEAR_PLATFORM_OPENGL

#include <graphics/renderer.h>
#include <graphics/mesh.h>
#include <graphics/mesh_info.h>
#include <graphics/texture.h>
#include <graphics/camera.h>
#include <graphics/platform/opengl/shader.h>
#include <graphics/platform/opengl/mesh.h>
#include <graphics/platform/opengl/mesh_info.h>
#include <graphics/platform/opengl/texture.h>
#include <scene/components/transform.h>
#include <scene/components/drawable.h>
#include <scene/components/camera.h>
#include <event/event_dispatcher.h>
#include <util/filesystem.h>
#include <util/array.h>
#include <core/app.h>
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

    glm_mat4_identity(renderer->view_matrix);

    renderer_set_viewport(renderer);
    renderer_calculate_projection(renderer);

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
    shader_use(renderer->shader);
    shader_set_u32(renderer->shader, 0, "u_platform");
    shader_set_mat4(renderer->shader, renderer->projection_matrix, "u_projection");
    shader_set_mat4(renderer->shader, renderer->view_matrix, "u_view");
    shader_set_i32(renderer->shader, 0, "u_texture");


    for (u32 i = 0; i < array_get_length(scene_get_entities(scene)); i++) {
        entity_t* entity = array_get(scene_get_entities(scene), i);
        if (entity_has_component(entity, ENTITY_COMPONENT_DRAWABLE) && entity_has_component(entity, ENTITY_COMPONENT_TRANSFORM)) {
            transform_component_t* transform = (transform_component_t*)entity_get_component(entity, ENTITY_COMPONENT_TRANSFORM);
            drawable_component_t* drawable = (drawable_component_t*)entity_get_component(entity, ENTITY_COMPONENT_DRAWABLE);

            mat4 model_matrix;
            transformcomponent_get_model_matrix(transform, model_matrix);

            shader_set_mat4(renderer->shader, model_matrix, "u_model");

            texture_use(drawable->texture, 0);
            mesh_use(drawable->mesh);
            glDrawElements(GL_TRIANGLES, mesh_get_num_indices(drawable->mesh), GL_UNSIGNED_INT, 0);
        }

        if (entity_has_component(entity, ENTITY_COMPONENT_CAMERA) && entity_has_component(entity, ENTITY_COMPONENT_TRANSFORM)) {
            transform_component_t* transform = (transform_component_t*)entity_get_component(entity, ENTITY_COMPONENT_TRANSFORM);
            camera_component_t* camera = (camera_component_t*)entity_get_component(entity, ENTITY_COMPONENT_CAMERA);

            if (camera->use) {
                camera_get_view_matrix(transform->pos, transform->rotation[0], transform->rotation[1], transform->rotation[2], renderer->view_matrix);
            }
        }
    }
}

#endif
