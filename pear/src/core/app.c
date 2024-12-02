#include <core/app.h>
#include <core/timer.h>
#include <graphics/window.h>
#include <graphics/renderer.h>
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

    timer_init();
}

void app_stop() {
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
