#include <core/app.h>
#include <core/timer.h>
#include <graphics/window.h>
#include <graphics/renderer.h>
#include <graphics/renderers/skybox_renderer.h>
#include <graphics/renderers/model_renderer.h>
#include <graphics/renderers/shadow_renderer.h>
#include <graphics/renderers/billboard_renderer.h>
#include <graphics/renderers/screen_renderer.h>
#include <graphics/editor/editor.h>
#include <script/script.h>
#include <event/event_dispatcher.h>
#include <core/types.h>
#include <core/log.h>
#include <core/alloc.h>

typedef struct app_t {
    bool is_running;
    f32 timestep;
    f32 last_time;

    window_t* window;
    renderer_t* renderer;
    scene_t* scene;

    #ifdef PEAR_ENABLE_EDITOR
        bool enable_editor;
    #endif
} app_t;

static app_t* app = NULL;

void app_on_event(event_type_t type, void* event, void* user_data) {
    if (type == EVENT_TYPE_QUIT) {
        app->is_running = false;
    }
}

void app_update_timestep() {
    f32 current_time = timer_get_time_ms();
    app->timestep = current_time - app->last_time;
    app->last_time = current_time;
}

void app_init() {
    app = (app_t*)PEAR_MALLOC(sizeof(app_t));

    event_init();

    app->is_running = true;
    app->timestep = 0.0f;
    app->last_time = 0.0f;
    app->scene = scene_new();
    app->window = window_new("pear", 1080, 720);

    app->renderer = renderer_new();
    renderer_add_renderer_interface_before(app->renderer, shadowrenderer_new(app->renderer));
    renderer_add_renderer_interface(app->renderer, modelrenderer_new(app->renderer));
    renderer_add_renderer_interface(app->renderer, billboardrenderer_new(app->renderer));
    renderer_add_renderer_interface_after(app->renderer, skyboxrenderer_new(app->renderer));

    #ifdef PEAR_ENABLE_EDITOR
        app->enable_editor = true;
    #endif

    scene_register_system(app->scene, script_system, NULL);
    event_subscribe(app_on_event, NULL);

    #ifdef PEAR_ENABLE_EDITOR
        editor_init();
    #endif
    timer_init();

    event_send(EVENT_TYPE_SCENE_NEW, &(scene_new_event_t){ .old_scene = NULL, .new_scene = app->scene });
}

void app_stop() {
    #ifdef PEAR_ENABLE_EDITOR
        editor_free();
    #endif
    renderer_delete(app->renderer);
    window_delete(app->window);
    scene_delete(app->scene);
    event_free();
    PEAR_FREE(app);
}

void app_run() {
    while (app->is_running) {
        app_update_timestep();
        renderer_clear(app->renderer, 0.2f, 0.2f, 0.2f);

        scene_update(app->scene, app->timestep);
        renderer_draw(app->renderer);

        #ifdef PEAR_ENABLE_EDITOR
            if (app->enable_editor) {
                editor_clear();
                editor_render();
            }
        #endif

        window_update(app->window);
    }

    app_stop();
}

scene_t* app_get_scene() {
    return app->scene;
}

void app_set_scene(scene_t* scene) {
    event_send(EVENT_TYPE_SCENE_NEW, &(scene_new_event_t){ .old_scene = app->scene, .new_scene = scene });

    if (app->scene != NULL) {
        scene_delete(app->scene);
    }
    app->scene = scene;

    scene_register_system(app->scene, script_system, NULL);
}

window_t* app_get_window() {
    return app->window;
}

renderer_t* app_get_renderer() {
    return app->renderer;
}

f32 app_get_timestep() {
    return app->timestep;
}

u32 app_get_version_major() {
    return 0;
}

u32 app_get_version_minor() {
    return 0;
}

u32 app_get_version_patch() {
    return 1;
}

const char* app_get_version_string() {
    #ifdef PEAR_DEBUG
        return "pear 0.0.1 (debug)";
    #endif
    #ifdef PEAR_RELEASE
        return "pear 0.0.1 (release)";
    #endif
}

#ifdef PEAR_ENABLE_EDITOR
void app_disable_editor() {
    app->enable_editor = false;
}
#endif
