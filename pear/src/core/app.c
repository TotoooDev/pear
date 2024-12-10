#include <core/app.h>
#include <core/timer.h>
#include <graphics/window.h>
#include <graphics/renderer.h>
#include <graphics/gui/gui.h>
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

    app->is_running = true;
    app->timestep = 0.0f;
    app->last_time = 0.0f;
    app->window = window_new("pear", 1080, 720);
    app->renderer = renderer_new();
    app->scene = scene_new();

    event_subscribe(app_on_event, NULL);

    gui_init();

    timer_init();
}

void app_stop() {
    gui_free();
    renderer_delete(app->renderer);
    window_delete(app->window);
    scene_delete(app->scene);
    PEAR_FREE(app);
}

void app_run() {
    while (app->is_running) {
        app_update_timestep();
        scene_update(app->scene, app->timestep);

        renderer_clear(app->renderer, 0.5f, 0.5f, 0.5f);
        renderer_draw_scene(app->renderer, app->scene);

        gui_clear();
        gui_render();

        window_update(app->window);
    }

    app_stop();
}

scene_t* app_get_scene() {
    return app->scene;
}

void app_set_scene(scene_t* scene) {
    app->scene = scene;
}

window_t* app_get_window() {
    return app->window;
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
    return "pear 0.0.1";
}
