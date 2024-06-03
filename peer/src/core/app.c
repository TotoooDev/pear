#include <core/app.h>
#include <core/log.h>
#include <stdlib.h>

typedef struct App {
    bool is_running;
} App;

static App* app = NULL;

App* app_new() {
    PEER_INFO("creating a new app! ");

    app = (App*)malloc(sizeof(App));

    app->is_running = false;

    return app;
}

App* app_get() {
    if (app == NULL)
        return app_new();
    return app;
}

void app_start() {
    app_get()->is_running = true;
}

void app_stop() {
    PEER_INFO("stopping the app...");
    app_get()->is_running = false;
}

bool app_is_running() {
    return app_get()->is_running;
}
