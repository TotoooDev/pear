#include <core/log.h>
#include <core/app.h>
#include <graphics/window.h>

int main(int argc, char* argv[]) {
    app_start();
    app_stop();

    Window* window = window_new();
    window_delete(window);

    PEER_INFO("goodbye!");

    return 0;
}