#ifdef PEAR_PLATFORM_OPENGL

#include <graphics/platform/opengl/renderers/scene_renderer.h>
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

#define RENDERER_NUM_MAX_LIGHTS 128

typedef struct scene_renderer_t {
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
    ubo_t* ubo_lights;

    shader_t* shader;

    u32 light_num_components;
    vec3 camera_pos;
} scene_renderer_t;

void scenerenderer_calculate_projection(scene_renderer_t* renderer) {
    glm_perspective(renderer->fov, renderer->aspect_ratio, renderer->near, renderer->far, renderer->projection_matrix);

    ubo_use(renderer->ubo_matrices);
    ubo_set_mat4(renderer->ubo_matrices, 2, renderer->projection_matrix);
}

void scenerenderer_set_viewport(scene_renderer_t* renderer) {
    glViewport(0, 0, renderer->viewport_width_scaled, renderer->viewport_height_scaled);
}

void scenerenderer_on_event(event_type_t type, void* e, void* user_data) {
    scene_renderer_t* renderer = (scene_renderer_t*)user_data;

    if (type == EVENT_TYPE_WINDOW_RESIZED) {
        window_resized_event_t* event = (window_resized_event_t*)e;
        renderer->viewport_width = event->width;
        renderer->viewport_height = event->height;
        renderer->viewport_width_scaled = renderer->viewport_width * renderer->viewport_scale_x;
        renderer->viewport_height_scaled = renderer->viewport_height * renderer->viewport_scale_y;
        renderer->aspect_ratio = renderer->viewport_width / renderer->viewport_height;

        scenerenderer_calculate_projection(renderer);
        scenerenderer_set_viewport(renderer);
    }

    if (type == EVENT_TYPE_WINDOW_SCALE_CHANGED) {
        window_scale_changed_event_t* event = (window_scale_changed_event_t*)e;
        renderer->viewport_scale_x = event->scale_x;
        renderer->viewport_scale_y = event->scale_y;
        renderer->viewport_width_scaled = renderer->viewport_width * event->scale_x;
        renderer->viewport_height_scaled = renderer->viewport_height * event->scale_y;

        scenerenderer_set_viewport(renderer);
    }
}

void scenerenderer_init_shaders(scene_renderer_t* renderer) {
    renderer->shader = shader_new_from_file("shaders/shader.vert", "shaders/shader.frag");
}

void scenerenderer_init_ubo_matrices(scene_renderer_t* renderer) {
    ubo_info_t* info = uboinfo_new();
    uboinfo_add_mat4(info); // model
    uboinfo_add_mat4(info); // view
    uboinfo_add_mat4(info); // projection
    uboinfo_add_mat4(info); // model transpose inverse
    renderer->ubo_matrices = ubo_new(info, true);
    uboinfo_delete(info);
}

void scenerenderer_init_ubo_lights(scene_renderer_t* renderer) {
    ubo_info_t* info = uboinfo_new();

    uboinfo_add_u32(info); // num lights
    uboinfo_add_vec3(info); // view pos
    uboinfo_pad_to_16_alignment(info);
    for (u32 i = 0; i < RENDERER_NUM_MAX_LIGHTS; i++) {
        uboinfo_add_u32(info); // type
        uboinfo_add_vec3(info); // pos
        uboinfo_add_vec3(info); // direction
        uboinfo_add_vec3(info); // ambient
        uboinfo_add_vec3(info); // diffuse
        uboinfo_add_vec3(info); // specular
        uboinfo_add_f32(info); // constant
        uboinfo_add_f32(info); // linear
        uboinfo_add_f32(info); // quadratic
        uboinfo_add_f32(info); // cutoff
        uboinfo_add_f32(info); // outer cutoff
        uboinfo_pad_to_16_alignment(info);
    }

    renderer->ubo_lights = ubo_new(info, true);
    uboinfo_delete(info);
}

void scenerenderer_draw_mesh(scene_renderer_t* renderer, mesh_t* mesh , material_t material, mat4 model_matrix) {
    shader_set_i32(renderer->shader, 0, "u_material.diffuse");
    shader_set_i32(renderer->shader, 1, "u_material.specular");
    shader_set_vec3(renderer->shader, material.color, "u_material.color");
    shader_set_f32(renderer->shader, material.shininess, "u_material.shininess");

    mat4 model_transpose_inverse;
    glm_mat4_inv(model_matrix, model_transpose_inverse);
    glm_mat4_transpose(model_transpose_inverse);

    ubo_use(renderer->ubo_matrices);
    ubo_set_mat4(renderer->ubo_matrices, 0, model_matrix);
    ubo_set_mat4(renderer->ubo_matrices, 3, model_transpose_inverse);

    if (material.diffuse != NULL) {
        texture_use(material.diffuse, 0);
    }
    if (material.specular != NULL) {
        texture_use(material.specular, 1);
    }
    mesh_use(mesh);
    glDrawElements(GL_TRIANGLES, mesh_get_num_indices(mesh), GL_UNSIGNED_INT, 0);
}

void scenerenderer_handle_model(scene_renderer_t* renderer, entity_t* entity) {
    transform_component_t* transform = (transform_component_t*)entity_get_component(entity, ENTITY_COMPONENT_TRANSFORM);
    model_component_t* model = (model_component_t*)entity_get_component(entity, ENTITY_COMPONENT_MODEL);

    if (!model->draw) {
        return;
    }

    array_add(renderer->models, model->model);
    array_add(renderer->model_transforms, transform);
}

void scenerenderer_handle_camera(scene_renderer_t* renderer, entity_t* entity) {
    transform_component_t* transform = (transform_component_t*)entity_get_component(entity, ENTITY_COMPONENT_TRANSFORM);
    camera_component_t* camera = (camera_component_t*)entity_get_component(entity, ENTITY_COMPONENT_CAMERA);

    if (camera->use) {
        camera_get_view_matrix(transform->pos, transform->rotation[0], transform->rotation[1], transform->rotation[2], renderer->view_matrix);
        glm_vec3_copy(transform->pos, renderer->camera_pos);
    }
}

void scenerenderer_handle_light(scene_renderer_t* renderer, entity_t* entity) {
    transform_component_t* transform = (transform_component_t*)entity_get_component(entity, ENTITY_COMPONENT_TRANSFORM);
    light_component_t* light = (light_component_t*)entity_get_component(entity, ENTITY_COMPONENT_LIGHT);

    if (!light->cast) {
        return;
    }

    array_add(renderer->lights, &light->light);
    array_add(renderer->light_transforms, transform);
}

void scenerenderer_render(scene_renderer_t* renderer) {
    ubo_use(renderer->ubo_lights);
    ubo_set_u32(renderer->ubo_lights, 0, array_get_length(renderer->lights));
    ubo_set_vec3(renderer->ubo_lights, 1, renderer->camera_pos);

    for (u32 i = 0; i < array_get_length(renderer->lights); i++) {
        light_t* light = array_get(renderer->lights, i);
        transform_component_t* transform = array_get(renderer->light_transforms, i);

        u32 index = 2 + i * renderer->light_num_components;

        ubo_set_u32 (renderer->ubo_lights, index    ,  light->type);
        ubo_set_vec3(renderer->ubo_lights, index + 1,  transform->pos);
        ubo_set_vec3(renderer->ubo_lights, index + 2,  transform->rotation);
        ubo_set_vec3(renderer->ubo_lights, index + 3,  light->ambient);
        ubo_set_vec3(renderer->ubo_lights, index + 4,  light->diffuse);
        ubo_set_vec3(renderer->ubo_lights, index + 5,  light->specular);
        ubo_set_f32 (renderer->ubo_lights, index + 6,  light->constant);
        ubo_set_f32 (renderer->ubo_lights, index + 7,  light->linear);
        ubo_set_f32 (renderer->ubo_lights, index + 8,  light->quadratic);
        ubo_set_f32 (renderer->ubo_lights, index + 9,  light->cutoff);
        ubo_set_f32 (renderer->ubo_lights, index + 10, light->outer_cutoff);
    }

    ubo_use(renderer->ubo_matrices);
    ubo_set_mat4(renderer->ubo_matrices, 1, renderer->view_matrix);

    for (u32 i = 0; i < array_get_length(renderer->models); i++) {
        model_t* model = array_get(renderer->models, i);
        transform_component_t* transform = array_get(renderer->model_transforms, i);

        mat4 model_matrix;
        transformcomponent_get_model_matrix(transform, model_matrix);

        for (u32 j = 0; j < model_get_num_meshes(model); j++) {
            mesh_t* mesh = model_get_meshes(model)[j];
            material_t material = model_get_materials(model)[mesh_get_material_index(mesh)];
            scenerenderer_draw_mesh(renderer, mesh, material, model_matrix);
        }
    }
}

scene_renderer_t* scenerenderer_new() {
    GLenum res = glewInit();
    if (res != GLEW_OK) {
        PEAR_ERROR("failed to initialize glew! %s", glewGetErrorString(res));
        return NULL;
    }

    scene_renderer_t* renderer = (scene_renderer_t*)PEAR_MALLOC(sizeof(scene_renderer_t));

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
    renderer->light_num_components = 11;

    scenerenderer_init_shaders(renderer);
    scenerenderer_init_ubo_matrices(renderer);
    scenerenderer_init_ubo_lights(renderer);

    glm_mat4_identity(renderer->view_matrix);

    scenerenderer_set_viewport(renderer);
    scenerenderer_calculate_projection(renderer);

    event_subscribe(scenerenderer_on_event, renderer);

    return renderer;
}

void scenerenderer_delete(scene_renderer_t* renderer) {
    array_delete(renderer->models);
    array_delete(renderer->model_transforms);
    array_delete(renderer->lights);
    array_delete(renderer->light_transforms);

    ubo_delete(renderer->ubo_matrices);
    ubo_delete(renderer->ubo_lights);

    shader_delete(renderer->shader);

    PEAR_FREE(renderer);
}

void scenerenderer_clear(scene_renderer_t* renderer, f32 r, f32 g, f32 b) {
    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    array_clear(renderer->lights);
    array_clear(renderer->models);
    array_clear(renderer->light_transforms);
    array_clear(renderer->model_transforms);
}

void scenerenderer_draw_scene(scene_renderer_t* renderer, scene_t* scene) {
    for (u32 i = 0; i < array_get_length(scene_get_entities(scene)); i++) {
        entity_t* entity = array_get(scene_get_entities(scene), i);
        if (entity_has_component(entity, ENTITY_COMPONENT_MODEL) && entity_has_component(entity, ENTITY_COMPONENT_TRANSFORM)) {
            scenerenderer_handle_model(renderer, entity);
        }

        if (entity_has_component(entity, ENTITY_COMPONENT_CAMERA) && entity_has_component(entity, ENTITY_COMPONENT_TRANSFORM)) {
            scenerenderer_handle_camera(renderer, entity);
        }

        if (entity_has_component(entity, ENTITY_COMPONENT_LIGHT) && entity_has_component(entity, ENTITY_COMPONENT_TRANSFORM)) {
            scenerenderer_handle_light(renderer, entity);
        }
    }
    
    shader_use(renderer->shader);
    shader_set_u32(renderer->shader, 0, "u_platform");
    shader_set_ubo(renderer->shader, renderer->ubo_matrices, "ubo_matrices");
    shader_set_ubo(renderer->shader, renderer->ubo_lights, "ubo_lights");
    scenerenderer_render(renderer);
}

#endif
