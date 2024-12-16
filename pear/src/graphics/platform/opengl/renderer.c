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
#include <graphics/platform/opengl/ubo_info.h>
#include <graphics/platform/opengl/ubo.h>
#include <scene/components/transform.h>
#include <scene/components/model.h>
#include <scene/components/camera.h>
#include <scene/components/light.h>
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

    array_t* models;
    array_t* lights;
    array_t* model_transforms;
    array_t* light_transforms;

    mat4 projection_matrix;
    mat4 view_matrix;

    ubo_t* ubo_matrices;

    shader_t* shader;
} renderer_t;

void renderer_calculate_projection(renderer_t* renderer) {
    glm_perspective(renderer->fov, renderer->aspect_ratio, renderer->near, renderer->far, renderer->projection_matrix);
    ubo_set_mat4(renderer->ubo_matrices, 2, renderer->projection_matrix);
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

void renderer_opengl_debug_output(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char *message, const void *userParam) {
    if(id == 131169 || id == 131185 || id == 131218 || id == 131204) {
        return;
    }

    switch (type) {
    case GL_DEBUG_SEVERITY_HIGH:
        PEAR_ERROR("(%d) %s", id, message);
        break;

    case GL_DEBUG_SEVERITY_MEDIUM:
    case GL_DEBUG_SEVERITY_LOW:
        PEAR_WARN("(%d) %s", id, message);
        break;

    case GL_DEBUG_SEVERITY_NOTIFICATION:
        PEAR_INFO("(%d) %s", id, message);
        break;
    }
}

void renderer_setup_debug_output() {
#ifdef PEAR_DEBUG
    i32 flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (!(flags & GL_CONTEXT_FLAG_DEBUG_BIT)) {
        PEAR_WARN("failed to initialize opengl debug output! good luck debugging >:)");
        return;
    }

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(renderer_opengl_debug_output, NULL);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
#endif
}

void renderer_init_ubo_matrices(renderer_t* renderer) {
    ubo_info_t* info = uboinfo_new();
    uboinfo_add_mat4(info);
    uboinfo_add_mat4(info);
    uboinfo_add_mat4(info);
    renderer->ubo_matrices = ubo_new(info, true);
    uboinfo_delete(info);
}

void renderer_draw_mesh(renderer_t* renderer, mesh_t* mesh , material_t material, mat4 model_matrix) {
    shader_set_i32(renderer->shader, 0, "u_texture");
    ubo_set_mat4(renderer->ubo_matrices, 0, model_matrix);

    texture_use(material.diffuse, 0);
    mesh_use(mesh);
    glDrawElements(GL_TRIANGLES, mesh_get_num_indices(mesh), GL_UNSIGNED_INT, 0);
}

void renderer_handle_model(renderer_t* renderer, entity_t* entity) {
    transform_component_t* transform = (transform_component_t*)entity_get_component(entity, ENTITY_COMPONENT_TRANSFORM);
    model_component_t* model = (model_component_t*)entity_get_component(entity, ENTITY_COMPONENT_MODEL);

    if (!model->draw) {
        return;
    }

    array_add(renderer->models, model->model);
    array_add(renderer->model_transforms, transform);
}

void renderer_handle_camera(renderer_t* renderer, entity_t* entity) {
    transform_component_t* transform = (transform_component_t*)entity_get_component(entity, ENTITY_COMPONENT_TRANSFORM);
    camera_component_t* camera = (camera_component_t*)entity_get_component(entity, ENTITY_COMPONENT_CAMERA);

    if (camera->use) {
        camera_get_view_matrix(transform->pos, transform->rotation[0], transform->rotation[1], transform->rotation[2], renderer->view_matrix);
    }
}

void renderer_handle_light(renderer_t* renderer, entity_t* entity) {
    transform_component_t* transform = (transform_component_t*)entity_get_component(entity, ENTITY_COMPONENT_TRANSFORM);
    light_component_t* light = (light_component_t*)entity_get_component(entity, ENTITY_COMPONENT_LIGHT);

    if (!light->cast) {
        return;
    }

    array_add(renderer->lights, &light->light);
    array_add(renderer->light_transforms, transform);
}

void renderer_render(renderer_t* renderer) {
    ubo_set_mat4(renderer->ubo_matrices, 1, renderer->view_matrix);

    for (u32 i = 0; i < array_get_length(renderer->models); i++) {
        model_t* model = array_get(renderer->models, i);
        transform_component_t* transform = array_get(renderer->model_transforms, i);

        mat4 model_matrix;
        transformcomponent_get_model_matrix(transform, model_matrix);

        for (u32 j = 0; j < model_get_num_meshes(model); j++) {
            mesh_t* mesh = model_get_meshes(model)[j];
            material_t material = model_get_materials(model)[mesh_get_material_index(mesh)];
            renderer_draw_mesh(renderer, mesh, material, model_matrix);
        }
    }
}

renderer_t* renderer_new() {
    GLenum res = glewInit();
    if (res != GLEW_OK) {
        PEAR_ERROR("failed to initialize glew! %s", glewGetErrorString(res));
        return NULL;
    }

    renderer_setup_debug_output();

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
    renderer->lights = array_new(10);
    renderer->models = array_new(10);
    renderer->light_transforms = array_new(10);
    renderer->model_transforms = array_new(10);
    renderer->shader = shader_new(fileystem_read_file("shaders/shader.vert"), fileystem_read_file("shaders/shader.frag"));

    renderer_init_ubo_matrices(renderer);

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
    glEnable(GL_DEPTH_TEST);
    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    array_clear(renderer->lights);
    array_clear(renderer->models);
    array_clear(renderer->light_transforms);
    array_clear(renderer->model_transforms);
}

void renderer_draw_scene(renderer_t* renderer, scene_t* scene) {
    for (u32 i = 0; i < array_get_length(scene_get_entities(scene)); i++) {
        entity_t* entity = array_get(scene_get_entities(scene), i);
        if (entity_has_component(entity, ENTITY_COMPONENT_MODEL) && entity_has_component(entity, ENTITY_COMPONENT_TRANSFORM)) {
            renderer_handle_model(renderer, entity);
        }

        if (entity_has_component(entity, ENTITY_COMPONENT_CAMERA) && entity_has_component(entity, ENTITY_COMPONENT_TRANSFORM)) {
            renderer_handle_camera(renderer, entity);
        }

        if (entity_has_component(entity, ENTITY_COMPONENT_LIGHT) && entity_has_component(entity, ENTITY_COMPONENT_TRANSFORM)) {
            renderer_handle_light(renderer, entity);
        }
    }
    
    shader_use(renderer->shader);
    shader_set_u32(renderer->shader, 0, "u_platform");
    shader_set_ubo(renderer->shader, renderer->ubo_matrices, "ubo_matrices");
    renderer_render(renderer);
}

#endif
