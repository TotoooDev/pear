#ifdef PEAR_PLATFORM_OPENGL

#include <graphics/renderer.h>
#include <graphics/framebuffer.h>
#include <graphics/mesh.h>
#include <graphics/window.h>
#include <graphics/platform/opengl/renderers/scene_renderer.h>
#include <graphics/platform/opengl/renderers/screen_renderer.h>
#include <graphics/platform/opengl/framebuffer.h>
#include <graphics/platform/opengl/ubo.h>
#include <graphics/platform/opengl/ubo_info.h>
#include <event/event_dispatcher.h>
#include <core/app.h>
#include <core/log.h>
#include <GL/glew.h>
#include <core/alloc.h>

#define RENDERER_NUM_MAX_LIGHTS 128

typedef struct renderer_t {
    f32 viewport_width;
    f32 viewport_height;
    f32 viewport_scale_x;
    f32 viewport_scale_y;
    f32 viewport_width_scaled;
    f32 viewport_height_scaled;

    framebuffer_t* screen_framebuffer;
    texture_t* screen_texture;
    texture_t* screen_depth_texture;

    ubo_t* ubo_matrices;
    ubo_t* ubo_lights;

    scene_renderer_t* scene_renderer;
    screen_renderer_t* screen_renderer;
} renderer_t;

void renderer_init_screen_framebuffer(renderer_t* renderer) {
    renderer->screen_framebuffer = framebuffer_new();
    renderer->screen_texture = texture_new(renderer->viewport_width_scaled, renderer->viewport_height_scaled, TEXTURE_WRAPPING_NONE, TEXTURE_FILTERING_LINEAR, TEXTURE_FORMAT_RGB);
    renderer->screen_depth_texture = texture_new(renderer->viewport_width_scaled, renderer->viewport_height_scaled, TEXTURE_WRAPPING_NONE, TEXTURE_FILTERING_LINEAR, TEXTURE_FORMAT_DEPTH);
    framebuffer_add_texture(renderer->screen_framebuffer, renderer->screen_texture);
    framebuffer_add_texture(renderer->screen_framebuffer, renderer->screen_depth_texture);
}

void renderer_on_event(event_type_t type, void* e, void* user_data) {
    renderer_t* renderer = (renderer_t*)user_data;

    if (type == EVENT_TYPE_WINDOW_RESIZED) {
        window_resized_event_t* event = (window_resized_event_t*)e;
        renderer->viewport_width = event->width;
        renderer->viewport_height = event->height;
        renderer->viewport_width_scaled = renderer->viewport_width * renderer->viewport_scale_x;
        renderer->viewport_height_scaled = renderer->viewport_height * renderer->viewport_scale_y;

        framebuffer_delete(renderer->screen_framebuffer);
        texture_delete(renderer->screen_texture);
        texture_delete(renderer->screen_depth_texture);
        renderer_init_screen_framebuffer(renderer);
        screenrenderer_set_screen_texture(renderer->screen_renderer, renderer->screen_texture);
    }

    if (type == EVENT_TYPE_WINDOW_SCALE_CHANGED) {
        window_scale_changed_event_t* event = (window_scale_changed_event_t*)e;
        renderer->viewport_scale_x = event->scale_x;
        renderer->viewport_scale_y = event->scale_y;
        renderer->viewport_width_scaled = renderer->viewport_width * event->scale_x;
        renderer->viewport_height_scaled = renderer->viewport_height * event->scale_y;

        framebuffer_delete(renderer->screen_framebuffer);
        texture_delete(renderer->screen_texture);
        texture_delete(renderer->screen_depth_texture);
        renderer_init_screen_framebuffer(renderer);
        screenrenderer_set_screen_texture(renderer->screen_renderer, renderer->screen_texture);
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
    uboinfo_add_mat4(info); // model
    uboinfo_add_mat4(info); // view
    uboinfo_add_mat4(info); // projection
    uboinfo_add_mat4(info); // model transpose inverse
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

renderer_t* renderer_new() {
    GLenum res = glewInit();
    if (res != GLEW_OK) {
        PEAR_ERROR("failed to initialize glew! %s", glewGetErrorString(res));
        return NULL;
    }

    renderer_setup_debug_output();

    renderer_t* renderer = (renderer_t*)PEAR_MALLOC(sizeof(renderer_t));
    renderer->viewport_width = window_get_width(app_get_window());
    renderer->viewport_height = window_get_height(app_get_window());
    renderer->viewport_scale_x = window_get_scale_x(app_get_window());
    renderer->viewport_scale_y = window_get_scale_y(app_get_window());
    renderer->viewport_width_scaled = renderer->viewport_width * window_get_scale_x(app_get_window());
    renderer->viewport_height_scaled = renderer->viewport_height * window_get_scale_y(app_get_window());
    
    renderer_init_ubo_matrices(renderer);
    renderer_init_ubo_lights(renderer);
    renderer_init_screen_framebuffer(renderer);
    
    renderer->scene_renderer = scenerenderer_new(renderer->ubo_matrices, renderer->ubo_lights);
    renderer->screen_renderer = screenrenderer_new(renderer->screen_texture);

    event_subscribe(renderer_on_event, renderer);

    return renderer;
}

void renderer_delete(renderer_t* renderer) {
    scenerenderer_delete(renderer->scene_renderer);
    screenrenderer_delete(renderer->screen_renderer);

    PEAR_FREE(renderer);
}

void renderer_clear(renderer_t* renderer, f32 r, f32 g, f32 b) {
    framebuffer_use(renderer->screen_framebuffer);
    scenerenderer_clear(renderer->scene_renderer, 0.2f, 0.2f, 0.2f);
}

void renderer_draw_scene(renderer_t* renderer, scene_t* scene) {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    framebuffer_use(renderer->screen_framebuffer);
    scenerenderer_draw_scene(renderer->scene_renderer, scene);

    glDisable(GL_DEPTH_TEST);
    framebuffer_use_default();
    screenrenderer_render_to_screen(renderer->screen_renderer);
}

#endif
