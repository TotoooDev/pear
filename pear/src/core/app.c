#include <core/app.h>
#include <core/timer.h>
#include <graphics/window.h>
#include <graphics/renderer.h>
#include <event/event_dispatcher.h>
#include <core/log.h>
#include <stdlib.h>

typedef struct App {
    bool is_running;
    f32 timestep;
    f32 last_time;
    Window* window;
    Renderer* renderer;
    Node* root_node;
} App;

static App* app = NULL;

void app_on_event(EventType type, void* event, void* user_data) {
    if (type == EVENT_TYPE_QUIT)
        app_stop();
}

void app_update_timestep() {
    f32 current_time = timer_get_time_ms();
    app->timestep = current_time - app->last_time;
    app->last_time = current_time;
}

App* app_new() {
    PEAR_INFO("creating a new app! ");

    app = (App*)malloc(sizeof(App));

    app->is_running = true;
    app->timestep = 0.0f;
    app->last_time = 0.0f;
    app->window = window_new();
    app->renderer = renderer_new();
    app->root_node = NULL;

    event_subscribe(app_on_event, NULL);

    return app;
}

void app_delete() {
    renderer_delete(app->renderer);
    window_delete(app->window);
    free(app);
}

void app_init() {
    app = app_new();
    timer_init_time();
}

void app_stop() {
    PEAR_INFO("stopping the app...");
    app->is_running = false;
}

void app_run() {
    while (app_is_running()) {
        node_update_recursive(app->root_node, app->timestep);
        renderer_clear(app->renderer, 0.3f, 0.3f, 0.3f, 0.0f);
        renderer_draw_node_hierarchy(app->renderer, app->root_node);
        window_update(app->window);
        app_update_timestep();
    }

    app_delete();
}

bool app_is_running() {
    return app->is_running;
}

void app_set_root_node(Node* node) {
    app->root_node = node;
}

Window* app_get_window() {
    return app->window;
}
