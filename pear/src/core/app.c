#include <core/app.h>
#include <graphics/window.h>
#include <graphics/renderer.h>
#include <event/event_dispatcher.h>
#include <core/log.h>
#include <stdlib.h>

typedef struct App {
    bool is_running;
    Window* window;
    Renderer* renderer;
    Node* root_node;
} App;

static App* app = NULL;

void app_on_event(EventType type, void* event, void* user_data) {
    if (type == EVENT_TYPE_QUIT)
        app_stop();
}

App* app_new() {
    PEAR_INFO("creating a new app! ");

    app = (App*)malloc(sizeof(App));

    app->is_running = true;
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
}

void app_stop() {
    PEAR_INFO("stopping the app...");
    app->is_running = false;
}

void app_run() {
    while (app_is_running()) {
        renderer_clear(app->renderer, 0.3f, 0.3f, 0.3f, 0.0f);
        renderer_draw_node_hierarchy(app->renderer, app->root_node);
        window_update(app->window);
    }

    app_delete();
}

bool app_is_running() {
    return app->is_running;
}

void app_set_root_node(Node* node) {
    app->root_node = node;
}
