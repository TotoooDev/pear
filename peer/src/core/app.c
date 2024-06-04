#include <core/app.h>
#include <graphics/window.h>
#include <event/event_dispatcher.h>
#include <core/log.h>
#include <stdlib.h>

typedef struct App {
    bool is_running;
    Window* window;
} App;

static App* app = NULL;

void app_on_event(EventType type, void* event, void* user_data) {
    if (type == EVENT_TYPE_QUIT)
        app_stop();
}

App* app_new() {
    PEER_INFO("creating a new app! ");

    app = (App*)malloc(sizeof(App));

    app->is_running = true;
    app->window = window_new();

    event_subscribe(app_on_event, NULL);

    return app;
}

void app_delete() {
    window_delete(app->window);
    free(app);
}

App* app_get() {
    if (app == NULL)
        return app_new();
    return app;
}

void app_stop() {
    PEER_INFO("stopping the app...");
    app_get()->is_running = false;
}

void app_run() {
    while (app_is_running()) {
        window_update(app_get()->window);
    }

    app_delete();
}

bool app_is_running() {
    return app_get()->is_running;
}
