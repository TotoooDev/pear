#ifdef PEAR_PLATFORM_OPENGL

#include <graphics/platform/opengl/renderer.h>
#include <graphics/framebuffer.h>
#include <graphics/mesh.h>
#include <graphics/window.h>
#include <graphics/camera.h>
#include <graphics/platform/opengl/renderers/skybox_renderer.h>
#include <graphics/platform/opengl/framebuffer.h>
#include <graphics/platform/opengl/ubo.h>
#include <graphics/platform/opengl/ubo_info.h>
#include <graphics/platform/opengl/texture.h>
#include <scene/components/transform.h>
#include <scene/components/camera.h>
#include <scene/components/model.h>
#include <scene/components/skybox.h>
#include <scene/components/billboard.h>
#include <scene/components/light.h>
#include <event/event_dispatcher.h>
#include <core/app.h>
#include <core/log.h>
#include <GL/glew.h>
#include <core/alloc.h>

#define RENDERER_NUM_MAX_LIGHTS 128
#define RENDERER_SHADOW_MAP_SIZE 4096

typedef struct renderer_t {
    bool enable_wireframe;

    u32 num_meshes;
    u32 num_vertices;

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

    mat4 projection;
    mat4 view;
    vec3 camera_pos;

    array_t* interfaces;

    framebuffer_t* screen_framebuffer;
    texture_t* screen_texture;
    texture_t* screen_depth_texture;

    framebuffer_t* shadow_framebuffer;
    texture_t* shadow_map;

    ubo_t* ubo_matrices;
    ubo_t* ubo_lights;
} renderer_t;

void renderer_system(scene_t* scene, entity_t* entity, f32 timestep, void* user_data) {
    if (!scene_has_component(scene, entity, "camera") || !scene_has_component(scene, entity, "transform")) {
        return;
    }

    camera_component_t* camera = scene_get_component(scene, entity, "camera");
    if (!camera->use) {
        return;
    }

    renderer_t* renderer = (renderer_t*)user_data;
    transform_component_t* transform = scene_get_component(scene, entity, "transform");
    camera_get_view_matrix(transform->pos, transform->rotation[0], transform->rotation[1], transform->rotation[2], renderer->view);
}

void renderer_init_screen_framebuffer(renderer_t* renderer) {
    renderer->screen_framebuffer = framebuffer_new();
    renderer->screen_texture = texture_new(renderer->viewport_width_scaled, renderer->viewport_height_scaled, TEXTURE_WRAPPING_NONE, TEXTURE_FILTERING_LINEAR, TEXTURE_FORMAT_RGB);
    renderer->screen_depth_texture = texture_new(renderer->viewport_width_scaled, renderer->viewport_height_scaled, TEXTURE_WRAPPING_NONE, TEXTURE_FILTERING_LINEAR, TEXTURE_FORMAT_DEPTH);
    framebuffer_add_texture(renderer->screen_framebuffer, renderer->screen_texture);
    framebuffer_add_texture(renderer->screen_framebuffer, renderer->screen_depth_texture);
}

void renderer_calculate_projection(renderer_t* renderer) {
    glm_perspective(renderer->fov, renderer->aspect_ratio, renderer->near, renderer->far, renderer->projection);

    ubo_use(renderer->ubo_matrices);
    ubo_set_mat4(renderer->ubo_matrices, 2, renderer->projection);
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

        framebuffer_delete(renderer->screen_framebuffer);

        renderer->screen_framebuffer = framebuffer_new();
        texture_resize(renderer->screen_texture, renderer->viewport_width_scaled, renderer->viewport_height_scaled);
        texture_resize(renderer->screen_depth_texture, renderer->viewport_width_scaled, renderer->viewport_height_scaled);
        framebuffer_add_texture(renderer->screen_framebuffer, renderer->screen_texture);
        framebuffer_add_texture(renderer->screen_framebuffer, renderer->screen_depth_texture);
    }

    if (type == EVENT_TYPE_WINDOW_SCALE_CHANGED) {
        window_scale_changed_event_t* event = (window_scale_changed_event_t*)e;
        renderer->viewport_scale_x = event->scale_x;
        renderer->viewport_scale_y = event->scale_y;
        renderer->viewport_width_scaled = renderer->viewport_width * event->scale_x;
        renderer->viewport_height_scaled = renderer->viewport_height * event->scale_y;

        renderer_calculate_projection(renderer);

        framebuffer_delete(renderer->screen_framebuffer);
        texture_delete(renderer->screen_texture);
        texture_delete(renderer->screen_depth_texture);
        renderer_init_screen_framebuffer(renderer);
    }

    if (type == EVENT_TYPE_SCENE_NEW) {
        scene_new_event_t* event = (scene_new_event_t*)e;
        for (u32 i = 0; i < array_get_length(renderer->interfaces); i++) {
            renderer_interface_t* interface = array_get(renderer->interfaces, i);
            scene_register_system(event->new_scene, renderer_system, renderer);
            scene_register_system(event->new_scene, interface->system, interface);
        }
    }
}

void renderer_opengl_debug_output(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char *message, const void *userParam) {
    if(id == 131169 || id == 131185 || id == 131218 || id == 131204) {
        return;
    }

    switch (severity) {
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
    uboinfo_add_mat4(info); // model
    uboinfo_add_mat4(info); // view
    uboinfo_add_mat4(info); // projection
    uboinfo_add_mat4(info); // model transpose inverse
    uboinfo_add_mat4(info); // light space transform
    renderer->ubo_matrices = ubo_new(info, true);
    uboinfo_delete(info);
}

void renderer_init_ubo_lights(renderer_t* renderer) {
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

void renderer_init_shadow_framebuffer(renderer_t* renderer) {
    renderer->shadow_map = texture_new(RENDERER_SHADOW_MAP_SIZE, RENDERER_SHADOW_MAP_SIZE, TEXTURE_WRAPPING_NONE, TEXTURE_FILTERING_LINEAR, TEXTURE_FORMAT_DEPTH);
    texture_set_border_color(renderer->shadow_map, (vec4){ 1.0f, 1.0f, 1.0f, 1.0f });

    renderer->shadow_framebuffer = framebuffer_new();
    framebuffer_add_texture(renderer->shadow_framebuffer, renderer->shadow_map);
    framebuffer_set_depth_only(renderer->shadow_framebuffer);
}

renderer_t* renderer_new() {
    GLenum res = glewInit();
    if (res != GLEW_OK) {
        PEAR_ERROR("failed to initialize glew! %s", glewGetErrorString(res));
        return NULL;
    }

    renderer_setup_debug_output();

    renderer_t* renderer = (renderer_t*)PEAR_MALLOC(sizeof(renderer_t));
    renderer->enable_wireframe = false;
    renderer->fov = glm_rad(45.0f);
    renderer->near = 0.1f;
    renderer->far = 30.0f;
    renderer->viewport_width = window_get_width(app_get_window());
    renderer->viewport_height = window_get_height(app_get_window());
    renderer->viewport_scale_x = window_get_scale_x(app_get_window());
    renderer->viewport_scale_y = window_get_scale_y(app_get_window());
    renderer->viewport_width_scaled = renderer->viewport_width * window_get_scale_x(app_get_window());
    renderer->viewport_height_scaled = renderer->viewport_height * window_get_scale_y(app_get_window());
    renderer->aspect_ratio = renderer->viewport_width / renderer->viewport_height;
    renderer->interfaces = array_new(10);

    renderer_init_ubo_matrices(renderer);
    renderer_init_ubo_lights(renderer);
    renderer_init_screen_framebuffer(renderer);
    renderer_init_shadow_framebuffer(renderer);

    array_add(renderer->interfaces, skyboxrenderer_new(renderer));

    renderer_calculate_projection(renderer);

    event_subscribe(renderer_on_event, renderer);

    return renderer;
}

void renderer_delete(renderer_t* renderer) {
    framebuffer_delete(renderer->screen_framebuffer);
    framebuffer_delete(renderer->shadow_framebuffer);
    texture_delete(renderer->screen_texture);
    texture_delete(renderer->screen_depth_texture);
    ubo_delete(renderer->ubo_matrices);
    ubo_delete(renderer->ubo_lights);

    PEAR_FREE(renderer);
}

void renderer_clear(renderer_t* renderer, f32 r, f32 g, f32 b) {
    framebuffer_use(renderer->screen_framebuffer);

    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (u32 i = 0; i < array_get_length(renderer->interfaces); i++) {
        renderer_interface_t* interface = array_get(renderer->interfaces, i);
        interface->clear_function(interface, r, g, b);
    }

    framebuffer_use_default();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderer->num_meshes = 0;
    renderer->num_vertices = 0;
}

void renderer_draw(renderer_t* renderer) {
    ubo_use(renderer->ubo_matrices);
    ubo_set_mat4(renderer->ubo_matrices, 1, renderer->view);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glViewport(0, 0, renderer->viewport_width_scaled, renderer->viewport_height_scaled);
    framebuffer_use(renderer->screen_framebuffer);

    for (u32 i = 0; i < array_get_length(renderer->interfaces); i++) {
        renderer_interface_t* interface = array_get(renderer->interfaces, i);
        interface->draw_function(interface, renderer);
    }

    framebuffer_use_default();
}

void renderer_add_renderer_interface(renderer_t* renderer, renderer_interface_t* interface) {
    array_add(renderer->interfaces, interface);
    scene_register_system(app_get_scene(), interface->system, interface);
}

u32 renderer_get_num_meshes(renderer_t* renderer) {
    return renderer->num_meshes;
}

u32 renderer_get_num_vertices(renderer_t* renderer) {
    return renderer->num_vertices;
}

texture_t* renderer_get_screen_texture(renderer_t* renderer) {
    return renderer->screen_texture;
}

texture_t* renderer_get_screen_depth_texture(renderer_t* renderer) {
    return renderer->screen_depth_texture;
}

void renderer_get_view_matrix(renderer_t* renderer, mat4 dest) {
    glm_mat4_copy(renderer->view, dest);
}

void renderer_get_projection_matrix(renderer_t* renderer, mat4 dest) {
    glm_mat4_copy(renderer->projection, dest);
}

void renderer_set_viewport_size(renderer_t* renderer, f32 width, f32 height, f32 width_scale, f32 height_scale) {
    renderer->viewport_width = width;
    renderer->viewport_height = height;
    renderer->viewport_scale_x = width_scale;
    renderer->viewport_scale_y = height_scale;
    renderer->viewport_width_scaled = renderer->viewport_width;
    renderer->viewport_height_scaled = renderer->viewport_height;
    renderer->aspect_ratio = renderer->viewport_width / renderer->viewport_height;

    texture_resize(renderer->screen_texture, renderer->viewport_width_scaled, renderer->viewport_height_scaled);
    texture_resize(renderer->screen_depth_texture, renderer->viewport_width_scaled, renderer->viewport_height_scaled);

    renderer_calculate_projection(renderer);
}

void renderer_set_near(renderer_t* renderer, f32 near) {
    renderer->near = near;
    renderer_calculate_projection(renderer);
}

void renderer_set_far(renderer_t* renderer, f32 far) {
    renderer->far = far;
    renderer_calculate_projection(renderer);
}

void renderer_set_fov(renderer_t* renderer, f32 fov) {
    renderer->fov = glm_rad(fov);
    renderer_calculate_projection(renderer);
}

void renderer_enable_wireframe(renderer_t* renderer, bool active) {
    renderer->enable_wireframe = active;
}

ubo_t* renderer_get_matrices_ubo(renderer_t* renderer) {
    return renderer->ubo_matrices;
}

ubo_t* renderer_get_lights_ubo(renderer_t* renderer) {
    return renderer->ubo_lights;
}

#endif
