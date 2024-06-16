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
    app->renderer = renderer_new();

    event_subscribe(app_on_event, NULL);

    return app;
}

void app_delete() {
    renderer_delete(app->renderer);
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
    app_get();

    f32 vertices[] = {
         0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,
         0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, 0.0f,   0.0f, 1.0f, 1.0f
    };
    u32 indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    Mesh* mesh = mesh_new(vertices, indices, sizeof(vertices), sizeof(indices));

    while (app_is_running()) {
        renderer_clear(app_get()->renderer, 0.3f, 0.3f, 0.3f, 0.0f);
        renderer_draw_mesh(app_get()->renderer, mesh, NULL);
        window_update(app_get()->window);
    }

    mesh_delete(mesh);
    app_delete();
}

bool app_is_running() {
    return app_get()->is_running;
}
